#ifndef DYNAMIC_BULLET_KIT_H
#define DYNAMIC_BULLET_KIT_H

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/curve.hpp>

#include "../bullet.h"
#include "../bullet_kit.h"

using namespace godot;


// Bullet definition.
class DynamicBullet : public Bullet {
	GDCLASS(DynamicBullet, Bullet)
public:
	Transform2D starting_transform;
	float starting_speed;

	Transform2D get_transform() { return transform; }
	void set_transform(Transform2D transform) {
		starting_transform = transform;
		this->transform = transform;
	}

	Transform2D get_starting_transform() { return starting_transform; }
	void set_starting_transform(Transform2D transform) {
		starting_transform = transform;
	}

	Vector2 get_velocity() { return velocity; }
	void set_velocity(Vector2 velocity) {
		starting_speed = velocity.length();
		this->velocity = velocity;
	}

	float get_starting_speed() { return starting_speed; }
	void set_starting_speed(float speed) {
		starting_speed = speed;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_transform", "texture"), &DynamicBullet::set_transform);
		ClassDB::bind_method(D_METHOD("get_transform"), &DynamicBullet::get_transform);
		ClassDB::bind_method(D_METHOD("set_starting_transform", "transform"), &DynamicBullet::set_starting_transform);
		ClassDB::bind_method(D_METHOD("get_starting_transform"), &DynamicBullet::get_starting_transform);
		ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &DynamicBullet::set_velocity);
		ClassDB::bind_method(D_METHOD("get_velocity"), &DynamicBullet::get_velocity);
		ClassDB::bind_method(D_METHOD("set_starting_speed", "speed"), &DynamicBullet::set_starting_speed);
		ClassDB::bind_method(D_METHOD("get_starting_speed"), &DynamicBullet::get_starting_speed);

		ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "transform", PROPERTY_HINT_NONE, "",
			PROPERTY_USAGE_DEFAULT, ""), "set_transform", "get_transform");
		ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "starting_transform", PROPERTY_HINT_NONE, "",
			PROPERTY_USAGE_DEFAULT, ""), "set_starting_transform", "get_starting_transform");
		ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity", PROPERTY_HINT_NONE, "",
			PROPERTY_USAGE_DEFAULT, ""), "set_velocity", "get_velocity");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "starting_speed", PROPERTY_HINT_NONE, "",
			PROPERTY_USAGE_DEFAULT, ""), "set_starting_speed", "get_starting_speed");
	}
};

// Bullet kit definition.
class DynamicBulletKit : public BulletKit {
	GDCLASS(DynamicBulletKit, BulletKit)
public:
	BULLET_KIT(DynamicBulletKit, DynamicBulletsPool, DynamicBullet)

	Ref<Texture2D> texture;
	float lifetime_curves_span = 1.0f;
	bool lifetime_curves_loop = true;
	Ref<Curve> speed_multiplier_over_lifetime;
	Ref<Curve> rotation_offset_over_lifetime;

	Ref<Texture2D> get_texture() { return texture; }
	void set_texture(Ref<Texture2D> texture) {
		this->texture = texture;
	}

	float get_lifetime_curves_span() { return lifetime_curves_span; }
	void set_lifetime_curves_span(float lifetime_curves_span) {
		this->lifetime_curves_span = lifetime_curves_span;
	}

	bool get_lifetime_curves_loop() { return lifetime_curves_loop; }
	void set_lifetime_curves_loop(bool lifetime_curves_loop) {
		this->lifetime_curves_loop = lifetime_curves_loop;
	}

	Ref<Curve> get_speed_multiplier_over_lifetime() { return speed_multiplier_over_lifetime; }
	void set_speed_multiplier_over_lifetime(Ref<Curve> speed_multiplier_over_lifetime) {
		this->speed_multiplier_over_lifetime = speed_multiplier_over_lifetime;
	}

