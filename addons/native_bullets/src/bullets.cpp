#include <VisualServer.hpp>
#include <Physics2DServer.hpp>
#include <World2D.hpp>
#include <Viewport.hpp>
#include <OS.hpp>
#include <Engine.hpp>
#include <Font.hpp>
#include <RegExMatch.hpp>

#include "bullets.h"

using namespace godot;

void Bullets::_register_methods() {
	register_method("_ready", &Bullets::_ready);
	register_method("_process", &Bullets::_process);
	register_method("_physics_process", &Bullets::_physics_process);
	register_method("_draw", &Bullets::_draw);

	register_method("_get", &Bullets::_get);
	register_method("_set", &Bullets::_set);
	register_method("_get_property_list", &Bullets::_get_property_list);

	register_method("spawn_bullet", &Bullets::spawn_bullet);
	register_method("obtain_bullet", &Bullets::obtain_bullet);
	register_method("release_bullet", &Bullets::release_bullet);
	register_method("get_available_bullets", &Bullets::get_available_bullets);
	register_method("get_active_bullets", &Bullets::get_active_bullets);
	register_method("get_bullet_from_shape", &Bullets::get_bullet_from_shape);
	register_method("get_kit_from_bullet", &Bullets::get_kit_from_bullet);

	register_method("set_bullet_property", &Bullets::set_bullet_property);
	register_method("get_bullet_property", &Bullets::get_bullet_property);

	register_property<Bullets, int32_t>("bullets_collision_layer", &Bullets::bullets_collision_layer, 0, 
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_LAYERS_2D_PHYSICS);
	register_property<Bullets, int32_t>("bullets_collision_mask", &Bullets::bullets_collision_mask, 0, 
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_LAYERS_2D_PHYSICS);

	register_property<Bullets, Array>("bullet_kits", &Bullets::bullet_kits, Array(),
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RESOURCE_TYPE, "BulletKit");
	register_property<Bullets, Array>("pools_sizes", &Bullets::pools_sizes, Array(),
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "1,100000");
	register_property<Bullets, Array>("z_indices", &Bullets::z_indices, Array(),
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "-128,128");

	register_property<Bullets, Ref<Font> >("debug_font", &Bullets::debug_font, Ref<Font>(),
		GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RESOURCE_TYPE, "Font");
	register_property<Bullets, bool>("debug", &Bullets::debug, false);
}

Bullets::Bullets() { }

Bullets::~Bullets() {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	for(int32_t i = 0; i < bullet_kits.size(); i++) {
		delete pools[i].pool;
	}
	delete[] pools;

	Physics2DServer::get_singleton()->area_clear_shapes(shared_area);
	Physics2DServer::get_singleton()->free_rid(shared_area);
}

void Bullets::_init() {
	available_bullets = 0;
	active_bullets = 0;
	kits_to_pools = Dictionary();
	debug = false;
	invalid_id = PoolIntArray();
	invalid_id.resize(2);
	invalid_id.set(0, -1);
	invalid_id.set(1, -1);

	prop_regex = Ref<RegEx>(RegEx::_new());
	prop_regex->compile("bullet_types/\\d*/\\w*");
}

void Bullets::_ready() {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}

	pools = new PoolKit[bullet_kits.size()];

	available_bullets = 0;
	active_bullets = 0;

	shared_area = Physics2DServer::get_singleton()->area_create();
	Physics2DServer::get_singleton()->area_set_collision_layer(shared_area, bullets_collision_layer);
	Physics2DServer::get_singleton()->area_set_collision_mask(shared_area, bullets_collision_mask);
	Physics2DServer::get_singleton()->area_set_monitorable(shared_area, true);
	Physics2DServer::get_singleton()->area_set_space(shared_area, get_world_2d()->get_space());

	int32_t no_collisions_pools = 0;

	for(int32_t i = 0; i < bullet_kits.size(); i++) {
		Ref<BulletKit> kit = Object::cast_to<BulletKit>(bullet_kits[i]);
		BulletsPool* pool = kit->_create_pool();

		int32_t index;
		if(kit->collisions_enabled) {
			index = i - no_collisions_pools;
		} else {
			// If this is a no collision kit, assign the pool at the end of the array.
			no_collisions_pools += 1;
			index = bullet_kits.size() - no_collisions_pools;
		}
		pools[index].pool = pool;
		pools[index].bullet_kit = kit;
		pools[index].size = pools_sizes[i];
		pools[index].z_index = z_indices[i];
	}

	for(int32_t i = 0; i < bullet_kits.size(); i++) {
		PoolKit* pool = &pools[i];

		pool->pool->_init(pool->bullet_kit, this, pool->z_index, shared_area, available_bullets, pool->size);

		available_bullets += pool->size;
		kits_to_pools[pool->bullet_kit] = i;
	}
	total_bullets = available_bullets;
}

