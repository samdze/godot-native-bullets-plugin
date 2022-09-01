#include <Godot.hpp>
#include <VisualServer.hpp>
#include <World2D.hpp>
#include <Physics2DServer.hpp>
#include <Viewport.hpp>
#include <Font.hpp>

#include "bullets_pool.h"

using namespace godot;


//-- START Default "standard" implementations.

template <class Kit, class BulletType>
void AbstractBulletsPool<Kit, BulletType>::_init_bullet(BulletType* bullet) {}

template <class Kit, class BulletType>
void AbstractBulletsPool<Kit, BulletType>::_enable_bullet(BulletType* bullet) {
	bullet->lifetime = 0.0f;

	Rect2 texture_rect = Rect2(-kit->texture->get_size() / 2.0f, kit->texture->get_size());
	RID texture_rid = kit->texture->get_rid();
	
	VisualServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
		texture_rect,
		texture_rid);
}

template <class Kit, class BulletType>
void AbstractBulletsPool<Kit, BulletType>::_disable_bullet(BulletType* bullet) {
	VisualServer::get_singleton()->canvas_item_clear(bullet->item_rid);
}

template <class Kit, class BulletType>
bool AbstractBulletsPool<Kit, BulletType>::_process_bullet(BulletType* bullet, float delta) {
	bullet->transform.set_origin(bullet->transform.get_origin() + bullet->velocity * delta);

	if(!active_rect.has_point(bullet->transform.get_origin())) {
		return true;
	}

	bullet->lifetime += delta;
	return false;
}

//-- END Default "standard" implementation.

template <class Kit, class BulletType>
AbstractBulletsPool<Kit, BulletType>::~AbstractBulletsPool() {
	// Bullets node is responsible for clearing all the area and area shapes
	for(int32_t i = 0; i < pool_size; i++) {
		VisualServer::get_singleton()->free_rid(bullets[i]->item_rid);
		bullets[i]->free();
	}
	VisualServer::get_singleton()->free_rid(canvas_item);

	delete[] bullets;
	delete[] shapes_to_indices;
}

template <class Kit, class BulletType>
void AbstractBulletsPool<Kit, BulletType>::_init(Node* parent_hint, RID shared_area, int32_t starting_shape_index,
		int32_t set_index, Ref<BulletKit> kit, int32_t pool_size, int32_t z_index) {
	
	// Check if collisions are enabled and if layer or mask are != 0, 
	// otherwise the bullets would not collide with anything anyways.
	this->collisions_enabled = kit->collisions_enabled && kit->collision_shape.is_valid() &&
		((int64_t)kit->collision_layer + (int64_t)kit->collision_mask) != 0;
	// this->viewport = viewport;
	this->shared_area = shared_area;
	this->starting_shape_index = starting_shape_index;
	this->kit = kit;
	this->pool_size = pool_size;
	this->set_index = set_index;

	this->viewport = nullptr;
	this->canvas_layer = nullptr;

	Node* n = parent_hint;
	while (n) {
		if (!this->canvas_layer) {
			this->canvas_layer = Object::cast_to<CanvasLayer>(n);
		}
		this->viewport = Object::cast_to<Viewport>(n);
		if (this->viewport) {
			break;
		}
		
		n = n->get_parent();
	}
	if (this->canvas_layer) {
		this->canvas_parent = canvas_layer->get_canvas();
	} else {
		this->canvas_parent = viewport->find_world_2d()->get_canvas();
	}

	available_bullets = pool_size;
	active_bullets = 0;

	bullets = new BulletType*[pool_size];
	shapes_to_indices = new int32_t[pool_size];

	canvas_item = VisualServer::get_singleton()->canvas_item_create();
	VisualServer::get_singleton()->canvas_item_set_parent(canvas_item, canvas_parent);
	VisualServer::get_singleton()->canvas_item_set_z_index(canvas_item, z_index);

	for(int32_t i = 0; i < pool_size; i++) {
		BulletType* bullet = BulletType::_new();
		bullets[i] = bullet;

		bullet->item_rid = VisualServer::get_singleton()->canvas_item_create();
		VisualServer::get_singleton()->canvas_item_set_parent(bullet->item_rid, canvas_item);
		VisualServer::get_singleton()->canvas_item_set_material(bullet->item_rid, kit->material->get_rid());

		if(collisions_enabled) {
			RID shared_shape_rid = kit->collision_shape->get_rid();

			Physics2DServer::get_singleton()->area_add_shape(shared_area, shared_shape_rid, Transform2D(), true);
			bullet->shape_index = starting_shape_index + i;
			shapes_to_indices[i] = i;
		}

		Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		switch(kit->unique_modulate_component) {
			case 1: // Red
				color.r = fmod((starting_shape_index + i) * 0.7213f, 1.0f);
				break;
			case 2: // Green
				color.g = fmod((starting_shape_index + i) * 0.7213f, 1.0f);
				break;
			case 3: // Blue
				color.b = fmod((starting_shape_index + i) * 0.7213f, 1.0f);
				break;
			case 4: // Alpha
				color.a = fmod((starting_shape_index + i) * 0.7213f, 1.0f);
				break;
			default: // None or other values
				break;
		}
		VisualServer::get_singleton()->canvas_item_set_modulate(bullet->item_rid, color);

		_init_bullet(bullet);
	}
}

