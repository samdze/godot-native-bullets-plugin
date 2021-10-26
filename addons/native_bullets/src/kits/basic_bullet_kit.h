#ifndef BASIC_BULLET_KIT_H
#define BASIC_BULLET_KIT_H

#include <Texture.hpp>
#include <PackedScene.hpp>

#include "../bullet_kit.h"

using namespace godot;


// Bullet kit definition.
class BasicBulletKit : public BulletKit {
	GODOT_CLASS(BasicBulletKit, BulletKit)
public:
	BULLET_KIT(BasicBulletsPool)

	Ref<Texture> texture;

	static void _register_methods() {
		register_property<BasicBulletKit, Ref<Texture>>("texture", &BasicBulletKit::texture, Ref<Texture>(), 
			GODOT_METHOD_RPC_MODE_DISABLED, GODOT_PROPERTY_USAGE_DEFAULT, GODOT_PROPERTY_HINT_RESOURCE_TYPE, "Texture");
		
		BULLET_KIT_REGISTRATION(BasicBulletKit, Bullet)
	}
};

// Bullets pool definition.
class BasicBulletsPool : public AbstractBulletsPool<BasicBulletKit, Bullet> {

	// void _init_bullet(Bullet* bullet); Use default implementation.

	void _enable_bullet(Bullet* bullet) {
		// Reset the bullet lifetime.
		bullet->lifetime = 0.0f;
		Rect2 texture_rect = Rect2(-kit->texture->get_size() / 2.0f, kit->texture->get_size());
		RID texture_rid = kit->texture->get_rid();
		
		// Configure the bullet to draw the kit texture each frame.
		VisualServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
			texture_rect,
			texture_rid);
	}

	// void _disable_bullet(Bullet* bullet); Use default implementation.

	bool _process_bullet(Bullet* bullet, float delta) {
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

BULLET_KIT_IMPLEMENTATION(BasicBulletKit, BasicBulletsPool)

#endif