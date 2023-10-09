#ifndef BASIC_BULLET_KIT_H
#define BASIC_BULLET_KIT_H

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "../bullet.h"
#include "../bullet_kit.h"

using namespace godot;


// Bullet kit definition.
class BasicBulletKit : public BulletKit {
	GDCLASS(BasicBulletKit, BulletKit)
public:
	BULLET_KIT(BasicBulletKit, BasicBulletsPool, Bullet)

	Ref<Texture2D> texture;

	void set_texture(Ref<Texture2D> texture) {
		this->texture = texture;
		emit_changed();
	}

	Ref<Texture2D> get_texture() {
		return texture;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_texture", "texture"), &BasicBulletKit::set_texture);
		ClassDB::bind_method(D_METHOD("get_texture"), &BasicBulletKit::get_texture);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D",
			PROPERTY_USAGE_DEFAULT, "Texture2D"), "set_texture", "get_texture");
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
		RenderingServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
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
		// Rotate the bullet based on its velocity if "auto_rotate" is enabled.
		if(kit->auto_rotate) {
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