template <class Kit, class BulletType>
int32_t AbstractBulletsPool<Kit, BulletType>::_process(float delta) {
	if(kit->use_viewport_as_active_rect) {
		Rect2 viewport_rect = viewport->get_visible_rect();
		Transform2D viewport_inv_transform = canvas_layer ? canvas_layer->get_transform().affine_inverse() : viewport->get_canvas_transform().affine_inverse();
		Vector2 top_left_point = viewport_inv_transform.xform(Vector2::ZERO);
		Vector2 top_right_point = viewport_inv_transform.xform(Vector2(viewport_rect.size.x, 0));
		Vector2 bot_right_point = viewport_inv_transform.xform(viewport_rect.size);
		Vector2 bot_left_point = viewport_inv_transform.xform(Vector2(0, viewport_rect.size.y));

		Vector2 origin = Vector2(Math::min(top_left_point.x, Math::min(top_right_point.x, Math::min(bot_right_point.x, bot_left_point.x))),
			Math::min(top_left_point.y, Math::min(top_right_point.y, Math::min(bot_right_point.y, bot_left_point.y))));
		Vector2 edge = Vector2(Math::max(top_left_point.x, Math::max(top_right_point.x, Math::max(bot_right_point.x, bot_left_point.x))),
			Math::max(top_left_point.y, Math::max(top_right_point.y, Math::max(bot_right_point.y, bot_left_point.y))));

		active_rect = Rect2(origin, edge - origin);
	} else {
		active_rect = kit->active_rect;
	}
	int32_t amount_variation = 0;

	if(collisions_enabled) {
		for(int32_t i = pool_size - 1; i >= available_bullets; i--) {
			BulletType* bullet = bullets[i];

			if(_process_bullet(bullet, delta)) {
				_release_bullet(i);
				amount_variation -= 1;
				i += 1;
				continue;
			}
			
			VisualServer::get_singleton()->canvas_item_set_transform(bullet->item_rid, bullet->transform);
			Physics2DServer::get_singleton()->area_set_shape_transform(shared_area, bullet->shape_index, bullet->transform);
		}
	} else {
		for(int32_t i = pool_size - 1; i >= available_bullets; i--) {
			BulletType* bullet = bullets[i];

			if(_process_bullet(bullet, delta)) {
				_release_bullet(i);
				amount_variation -= 1;
				i += 1;
				continue;
			}
			
			VisualServer::get_singleton()->canvas_item_set_transform(bullet->item_rid, bullet->transform);
		}
	}
	return amount_variation;
}

template <class Kit, class BulletType>
void AbstractBulletsPool<Kit, BulletType>::spawn_bullet(Dictionary properties) {
	if(available_bullets > 0) {
		available_bullets -= 1;
		active_bullets += 1;

		BulletType* bullet = bullets[available_bullets];

		if(collisions_enabled)
			Physics2DServer::get_singleton()->area_set_shape_disabled(shared_area, bullet->shape_index, false);

		Array keys = properties.keys();
		for(int32_t i = 0; i < keys.size(); i++) {
			bullet->set(keys[i], properties[keys[i]]);
		}

		VisualServer::get_singleton()->canvas_item_set_transform(bullet->item_rid, bullet->transform);
		if(collisions_enabled)
			Physics2DServer::get_singleton()->area_set_shape_transform(shared_area, bullet->shape_index, bullet->transform);

		_enable_bullet(bullet);
	}
}

