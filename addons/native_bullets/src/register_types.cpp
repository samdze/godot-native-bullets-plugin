#include "register_types.h"
#include "native_bullets.h"
#include "kits/basic_bullet_kit.h"
#include "kits/dynamic_bullet_kit.h"
#include "kits/following_bullet_kit.h"
#include "kits/following_dynamic_bullet_kit.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_native_bullets_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

	ClassDB::register_class<Bullet>();
	ClassDB::register_class<BulletKit>();
	ClassDB::register_class<NativeBullets>();

	// Default Bullet Kits.
	ClassDB::register_class<BasicBulletKit>();

	ClassDB::register_class<FollowingBullet>();
	ClassDB::register_class<FollowingBulletKit>();

	ClassDB::register_class<DynamicBullet>();
	ClassDB::register_class<DynamicBulletKit>();

	ClassDB::register_class<FollowingDynamicBullet>();
	ClassDB::register_class<FollowingDynamicBulletKit>();

	// Custom Bullet Kits.
	//ClassDB::register_class<CustomBulletKit>();
}

void uninitialize_native_bullets_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
	// Initialization.
	GDExtensionBool GDE_EXPORT native_bullets_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
		godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

		init_obj.register_initializer(initialize_native_bullets_module);
		init_obj.register_terminator(uninitialize_native_bullets_module);
		init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

		return init_obj.init();
	}
}