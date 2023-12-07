#ifndef FOLLOWING_DYNAMIC_BULLET_KIT_H
#define FOLLOWING_DYNAMIC_BULLET_KIT_H

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../bullet_kit.h"

using namespace godot;


// Bullet definition.
class FollowingDynamicBullet : public Bullet {
	GDCLASS(FollowingDynamicBullet, Bullet)
public:
	Node2D* target_node = nullptr;

	float starting_speed;

	Node2D* get_target_node() { return target_node; }
	void set_target_node(Node2D* node) {
		target_node = node;
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
		ClassDB::bind_method(D_METHOD("set_target_node", "node"), &FollowingDynamicBullet::set_target_node);
		ClassDB::bind_method(D_METHOD("get_target_node"), &FollowingDynamicBullet::get_target_node);
		ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &FollowingDynamicBullet::set_velocity);
		ClassDB::bind_method(D_METHOD("get_velocity"), &FollowingDynamicBullet::get_velocity);
		ClassDB::bind_method(D_METHOD("set_starting_speed", "speed"), &FollowingDynamicBullet::set_starting_speed);
		ClassDB::bind_method(D_METHOD("get_starting_speed"), &FollowingDynamicBullet::get_starting_speed);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_node", PROPERTY_HINT_NODE_TYPE, "Node2D",
			PROPERTY_USAGE_DEFAULT), "set_target_node", "get_target_node");
		ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT), "set_velocity", "get_velocity");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "starting_speed", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT), "set_starting_speed", "get_starting_speed");
	}
};

// Bullet kit definition.
class FollowingDynamicBulletKit : public BulletKit {
	GDCLASS(FollowingDynamicBulletKit, BulletKit)
public:
	BULLET_KIT(FollowingDynamicBulletKit, FollowingDynamicBulletsPool, FollowingDynamicBullet)

	Ref<Texture2D> texture;
	float lifetime_curves_span = 1.0f;
	float distance_curves_span = 128.0f;
	bool lifetime_curves_loop = true;
	int32_t speed_control_mode = 0;
	Ref<Curve> speed_multiplier;
	int32_t turning_speed_control_mode = 0;
	Ref<Curve> turning_speed;

	Ref<Texture2D> get_texture() { return texture; }
	void set_texture(Ref<Texture2D> texture) {
		this->texture = texture;
	}

	float get_lifetime_curves_span() { return lifetime_curves_span; }
	void set_lifetime_curves_span(float span) {
		lifetime_curves_span = span;
	}

	float get_distance_curves_span() { return distance_curves_span; }
	void set_distance_curves_span(float span) {
		distance_curves_span = span;
	}

	bool get_lifetime_curves_loop() { return lifetime_curves_loop; }
	void set_lifetime_curves_loop(bool loop) {
		lifetime_curves_loop = loop;
	}

	int32_t get_speed_control_mode() { return speed_control_mode; }
	void set_speed_control_mode(int32_t mode) {
		speed_control_mode = mode;
	}

	Ref<Curve> get_speed_multiplier() { return speed_multiplier; }
	void set_speed_multiplier(Ref<Curve> curve) {
		speed_multiplier = curve;
	}

	int32_t get_turning_speed_control_mode() { return turning_speed_control_mode; }
	void set_turning_speed_control_mode(int32_t mode) {
		turning_speed_control_mode = mode;
	}

