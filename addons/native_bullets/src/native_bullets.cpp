#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/classes/physics_server2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/reg_ex_match.hpp>

#include "native_bullets.h"

using namespace godot;


void NativeBullets::_bind_methods() {
	ClassDB::bind_method(D_METHOD("mount", "bullets_environment"), &NativeBullets::mount);
	ClassDB::bind_method(D_METHOD("unmount", "bullets_environment"), &NativeBullets::unmount);
	ClassDB::bind_method(D_METHOD("get_bullets_environment"), &NativeBullets::get_bullets_environment);
	ClassDB::bind_method(D_METHOD("spawn_bullet", "kit", "properties"), &NativeBullets::spawn_bullet);
	ClassDB::bind_method(D_METHOD("obtain_bullet", "kit"), &NativeBullets::obtain_bullet);
	ClassDB::bind_method(D_METHOD("release_bullet", "id"), &NativeBullets::release_bullet);
	ClassDB::bind_method(D_METHOD("is_bullet_valid", "id"), &NativeBullets::is_bullet_valid);
	ClassDB::bind_method(D_METHOD("is_kit_valid", "kit"), &NativeBullets::is_kit_valid);
	ClassDB::bind_method(D_METHOD("get_available_bullets", "kit"), &NativeBullets::get_available_bullets);
	ClassDB::bind_method(D_METHOD("get_active_bullets", "kit"), &NativeBullets::get_active_bullets);
	ClassDB::bind_method(D_METHOD("get_pool_size", "kit"), &NativeBullets::get_pool_size);
	ClassDB::bind_method(D_METHOD("get_z_index", "kit"), &NativeBullets::get_z_index);
	ClassDB::bind_method(D_METHOD("get_total_available_bullets"), &NativeBullets::get_total_available_bullets);
	ClassDB::bind_method(D_METHOD("get_total_active_bullets"), &NativeBullets::get_total_active_bullets);
	ClassDB::bind_method(D_METHOD("is_bullet_existing", "area_rid", "shape_index"), &NativeBullets::is_bullet_existing);
	ClassDB::bind_method(D_METHOD("get_bullet_from_shape", "area_rid", "shape_index"), &NativeBullets::get_bullet_from_shape);
	ClassDB::bind_method(D_METHOD("get_kit_from_bullet", "bullet_id"), &NativeBullets::get_kit_from_bullet);
	ClassDB::bind_method(D_METHOD("set_bullet_property", "bullet_id", "property", "value"), &NativeBullets::set_bullet_property);
	ClassDB::bind_method(D_METHOD("get_bullet_property", "bullet_id", "property"), &NativeBullets::get_bullet_property);
}

NativeBullets::NativeBullets() {
	available_bullets = 0;
	active_bullets = 0;
	total_bullets = 0;
	invalid_id = PackedInt32Array();
	invalid_id.resize(3);
	invalid_id.set(0, -1);
	invalid_id.set(1, -1);
	invalid_id.set(2, -1);
}

NativeBullets::~NativeBullets() {
	_clear_rids();
}

void NativeBullets::_physics_process(float delta) {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	int32_t bullets_variation = 0;

	for(int32_t i = 0; i < pool_sets.size(); i++) {
		for(int32_t j = 0; j < pool_sets[i].pools.size(); j++) {
			bullets_variation = pool_sets[i].pools[j].pool->_process(delta);
			available_bullets -= bullets_variation;
			active_bullets += bullets_variation;
		}
	}
}

void NativeBullets::_clear_rids() {
	for(int32_t i = 0; i < shared_areas.size(); i++) {
		PhysicsServer2D::get_singleton()->area_clear_shapes(shared_areas[i]);
		PhysicsServer2D::get_singleton()->free_rid(shared_areas[i]);
	}
}

int32_t NativeBullets::_get_pool_index(int32_t set_index, int32_t bullet_index) {
	if(bullet_index >= 0 && set_index >= 0 && set_index < pool_sets.size() && bullet_index < pool_sets[set_index].bullets_amount) {
		int32_t pool_threshold = pool_sets[set_index].pools[0].size;
		int32_t pool_index = 0;

		while(bullet_index >= pool_threshold) {
			pool_index++;
			pool_threshold += pool_sets[set_index].pools[pool_index].size;
		}
		if(pool_index < pool_sets[set_index].pools.size()) {
			return pool_index;
		}
	}
	return -1;
}

