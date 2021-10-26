#ifndef FOLLOWING_BULLET_KIT_H
#define FOLLOWING_BULLET_KIT_H

#include <Texture.hpp>
#include <PackedScene.hpp>
#include <Node2D.hpp>
#include <SceneTree.hpp>
#include <cmath>

#include "../bullet_kit.h"

using namespace godot;


// Bullet definition.
class FollowingBullet : public Bullet {
	GODOT_CLASS(FollowingBullet, Bullet)
public:
	Node2D* target_node = nullptr;

	void _init() {}

	void set_target_node(Node2D* node) {
		target_node = node;
	}

	Node2D* get_target_node() {
		return target_node;
	}

	static void _register_methods() {
		// Registering an Object reference property with GODOT_PROPERTY_HINT_RESOURCE_TYPE and hint_string is just
		// a way to tell the editor plugin the type of the property, so that it can be viewed in the BulletKit inspector.
		register_property<FollowingBullet, Node2D*>("target_node",
			&FollowingBullet::set_target_node,
			&FollowingBullet::get_target_node, nullptr,
			GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_NO_INSTANCE_STATE, GODOT_PROPERTY_HINT_RESOURCE_TYPE, "Node2D");
	}
};

// Bullet kit definition.
class FollowingBulletKit : public BulletKit {
	GODOT_CLASS(FollowingBulletKit, BulletKit)
public:
	BULLET_KIT(FollowingBulletsPool)

	Ref<Texture> texture;
	float bullets_turning_speed = 1.0f;

	static void _register_methods() {
		register_property<FollowingBulletKit, Ref<Texture>>("texture", &FollowingBulletKit::texture, Ref<Texture>(), 
			GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RESOURCE_TYPE, "Texture");
		register_property<FollowingBulletKit, float>("bullets_turning_speed", &FollowingBulletKit::bullets_turning_speed, 1.0f, 
			GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RANGE, "0.0,128.0");
		
		BULLET_KIT_REGISTRATION(FollowingBulletKit, FollowingBullet)
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
		VisualServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
			texture_rect,
			texture_rid);
	}

	//void _disable_bullet(FollowingBullet* bullet); Use default implementation.

	bool _process_bullet(FollowingBullet* bullet, float delta) {
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
		if(kit->rotate) {
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