void Bullets::_physics_process(float delta) {
	if(Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	int32_t bullets_variation = 0;
	for(int32_t i = 0; i < bullet_kits.size(); i++) {
		bullets_variation = pools[i].pool->_process(delta);
		available_bullets -= bullets_variation;
		active_bullets += bullets_variation;
	}
	if(debug) {
		update();
	}
}

void Bullets::_process(float delta) {

}

void Bullets::_draw() {
	if(debug && !Engine::get_singleton()->is_editor_hint()) {
		for(int32_t i = 0; i < bullet_kits.size(); i++) {
			pools[i].pool->_draw(debug_font);
		}
	}
}

Variant Bullets::_get(String property) {
	if(property == "bullet_types_amount") {
		return bullet_kits.size();
	}
	Ref<RegExMatch> result = prop_regex->search(property);

	if(result.is_valid()) {
		PoolStringArray strings = result->get_string().split("/");
		int prop_index = (int)strings[1].to_int();
		if(bullet_kits.size() > prop_index) {
			if(strings[2] == "bullet_kit"){
				return bullet_kits[prop_index];
			}
			else if(strings[2] == "pool_size") {
				return pools_sizes[prop_index];
			} 
			else if(strings[2] == "z_index") {
				return z_indices[prop_index];
			}
		}
	}

	return Variant((Object*)nullptr);
}

bool Bullets::_set(String property, Variant value) {
	if(property == "bullet_types_amount") {
		bullet_kits.resize(value);
		pools_sizes.resize(value);
		z_indices.resize(value);
		property_list_changed_notify();
		return true;
	}
	Ref<RegExMatch> result = prop_regex->search(property);

	if(result.is_valid()) {
		PoolStringArray strings = result->get_string().split("/");
		int prop_index = (int)strings[1].to_int();
		if(bullet_kits.size() > prop_index) {
			if(strings[2] == "bullet_kit"){
				bullet_kits[prop_index] = value;
				return true;
			}
			else if(strings[2] == "pool_size") {
				pools_sizes[prop_index] = value;
				return true;
			} 
			else if(strings[2] == "z_index") {
				z_indices[prop_index] = value;
				return true;
			}
		}
	}

	return false;
}

Array Bullets::_get_property_list() {
	Array properties = Array();

	Dictionary kits_amount = Dictionary();
	kits_amount["name"] = "bullet_types_amount";
	kits_amount["type"] = GODOT_VARIANT_TYPE_INT;
	kits_amount["usage"] = GODOT_PROPERTY_USAGE_DEFAULT;
	kits_amount["hint"] = GODOT_PROPERTY_HINT_RANGE;
	kits_amount["hint_string"] = "0,64";

	properties.append(kits_amount);

	for(int32_t i = 0; i < bullet_kits.size(); i++) {
		Array index = Array();
		index.append(i);

		Dictionary kit = Dictionary();
		kit["name"] = String("bullet_types/{0}/bullet_kit").format(index);
		kit["type"] = GODOT_VARIANT_TYPE_OBJECT;
		kit["usage"] = GODOT_PROPERTY_USAGE_DEFAULT;
		kit["hint"] = GODOT_PROPERTY_HINT_RESOURCE_TYPE;
		kit["hint_string"] = "Resource";

		Dictionary size = Dictionary();
		size["name"] = String("bullet_types/{0}/pool_size").format(index);
		size["type"] = GODOT_VARIANT_TYPE_INT;
		size["usage"] = GODOT_PROPERTY_USAGE_DEFAULT;
		size["hint"] = GODOT_PROPERTY_HINT_RANGE;
		size["hint_string"] = "1,65536";

		Dictionary z_index = Dictionary();
		z_index["name"] = String("bullet_types/{0}/z_index").format(index);
		z_index["type"] = GODOT_VARIANT_TYPE_INT;
		z_index["usage"] = GODOT_PROPERTY_USAGE_DEFAULT;
		z_index["hint"] = GODOT_PROPERTY_HINT_RANGE;
		z_index["hint_string"] = "-4096,4096";

		properties.append(kit);
		properties.append(size);
		properties.append(z_index);
	}

	return properties;
}

int32_t Bullets::_get_pool_index(int32_t bullet_index) {
	if(bullet_index >= 0 && bullet_index < total_bullets && bullet_kits.size() > 0) {
		int32_t pool_threshold = pools[0].size;
		int32_t pool_index = 0;

		while(bullet_index >= pool_threshold) {
			pool_index++;
			pool_threshold += pools[pool_index].size;
		}
		if(pool_index < bullet_kits.size()) {
			return pool_index;
		}
	}
	return -1;
}

void Bullets::spawn_bullet(Ref<BulletKit> kit, Dictionary properties) {
	if(available_bullets > 0 && kits_to_pools.has(kit)) {
		BulletsPool* pool = pools[kits_to_pools[kit].operator int()].pool;

		if(pool->get_available_bullets() > 0) {
			available_bullets -= 1;
			active_bullets += 1;

			pool->spawn_bullet(properties);
		}
	}
}

Variant Bullets::obtain_bullet(Ref<BulletKit> kit) {
	if(available_bullets > 0 && kits_to_pools.has(kit)) {
		BulletsPool* pool = pools[kits_to_pools[kit].operator int()].pool;

		if(pool->get_available_bullets() > 0) {
			available_bullets -= 1;
			active_bullets += 1;

			BulletID bullet_id = pool->obtain_bullet();
			PoolIntArray to_return = invalid_id;
			to_return.set(0, bullet_id.index);
			to_return.set(1, bullet_id.cycle);
			return to_return;
		}
	}
	return invalid_id;
}

bool Bullets::release_bullet(Variant id) {
	PoolIntArray bullet_id = id.operator PoolIntArray();
	bool result = false;

	int32_t pool_index = _get_pool_index(bullet_id[0]);
	if(pool_index >= 0) {
		result = pools[pool_index].pool->release_bullet(BulletID(bullet_id[0], bullet_id[1]));
		if(result) {
			available_bullets += 1;
			active_bullets -= 1;
		}
	}
	return result;
}

bool Bullets::is_bullet_valid(Variant id) {
	PoolIntArray bullet_id = id.operator PoolIntArray();

	int32_t pool_index = _get_pool_index(bullet_id[0]);
	if(pool_index >= 0) {
		return pools[pool_index].pool->is_bullet_valid(BulletID(bullet_id[0], bullet_id[1]));
	}
	return false;
}

int32_t Bullets::get_available_bullets() {
	return available_bullets;
}

int32_t Bullets::get_active_bullets() {
	return active_bullets;
}

Variant Bullets::get_bullet_from_shape(int32_t shape_index) {
	int32_t pool_index = _get_pool_index(shape_index);
	if(pool_index >= 0) {
		BulletID result = pools[pool_index].pool->get_bullet_from_shape(shape_index);

		PoolIntArray to_return = invalid_id;
		to_return.set(0, result.index);
		to_return.set(1, result.cycle);
		return to_return;
	}
	return invalid_id;
}

Ref<BulletKit> Bullets::get_kit_from_bullet(Variant id) {
	PoolIntArray bullet_id = id.operator PoolIntArray();

	int32_t pool_index = _get_pool_index(bullet_id[0]);
	if(pool_index >= 0 && pools[pool_index].pool->is_bullet_valid(BulletID(bullet_id[0], bullet_id[1]))) {
		return pools[pool_index].bullet_kit;
	}
	return Ref<BulletKit>();
}

void Bullets::set_bullet_property(Variant id, String property, Variant value) {
	PoolIntArray bullet_id = id.operator PoolIntArray();

	int32_t pool_index = _get_pool_index(bullet_id[0]);
	if(pool_index >= 0) {
		pools[pool_index].pool->set_bullet_property(BulletID(bullet_id[0], bullet_id[1]), property, value);
	}
}

Variant Bullets::get_bullet_property(Variant id, String property) {
	PoolIntArray bullet_id = id.operator PoolIntArray();

	int32_t pool_index = _get_pool_index(bullet_id[0]);
	if(pool_index >= 0) {
		return pools[pool_index].pool->get_bullet_property(BulletID(bullet_id[0], bullet_id[1]), property);
	}
	return Variant();
}