template <class Kit, class BulletType>
BulletID AbstractBulletsPool<Kit, BulletType>::obtain_bullet() {
	if(available_bullets > 0) {
		available_bullets -= 1;
		active_bullets += 1;

		BulletType* bullet = bullets[available_bullets];

		if(collisions_enabled)
			Physics2DServer::get_singleton()->area_set_shape_disabled(shared_area, bullet->shape_index, false);

		_enable_bullet(bullet);

		return BulletID(bullet->shape_index, bullet->cycle, set_index);
	}
	return BulletID(-1, -1, -1);
}

template <class Kit, class BulletType>
bool AbstractBulletsPool<Kit, BulletType>::release_bullet(BulletID id) {
	if(id.index >= starting_shape_index && id.index < starting_shape_index + pool_size && id.set == set_index) {
		int32_t bullet_index = shapes_to_indices[id.index - starting_shape_index];
		if(bullet_index >= available_bullets && bullet_index < pool_size && id.cycle == bullets[bullet_index]->cycle) {
			_release_bullet(bullet_index);
			return true;
		}
	}
	return false;
}

template <class Kit, class BulletType>
void AbstractBulletsPool<Kit, BulletType>::_release_bullet(int32_t index) {
	BulletType* bullet = bullets[index];
	
	if(collisions_enabled)
		Physics2DServer::get_singleton()->area_set_shape_disabled(shared_area, bullet->shape_index, true);
	
	_disable_bullet(bullet);
	bullet->cycle += 1;

	_swap(shapes_to_indices[bullet->shape_index - starting_shape_index], shapes_to_indices[bullets[available_bullets]->shape_index - starting_shape_index]);
	_swap(bullets[index], bullets[available_bullets]);

	available_bullets += 1;
	active_bullets -= 1;
}

template <class Kit, class BulletType>
bool AbstractBulletsPool<Kit, BulletType>::is_bullet_valid(BulletID id) {
	if(id.index >= starting_shape_index && id.index < starting_shape_index + pool_size && id.set == set_index) {
		int32_t bullet_index = shapes_to_indices[id.index - starting_shape_index];
		if(bullet_index >= available_bullets && bullet_index < pool_size && id.cycle == bullets[bullet_index]->cycle) {
			return true;
		}
	}
	return false;
}

template <class Kit, class BulletType>
bool AbstractBulletsPool<Kit, BulletType>::is_bullet_existing(int32_t shape_index) {
	if(shape_index >= starting_shape_index && shape_index < starting_shape_index + pool_size) {
		int32_t bullet_index = shapes_to_indices[shape_index - starting_shape_index];
		if(bullet_index >= available_bullets) {
			return true;
		}
	}
	return false;
}

template <class Kit, class BulletType>
BulletID AbstractBulletsPool<Kit, BulletType>::get_bullet_from_shape(int32_t shape_index) {
	if(shape_index >= starting_shape_index && shape_index < starting_shape_index + pool_size) {
		int32_t bullet_index = shapes_to_indices[shape_index - starting_shape_index];
		if(bullet_index >= available_bullets) {
			return BulletID(shape_index, bullets[bullet_index]->cycle, set_index);
		}
	}
	return BulletID(-1, -1, -1);
}


template <class Kit, class BulletType>
void AbstractBulletsPool<Kit, BulletType>::set_bullet_property(BulletID id, String property, Variant value) {
	if(is_bullet_valid(id)) {
		int32_t bullet_index = shapes_to_indices[id.index - starting_shape_index];
		bullets[bullet_index]->set(property, value);

		if(property == "transform") {
			BulletType* bullet = bullets[bullet_index];
			VisualServer::get_singleton()->canvas_item_set_transform(bullet->item_rid, bullet->transform);
			if(collisions_enabled)
				Physics2DServer::get_singleton()->area_set_shape_transform(shared_area, bullet->shape_index, bullet->transform);
		}
	}
}

template <class Kit, class BulletType>
Variant AbstractBulletsPool<Kit, BulletType>::get_bullet_property(BulletID id, String property) {
	if(is_bullet_valid(id)) {
		int32_t bullet_index = shapes_to_indices[id.index - starting_shape_index];

		return bullets[bullet_index]->get(property);
	}
	return Variant();
}