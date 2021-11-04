#include "bullets.h"
#include "kits/basic_bullet_kit.h"
#include "kits/following_bullet_kit.h"
#include "kits/dynamic_bullet_kit.h"
#include "kits/following_dynamic_bullet_kit.h"


extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	godot::Godot::nativescript_init(handle);

	godot::register_class<Bullet>();
	godot::register_class<BulletKit>();
	godot::register_class<Bullets>();

	// Default Bullet Kits.
	godot::register_class<BasicBulletKit>();

	godot::register_class<FollowingBullet>();
	godot::register_class<FollowingBulletKit>();

	godot::register_class<DynamicBullet>();
	godot::register_class<DynamicBulletKit>();

	godot::register_class<FollowingDynamicBullet>();
	godot::register_class<FollowingDynamicBulletKit>();
	
	// Custom Bullet Kits.
	//godot::register_class<CustomBulletKit>();
}