	Ref<Curve> get_turning_speed() { return turning_speed; }
	void set_turning_speed(Ref<Curve> curve) {
		turning_speed = curve;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_texture", "texture"), &FollowingDynamicBulletKit::set_texture);
		ClassDB::bind_method(D_METHOD("get_texture"), &FollowingDynamicBulletKit::get_texture);
		ClassDB::bind_method(D_METHOD("set_lifetime_curves_span", "span"), &FollowingDynamicBulletKit::set_lifetime_curves_span);
		ClassDB::bind_method(D_METHOD("get_lifetime_curves_span"), &FollowingDynamicBulletKit::get_lifetime_curves_span);
		ClassDB::bind_method(D_METHOD("set_distance_curves_span", "span"), &FollowingDynamicBulletKit::set_distance_curves_span);
		ClassDB::bind_method(D_METHOD("get_distance_curves_span"), &FollowingDynamicBulletKit::get_distance_curves_span);
		ClassDB::bind_method(D_METHOD("set_lifetime_curves_loop", "loop"), &FollowingDynamicBulletKit::set_lifetime_curves_loop);
		ClassDB::bind_method(D_METHOD("get_lifetime_curves_loop"), &FollowingDynamicBulletKit::get_lifetime_curves_loop);
		ClassDB::bind_method(D_METHOD("set_speed_control_mode", "mode"), &FollowingDynamicBulletKit::set_speed_control_mode);
		ClassDB::bind_method(D_METHOD("get_speed_control_mode"), &FollowingDynamicBulletKit::get_speed_control_mode);
		ClassDB::bind_method(D_METHOD("set_speed_multiplier", "curve"), &FollowingDynamicBulletKit::set_speed_multiplier);
		ClassDB::bind_method(D_METHOD("get_speed_multiplier"), &FollowingDynamicBulletKit::get_speed_multiplier);
		ClassDB::bind_method(D_METHOD("set_turning_speed_control_mode", "mode"), &FollowingDynamicBulletKit::set_turning_speed_control_mode);
		ClassDB::bind_method(D_METHOD("get_turning_speed_control_mode"), &FollowingDynamicBulletKit::get_turning_speed_control_mode);
		ClassDB::bind_method(D_METHOD("set_turning_speed", "curve"), &FollowingDynamicBulletKit::set_turning_speed);
		ClassDB::bind_method(D_METHOD("get_turning_speed"), &FollowingDynamicBulletKit::get_turning_speed);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D",
			PROPERTY_USAGE_DEFAULT, "Texture2D"), "set_texture", "get_texture");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime_curves_span", PROPERTY_HINT_RANGE, "0.001,256.0", 
			PROPERTY_USAGE_DEFAULT, ""), "set_lifetime_curves_span", "get_lifetime_curves_span");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance_curves_span", PROPERTY_HINT_RANGE, "0.001,65536.0", 
			PROPERTY_USAGE_DEFAULT, ""), "set_distance_curves_span", "get_distance_curves_span");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "lifetime_curves_loop", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT, ""), "set_lifetime_curves_loop", "get_lifetime_curves_loop");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "speed_control_mode", PROPERTY_HINT_ENUM, 
			"Based On Lifetime,Based On Target Distance,Based On Angle To Target", 
			PROPERTY_USAGE_DEFAULT, ""), "set_speed_control_mode", "get_speed_control_mode");
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "speed_multiplier", PROPERTY_HINT_RESOURCE_TYPE, "Curve", 
			PROPERTY_USAGE_DEFAULT, "Curve"), "set_speed_multiplier", "get_speed_multiplier");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "turning_speed_control_mode", PROPERTY_HINT_ENUM, 
			"Based On Lifetime,Based On Target Distance,Based On Angle To Target", 
			PROPERTY_USAGE_DEFAULT, ""), "set_turning_speed_control_mode", "get_turning_speed_control_mode");
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "turning_speed", PROPERTY_HINT_RESOURCE_TYPE, "Curve", 
			PROPERTY_USAGE_DEFAULT, "Curve"), "set_turning_speed", "get_turning_speed");
		
		ClassDB::bind_integer_constant(get_class_static(), "ControlMode", "BASED_ON_LIFETIME", 0);
		ClassDB::bind_integer_constant(get_class_static(), "ControlMode", "BASED_ON_TARGET_DISTANCE", 1);
		ClassDB::bind_integer_constant(get_class_static(), "ControlMode", "BASED_ON_ANGLE_TO_TARGET", 2);
	}
};