	Ref<Curve> get_rotation_offset_over_lifetime() { return rotation_offset_over_lifetime; }
	void set_rotation_offset_over_lifetime(Ref<Curve> rotation_offset_over_lifetime) {
		this->rotation_offset_over_lifetime = rotation_offset_over_lifetime;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_texture", "texture"), &DynamicBulletKit::set_texture);
		ClassDB::bind_method(D_METHOD("get_texture"), &DynamicBulletKit::get_texture);
		ClassDB::bind_method(D_METHOD("set_lifetime_curves_span", "lifetime_curves_span"), &DynamicBulletKit::set_lifetime_curves_span);
		ClassDB::bind_method(D_METHOD("get_lifetime_curves_span"), &DynamicBulletKit::get_lifetime_curves_span);
		ClassDB::bind_method(D_METHOD("set_lifetime_curves_loop", "lifetime_curves_loop"), &DynamicBulletKit::set_lifetime_curves_loop);
		ClassDB::bind_method(D_METHOD("get_lifetime_curves_loop"), &DynamicBulletKit::get_lifetime_curves_loop);
		ClassDB::bind_method(D_METHOD("set_speed_multiplier_over_lifetime", "speed_multiplier_over_lifetime"), &DynamicBulletKit::set_speed_multiplier_over_lifetime);
		ClassDB::bind_method(D_METHOD("get_speed_multiplier_over_lifetime"), &DynamicBulletKit::get_speed_multiplier_over_lifetime);
		ClassDB::bind_method(D_METHOD("set_rotation_offset_over_lifetime", "rotation_offset_over_lifetime"), &DynamicBulletKit::set_rotation_offset_over_lifetime);
		ClassDB::bind_method(D_METHOD("get_rotation_offset_over_lifetime"), &DynamicBulletKit::get_rotation_offset_over_lifetime);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D",
			PROPERTY_USAGE_DEFAULT, "Texture2D"), "set_texture", "get_texture");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_curves_span", PROPERTY_HINT_RANGE, "0.001,256.0", 
			PROPERTY_USAGE_DEFAULT, ""), "set_lifetime_curves_span", "get_lifetime_curves_span");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "lifetime_curves_loop", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT, ""), "set_lifetime_curves_loop", "get_lifetime_curves_loop");
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "speed_multiplier_over_lifetime", PROPERTY_HINT_RESOURCE_TYPE, "Curve", 
			PROPERTY_USAGE_DEFAULT, "Curve"), "set_speed_multiplier_over_lifetime", "get_speed_multiplier_over_lifetime");
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "rotation_offset_over_lifetime", PROPERTY_HINT_RESOURCE_TYPE, "Curve", 
			PROPERTY_USAGE_DEFAULT, "Curve"), "set_rotation_offset_over_lifetime", "get_rotation_offset_over_lifetime");
	}
};

// Bullets pool definition.
class DynamicBulletsPool : public AbstractBulletsPool<DynamicBulletKit, DynamicBullet> {

	// void _init_bullet(Bullet* bullet); Use default implementation.

	void _enable_bullet(DynamicBullet* bullet) {
		// Reset the bullet lifetime.
		bullet->lifetime = 0.0f;
		Rect2 texture_rect = Rect2(-kit->texture->get_size() / 2.0f, kit->texture->get_size());
		RID texture_rid = kit->texture->get_rid();
		
		// Configure the bullet to draw the kit texture each frame.
		RenderingServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
			texture_rect,
			texture_rid);
	}

	// void _disable_bullet(Bullet* bullet); Use default implementation.

	bool _process_bullet(DynamicBullet* bullet, float delta) {
		float adjusted_lifetime = bullet->lifetime / kit->lifetime_curves_span;
		if(kit->lifetime_curves_loop) {
			adjusted_lifetime = fmod(adjusted_lifetime, 1.0f);
		}

		if(kit->speed_multiplier_over_lifetime.is_valid()) {
			float speed_multiplier = kit->speed_multiplier_over_lifetime->sample(adjusted_lifetime);
			bullet->velocity = bullet->velocity.normalized() * bullet->starting_speed * speed_multiplier;
		}
		if(kit->rotation_offset_over_lifetime.is_valid()) {
			float rotation_offset = kit->rotation_offset_over_lifetime->sample(adjusted_lifetime);
			float absolute_rotation = bullet->starting_transform.get_rotation() + rotation_offset;

			bullet->velocity = bullet->velocity.rotated(absolute_rotation - bullet->transform.get_rotation());
		}

		bullet->transform.set_origin(bullet->transform.get_origin() + bullet->velocity * delta);

		if(!active_rect.has_point(bullet->transform.get_origin())) {
			// Return true if the bullet should be deleted.
			return true;
		}
		// Rotate the bullet based on its velocity "rotate" is enabled.
		if(kit->auto_rotate) {
			bullet->transform.set_rotation(bullet->velocity.angle());
		}
		// Bullet is still alive, increase its lifetime.
		bullet->lifetime += delta;
		// Return false if the bullet should not be deleted yet.
		return false;
	}
};

BULLET_KIT_IMPLEMENTATION(DynamicBulletKit, DynamicBulletsPool)

#endif