void NativeBullets::mount(Node* bullets_environment) {
	if(bullets_environment == nullptr || this->bullets_environment == bullets_environment) {
		return;
	}
	if(this->bullets_environment != nullptr) {
		this->bullets_environment->set("current", false);
	}
	
	this->bullets_environment = bullets_environment;
	this->bullets_environment->set("current", true);

	Array bullet_kits = bullets_environment->get("bullet_kits");
	Array pools_sizes = bullets_environment->get("pools_sizes");
	Array parents_paths = bullets_environment->get("parents_hints");
	Array z_indices = bullets_environment->get("z_indices");

	pool_sets.clear();
	areas_to_pool_set_indices.clear();
	kits_to_set_pool_indices.clear();
	_clear_rids();
	shared_areas.clear();

	available_bullets = 0;
	active_bullets = 0;

	Dictionary collision_layers_masks_to_kits;

	Viewport* default_viewport = bullets_environment->get_viewport();
	RID default_parent_canvas = default_viewport->find_world_2d()->get_canvas();
	
	for(int32_t i = 0; i < bullet_kits.size(); i++) {
		Ref<BulletKit> kit = bullet_kits[i];
		if(!kit->is_valid()) {
			ERR_PRINT("BulletKit is not valid!");
			continue;
		}
		// By default add the the BulletKit to a no-collisions list. (layer and mask = 0)
		int64_t layer_mask = 0;
		if(kit->collisions_enabled && kit->collision_shape.is_valid()) {
			// If collisions are enabled, add the BulletKit to another list.
			layer_mask = (int64_t)kit->collision_layer + ((int64_t)kit->collision_mask << 32);
		}
		if(collision_layers_masks_to_kits.has(layer_mask)) {
			collision_layers_masks_to_kits[layer_mask].operator Array().append(kit);
		}
		else {
			Array array = Array();
			array.append(kit);
			collision_layers_masks_to_kits[layer_mask] = array;
		}
	}
	// Create the PoolKitSets array. If they exist, a set will be allocated for no-collisions pools.
	pool_sets.resize(collision_layers_masks_to_kits.size());
	
	Array layer_mask_keys = collision_layers_masks_to_kits.keys();
	for(int32_t i = 0; i < layer_mask_keys.size(); i++) {
		Array kits = collision_layers_masks_to_kits[layer_mask_keys[i]];
		Ref<BulletKit> first_kit = kits[0];

		pool_sets[i].pools.resize(kits.size());

		RID shared_area = RID();
		RID world_space = get_viewport()->get_world_2d()->get_space();
		if(layer_mask_keys[i].operator int64_t() != 0) {
			// This is a collisions-enabled set, create the shared area.
			shared_area = PhysicsServer2D::get_singleton()->area_create();
			PhysicsServer2D::get_singleton()->area_set_collision_layer(shared_area, first_kit->collision_layer);
			PhysicsServer2D::get_singleton()->area_set_collision_mask(shared_area, first_kit->collision_mask);
			PhysicsServer2D::get_singleton()->area_set_monitorable(shared_area, true);
			PhysicsServer2D::get_singleton()->area_set_space(shared_area, world_space);

			shared_areas.push_back(shared_area);
			areas_to_pool_set_indices[shared_area] = i;
		}
		int32_t pool_set_available_bullets = 0;

		for(int32_t j = 0; j < kits.size(); j++) {
			Ref<BulletKit> kit = kits[j];

			PackedInt32Array set_pool_indices = PackedInt32Array();
			set_pool_indices.resize(2);
			set_pool_indices.set(0, i);
			set_pool_indices.set(1, j);
			kits_to_set_pool_indices[kit] = set_pool_indices;
			
			int32_t kit_index_in_node = bullet_kits.find(kit);
			int32_t pool_size = pools_sizes[kit_index_in_node];

			Node* parent_node_hint = nullptr;

			NodePath parent_path = parents_paths[0];
			if (!parent_path.is_empty()) {
				parent_node_hint = bullets_environment->get_node<Node>(parent_path);
			}
			if (parent_node_hint == nullptr) {
				parent_node_hint = bullets_environment;
			}

			pool_sets[i].pools[j].pool = kit->_create_pool();
			pool_sets[i].pools[j].bullet_kit = kit;
			pool_sets[i].pools[j].size = pool_size;
			pool_sets[i].pools[j].z_index = z_indices[kit_index_in_node];

			pool_sets[i].pools[j].pool->_init(parent_node_hint, shared_area, pool_set_available_bullets,
				i, kit, pool_size, z_indices[kit_index_in_node]);

			pool_set_available_bullets += pool_size;
		}
		pool_sets[i].bullets_amount = pool_set_available_bullets;
		available_bullets += pool_set_available_bullets;
	}
	total_bullets = available_bullets;
}