// Bullets pool definition.
class FollowingDynamicBulletsPool : public AbstractBulletsPool<FollowingDynamicBulletKit, FollowingDynamicBullet> {

	// void _init_bullet(FollowingDynamicBullet* bullet); Use default implementation.

	void _enable_bullet(FollowingDynamicBullet* bullet) {
		// Reset the bullet lifetime.
		bullet->lifetime = 0.0f;
		Rect2 texture_rect = Rect2(-kit->texture->get_size() / 2.0f, kit->texture->get_size());
		RID texture_rid = kit->texture->get_rid();
		
		// Configure the bullet to draw the kit texture each frame.
		RenderingServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
			texture_rect,
			texture_rid);
	}

	// void _disable_bullet(FollowingDynamicBullet* bullet); Use default implementation.

	bool _process_bullet(FollowingDynamicBullet* bullet, float delta) {
		float adjusted_lifetime = bullet->lifetime / kit->lifetime_curves_span;
		if(kit->lifetime_curves_loop) {
			adjusted_lifetime = fmod(adjusted_lifetime, 1.0f);
		}
		float bullet_turning_speed = 0.0f;
		float speed_multiplier = 1.0f;
		
		if(!UtilityFunctions::is_instance_valid(bullet->target_node)) {
			bullet->set_target_node(nullptr);
		}
		if(kit->turning_speed.is_valid() && bullet->target_node != nullptr) {
			Vector2 to_target = bullet->target_node->get_global_position() - bullet->transform.get_origin();
			// If based on lifetime.
			if(kit->turning_speed_control_mode == 0) {
				bullet_turning_speed = kit->turning_speed->sample(adjusted_lifetime);
			}
			// If based on distance to target.
			else if(kit->turning_speed_control_mode == 1) {
				float distance_to_target = to_target.length();
				bullet_turning_speed = kit->turning_speed->sample(distance_to_target / kit->distance_curves_span);
			}
			// If based on angle to target.
			else if(kit->turning_speed_control_mode == 2) {
				float angle_to_target = bullet->velocity.angle_to(to_target);
				bullet_turning_speed = kit->turning_speed->sample(std::abs(angle_to_target) / (float)Math_PI);
			}
		}
		if(kit->speed_multiplier.is_valid()) {
			// If based on lifetime.
			if(kit->speed_control_mode <= 0) {
				speed_multiplier = kit->speed_multiplier->sample(adjusted_lifetime);
			}
			// If based on target node: 1 or 2.
			else if(kit->speed_control_mode < 3 && bullet->target_node != nullptr) {
				Vector2 to_target = bullet->target_node->get_global_position() - bullet->transform.get_origin();
				// If based on distance to target.
				if(kit->speed_control_mode == 1) {
					float distance_to_target = to_target.length();
					speed_multiplier = kit->speed_multiplier->sample(distance_to_target / kit->distance_curves_span);
				}
				// If based on angle to target.
				else if(kit->speed_control_mode == 2) {
					float angle_to_target = bullet->velocity.angle_to(to_target);
					speed_multiplier = kit->speed_multiplier->sample(std::abs(angle_to_target) / (float)Math_PI);
				}
			}
		}

		if(speed_multiplier != 1.0f) {
			bullet->velocity = bullet->velocity.normalized() * bullet->starting_speed * speed_multiplier;
		}
		if(bullet_turning_speed != 0.0 && bullet->target_node != nullptr) {
			// Find the rotation to the target node.
			Vector2 to_target = bullet->target_node->get_global_position() - bullet->transform.get_origin();
			float rotation_to_target = bullet->velocity.angle_to(to_target);
			float rotation_value = Math::min(bullet_turning_speed * delta, std::abs(rotation_to_target));
			// Apply the rotation, capped to the max turning speed.
			bullet->velocity = bullet->velocity.rotated(Math::sign(rotation_to_target) * rotation_value);
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

BULLET_KIT_IMPLEMENTATION(FollowingDynamicBulletKit, FollowingDynamicBulletsPool)

#endif