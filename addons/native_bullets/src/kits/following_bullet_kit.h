#ifndef FOLLOWING_BULLET_KIT_H
#define FOLLOWING_BULLET_KIT_H

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cmath>

#include "../bullet_kit.h"

using namespace godot;


// Bullet definition.
class FollowingBullet : public Bullet {
	GDCLASS(FollowingBullet, Bullet)
public:
	Node2D* target_node = nullptr;

	Node2D* get_target_node() { return target_node; }
	void set_target_node(Node2D* node) {
		target_node = node;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_target_node", "node"), &FollowingBullet::set_target_node);
		ClassDB::bind_method(D_METHOD("get_target_node"), &FollowingBullet::get_target_node);

		// Registering an Object reference property with GODOT_PROPERTY_HINT_NODE_TYPE and hint_string is just
		// a way to tell the editor plugin the type of the property, so that it can be viewed in the BulletKit inspector.
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_node", PROPERTY_HINT_NODE_TYPE, "Node2D",
			PROPERTY_USAGE_NO_INSTANCE_STATE, "Node2D"), "set_target_node", "get_target_node");
	}
};

// Bullet kit definition.
class FollowingBulletKit : public BulletKit {
	GDCLASS(FollowingBulletKit, BulletKit)
public:
	BULLET_KIT(FollowingBulletKit, FollowingBulletsPool, FollowingBullet)

	Ref<Texture2D> texture;
	float bullets_turning_speed = 1.0f;

	Ref<Texture2D> get_texture() { return texture; }
	void set_texture(Ref<Texture2D> texture) {
		this->texture = texture;
	}
	
	float get_bullets_turning_speed() { return bullets_turning_speed; }
	void set_bullets_turning_speed(float speed) {
		bullets_turning_speed = speed;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_texture", "texture"), &FollowingBulletKit::set_texture);
		ClassDB::bind_method(D_METHOD("get_texture"), &FollowingBulletKit::get_texture);
		ClassDB::bind_method(D_METHOD("set_bullets_turning_speed", "speed"), &FollowingBulletKit::set_bullets_turning_speed);
		ClassDB::bind_method(D_METHOD("get_bullets_turning_speed"), &FollowingBulletKit::get_bullets_turning_speed);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D",
			PROPERTY_USAGE_DEFAULT, "Texture2D"), "set_texture", "get_texture");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bullets_turning_speed", PROPERTY_HINT_RANGE, "0.0,128.0",
			PROPERTY_USAGE_DEFAULT, ""), "set_bullets_turning_speed", "get_bullets_turning_speed");
	}
};

// Bullets pool definition.
class FollowingBulletsPool : public AbstractBulletsPool<FollowingBulletKit, FollowingBullet> {

	//void _init_bullet(FollowingBullet* bullet); Use default implementation.

	void _enable_bullet(FollowingBullet* bullet) {
		// Reset the bullet lifetime.
		bullet->lifetime = 0.0f;
		Rect2 texture_rect = Rect2(-kit->texture->get_size() / 2.0f, kit->texture->get_size());
		RID texture_rid = kit->texture->get_rid();
		
		// Configure the bullet to draw the kit texture each frame.
		RenderingServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
			texture_rect,
			texture_rid);
	}

	//void _disable_bullet(FollowingBullet* bullet); Use default implementation.

	bool _process_bullet(FollowingBullet* bullet, float delta) {
		if(!UtilityFunctions::is_instance_valid(bullet->target_node)) {
			bullet->set_target_node(nullptr);
		}
		if(bullet->target_node != nullptr) {
			// Find the rotation to the target node.
			Vector2 to_target = bullet->target_node->get_global_position() - bullet->transform.get_origin();
			float rotation_to_target = bullet->velocity.angle_to(to_target);
			float rotation_value = Math::min(kit->bullets_turning_speed * delta, std::abs(rotation_to_target));

			// Apply the rotation, capped to the max turning speed.
			bullet->velocity = bullet->velocity.rotated(Math::sign(rotation_to_target) * rotation_value);
		}
		// Apply velocity.
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

BULLET_KIT_IMPLEMENTATION(FollowingBulletKit, FollowingBulletsPool)

#endif