void NativeBullets::unmount(Node* bullets_environment) {
	if(this->bullets_environment == bullets_environment) {
		pool_sets.clear();
		areas_to_pool_set_indices.clear();
		kits_to_set_pool_indices.clear();
		_clear_rids();
		shared_areas.clear();

		available_bullets = 0;
		active_bullets = 0;
		total_bullets = 0;

		this->bullets_environment = nullptr;
	}
	if(bullets_environment != nullptr) {
		bullets_environment->set("current", false);
	}
}

Node* NativeBullets::get_bullets_environment() {
	return bullets_environment;
}

bool NativeBullets::spawn_bullet(Ref<BulletKit> kit, Dictionary properties) {
	if(available_bullets > 0 && kits_to_set_pool_indices.has(kit)) {
		PackedInt32Array set_pool_indices = kits_to_set_pool_indices[kit].operator PackedInt32Array();
		BulletsPool* pool = pool_sets[set_pool_indices[0]].pools[set_pool_indices[1]].pool.get();

		if(pool->get_available_bullets() > 0) {
			available_bullets -= 1;
			active_bullets += 1;

			pool->spawn_bullet(properties);
			return true;
		}
	}
	return false;
}

Variant NativeBullets::obtain_bullet(Ref<BulletKit> kit) {
	if(available_bullets > 0 && kits_to_set_pool_indices.has(kit)) {
		PackedInt32Array set_pool_indices = kits_to_set_pool_indices[kit].operator PackedInt32Array();
		BulletsPool* pool = pool_sets[set_pool_indices[0]].pools[set_pool_indices[1]].pool.get();

		if(pool->get_available_bullets() > 0) {
			available_bullets -= 1;
			active_bullets += 1;

			BulletID bullet_id = pool->obtain_bullet();
			PackedInt32Array to_return = invalid_id;
			to_return.set(0, bullet_id.index);
			to_return.set(1, bullet_id.cycle);
			to_return.set(2, bullet_id.set);

			const String description = "Returning a BulletID: {0}, {1}, {2}.";
			WARN_PRINT(description.format(Array::make(to_return[0], to_return[1], to_return[2])));

			return to_return;
		}
	}
	return invalid_id;
}

bool NativeBullets::release_bullet(Variant id) {
	PackedInt32Array bullet_id = id.operator PackedInt32Array();

	if (bullet_id.size() < 3) {
		ERR_PRINT("Bullet ID malformed!");
		return false;
	}

	bool result = false;

	int32_t pool_index = _get_pool_index(bullet_id[2], bullet_id[0]);
	if(pool_index >= 0) {
		result = pool_sets[bullet_id[2]].pools[pool_index].pool->release_bullet(BulletID(bullet_id[0], bullet_id[1], bullet_id[2]));
		if(result) {
			available_bullets += 1;
			active_bullets -= 1;
		}
	}
	return result;
}

bool NativeBullets::is_bullet_valid(Variant id) {
	PackedInt32Array bullet_id = id.operator PackedInt32Array();

	if (bullet_id.size() < 3) {
		ERR_PRINT("Bullet ID malformed!");
		return false;
	}

	int32_t pool_index = _get_pool_index(bullet_id[2], bullet_id[0]);
	if(pool_index >= 0) {
		return pool_sets[bullet_id[2]].pools[pool_index].pool->is_bullet_valid(BulletID(bullet_id[0], bullet_id[1], bullet_id[2]));
	}
	return false;
}

bool NativeBullets::is_kit_valid(Ref<BulletKit> kit) {
	return kits_to_set_pool_indices.has(kit);
}

int32_t NativeBullets::get_available_bullets(Ref<BulletKit> kit) {
	if(kits_to_set_pool_indices.has(kit)) {
		PackedInt32Array set_pool_indices = kits_to_set_pool_indices[kit];
		return pool_sets[set_pool_indices[0]].pools[set_pool_indices[1]].pool->get_available_bullets();
	}
	return 0;
}

int32_t NativeBullets::get_active_bullets(Ref<BulletKit> kit) {
	if(kits_to_set_pool_indices.has(kit)) {
		PackedInt32Array set_pool_indices = kits_to_set_pool_indices[kit];
		return pool_sets[set_pool_indices[0]].pools[set_pool_indices[1]].pool->get_active_bullets();
	}
	return 0;
}

int32_t NativeBullets::get_pool_size(Ref<BulletKit> kit) {
	if(kits_to_set_pool_indices.has(kit)) {
		PackedInt32Array set_pool_indices = kits_to_set_pool_indices[kit];
		return pool_sets[set_pool_indices[0]].pools[set_pool_indices[1]].size;
	}
	return 0;
}

int32_t NativeBullets::get_z_index(Ref<BulletKit> kit) {
	if(kits_to_set_pool_indices.has(kit)) {
		PackedInt32Array set_pool_indices = kits_to_set_pool_indices[kit];
		return pool_sets[set_pool_indices[0]].pools[set_pool_indices[1]].z_index;
	}
	return 0;
}

int32_t NativeBullets::get_total_available_bullets() {
	return available_bullets;
}

int32_t NativeBullets::get_total_active_bullets() {
	return active_bullets;
}

bool NativeBullets::is_bullet_existing(RID area_rid, int32_t shape_index) {
	if(!areas_to_pool_set_indices.has(area_rid)) {
		return false;
	}
	int32_t set_index = areas_to_pool_set_indices[area_rid];
	int32_t pool_index = _get_pool_index(set_index, shape_index);
	if(pool_index >= 0) {
		return pool_sets[set_index].pools[pool_index].pool->is_bullet_existing(shape_index);
	}
	return false;
}

Variant NativeBullets::get_bullet_from_shape(RID area_rid, int32_t shape_index) {
	if(!areas_to_pool_set_indices.has(area_rid)) {
		return invalid_id;
	}
	int32_t set_index = areas_to_pool_set_indices[area_rid];
	int32_t pool_index = _get_pool_index(set_index, shape_index);
	if(pool_index >= 0) {
		BulletID result = pool_sets[set_index].pools[pool_index].pool->get_bullet_from_shape(shape_index);

		PackedInt32Array to_return = invalid_id;
		to_return.set(0, result.index);
		to_return.set(1, result.cycle);
		to_return.set(2, result.set);
		return to_return;
	}
	return invalid_id;
}

Ref<BulletKit> NativeBullets::get_kit_from_bullet(Variant id) {
	PackedInt32Array bullet_id = id.operator PackedInt32Array();

	if (bullet_id.size() < 3) {
		ERR_PRINT("Bullet ID malformed!");
		return nullptr;
	}

	int32_t pool_index = _get_pool_index(bullet_id[2], bullet_id[0]);
	if(pool_index >= 0 && pool_sets[bullet_id[2]].pools[pool_index].pool->is_bullet_valid(BulletID(bullet_id[0], bullet_id[1], bullet_id[2]))) {
		return pool_sets[bullet_id[2]].pools[pool_index].bullet_kit;
	}
	return nullptr;
}

void NativeBullets::set_bullet_property(Variant id, String property, Variant value) {
	PackedInt32Array bullet_id = id.operator PackedInt32Array();

	ERR_FAIL_COND_MSG(bullet_id.size() < 3, "Bullet ID malformed!");

	int32_t pool_index = _get_pool_index(bullet_id[2], bullet_id[0]);
	if(pool_index >= 0) {
		pool_sets[bullet_id[2]].pools[pool_index].pool->set_bullet_property(BulletID(bullet_id[0], bullet_id[1], bullet_id[2]), property, value);
	}
}

Variant NativeBullets::get_bullet_property(Variant id, String property) {
	PackedInt32Array bullet_id = id.operator PackedInt32Array();

	if (bullet_id.size() < 3) {
		ERR_PRINT("Bullet ID malformed!");
		return Variant();
	}

	int32_t pool_index = _get_pool_index(bullet_id[2], bullet_id[0]);
	if(pool_index >= 0) {
		return pool_sets[bullet_id[2]].pools[pool_index].pool->get_bullet_property(BulletID(bullet_id[0], bullet_id[1], bullet_id[2]), property);
	}
	return Variant();
}