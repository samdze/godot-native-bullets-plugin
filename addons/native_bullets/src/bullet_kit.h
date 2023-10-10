#ifndef BULLET_KIT_H
#define BULLET_KIT_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/shape2d.hpp>
#include <godot_cpp/classes/material.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/script.hpp>

#include <memory>

#include "bullet.h"

#define BULLET_KIT(BulletKitType, BulletsPoolType, BulletType)					\
std::unique_ptr<BulletsPool> _create_pool() override;							\
BulletKitType() { bullet_class_name = BulletType::get_class_static(); }

#define BULLET_KIT_IMPLEMENTATION(BulletKitType, BulletsPoolType)					\
std::unique_ptr<BulletsPool> BulletKitType::_create_pool() {						\
	return std::unique_ptr<BulletsPool>(new BulletsPoolType());						\
}																										

using namespace godot;


class BulletsPool;

class BulletKit : public Resource {
	GDCLASS(BulletKit, Resource)

public:
	String bullet_class_name = Bullet::get_class_static();
	// The material used to render each bullet.
	Ref<Material> material;
	// Controls whether collisions with other objects are enabled. Turning it off increases performance.
	bool collisions_enabled = true;
	// Collisions related properties.
	int32_t collision_layer = 0;
	int32_t collision_mask = 0;
	Ref<Shape2D> collision_shape;
	// Controls whether the active rect is automatically set as the viewport visible rect.
	bool use_viewport_as_active_rect = true;
	// Controls where the bullets can live, if a bullet exits this rect, it will be removed.
	Rect2 active_rect;
	// If enabled, bullets will auto-rotate based on their direction of travel.
	bool auto_rotate = false;
	// Allows the ability to have a unique-ish value in each instance of the bullet material.
	// Can be used to offset the bullets animation by a unique amount to avoid having them animate in sync.
	int32_t unique_modulate_component = 0;
	// Additional data the user can set via the editor.
	Variant data;

	Ref<Material> get_material() { return material; }
	void set_material(Ref<Material> material) {
		this->material = material;
	}

	bool get_collisions_enabled() { return collisions_enabled; }
	void set_collisions_enabled(bool enabled) {
		collisions_enabled = enabled;
		notify_property_list_changed();
	}

	int32_t get_collision_layer() { return collision_layer; }
	void set_collision_layer(int32_t layer) {
		collision_layer = layer;
	}

	int32_t get_collision_mask() { return collision_mask; }
	void set_collision_mask(int32_t mask) {
		collision_mask = mask;
	}

	Ref<Shape2D> get_collision_shape() { return collision_shape; }
	void set_collision_shape(Ref<Shape2D> shape) {
		collision_shape = shape;
	}

	bool get_use_viewport_as_active_rect() { return use_viewport_as_active_rect; }
	void set_use_viewport_as_active_rect(bool use) {
		use_viewport_as_active_rect = use;
		notify_property_list_changed();
	}

	Rect2 get_active_rect() { return active_rect; }
	void set_active_rect(Rect2 rect) {
		active_rect = rect;
	}

	bool get_auto_rotate() { return auto_rotate; }
	void set_auto_rotate(bool rotate) {
		auto_rotate = rotate;
	}

	int32_t get_unique_modulate_component() { return unique_modulate_component; }
	void set_unique_modulate_component(int32_t component) {
		unique_modulate_component = component;
	}

	Variant get_data() { return data; }
	void set_data(Variant data) {
		this->data = data;
	}
	
	String get_bullet_class_name() { return bullet_class_name; }
	String get_bullet_properties() { return bullet_class_name; }

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_material", "material"), &BulletKit::set_material);
		ClassDB::bind_method(D_METHOD("get_material"), &BulletKit::get_material);
		ClassDB::bind_method(D_METHOD("set_collisions_enabled", "enabled"), &BulletKit::set_collisions_enabled);
		ClassDB::bind_method(D_METHOD("get_collisions_enabled"), &BulletKit::get_collisions_enabled);
		ClassDB::bind_method(D_METHOD("set_collision_layer", "layer"), &BulletKit::set_collision_layer);
		ClassDB::bind_method(D_METHOD("get_collision_layer"), &BulletKit::get_collision_layer);
		ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &BulletKit::set_collision_mask);
		ClassDB::bind_method(D_METHOD("get_collision_mask"), &BulletKit::get_collision_mask);
		ClassDB::bind_method(D_METHOD("set_collision_shape", "shape"), &BulletKit::set_collision_shape);
		ClassDB::bind_method(D_METHOD("get_collision_shape"), &BulletKit::get_collision_shape);
		ClassDB::bind_method(D_METHOD("set_use_viewport_as_active_rect", "use"), &BulletKit::set_use_viewport_as_active_rect);
		ClassDB::bind_method(D_METHOD("get_use_viewport_as_active_rect"), &BulletKit::get_use_viewport_as_active_rect);
		ClassDB::bind_method(D_METHOD("set_active_rect", "rect"), &BulletKit::set_active_rect);
		ClassDB::bind_method(D_METHOD("get_active_rect"), &BulletKit::get_active_rect);
		ClassDB::bind_method(D_METHOD("set_auto_rotate", "rotate"), &BulletKit::set_auto_rotate);
		ClassDB::bind_method(D_METHOD("get_auto_rotate"), &BulletKit::get_auto_rotate);
		ClassDB::bind_method(D_METHOD("set_unique_modulate_component", "component"), &BulletKit::set_unique_modulate_component);
		ClassDB::bind_method(D_METHOD("get_unique_modulate_component"), &BulletKit::get_unique_modulate_component);
		ClassDB::bind_method(D_METHOD("set_data", "data"), &BulletKit::set_data);
		ClassDB::bind_method(D_METHOD("get_data"), &BulletKit::get_data);

		ClassDB::bind_method(D_METHOD("get_bullet_class_name"), &BulletKit::get_bullet_class_name);
		ClassDB::bind_method(D_METHOD("get_bullet_properties"), &BulletKit::get_bullet_properties);
		
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "material", PROPERTY_HINT_RESOURCE_TYPE, "Material",
			PROPERTY_USAGE_DEFAULT, "Material"), "set_material", "get_material");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "collisions_enabled", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT, "bool"), "set_collisions_enabled", "get_collisions_enabled");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_layer", PROPERTY_HINT_LAYERS_2D_PHYSICS, "", 
			PROPERTY_USAGE_DEFAULT, "int"), "set_collision_layer", "get_collision_layer");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS, "", 
			PROPERTY_USAGE_DEFAULT, "int"), "set_collision_mask", "get_collision_mask");
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collision_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D", 
			PROPERTY_USAGE_DEFAULT, "Shape2D"), "set_collision_shape", "get_collision_shape");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_viewport_as_active_rect", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT, "bool"), "set_use_viewport_as_active_rect", "get_use_viewport_as_active_rect");
		ADD_PROPERTY(PropertyInfo(Variant::RECT2, "active_rect", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT, "Rect2"), "set_active_rect", "get_active_rect");
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "auto_rotate", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT, "bool"), "set_auto_rotate", "get_auto_rotate");
		ADD_PROPERTY(PropertyInfo(Variant::INT, "unique_modulate_component", PROPERTY_HINT_ENUM, "None,Red,Green,Blue,Alpha", 
			PROPERTY_USAGE_DEFAULT, "int"), "set_unique_modulate_component", "get_unique_modulate_component");
		ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "data", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_DEFAULT, "Dictionary"), "set_data", "get_data");

		ADD_PROPERTY(PropertyInfo(Variant::NIL, "Bullet", PROPERTY_HINT_NONE, "Bullet", 
			PROPERTY_USAGE_CATEGORY, "Bullet"), "", "get_bullet_properties");
		// This property returns the class name of the used Bullet.
		// It's also a property used to allow the inspector to show custom editor controls.
		ADD_PROPERTY(PropertyInfo(Variant::STRING, "bullet_class_name", PROPERTY_HINT_NONE, "", 
			PROPERTY_USAGE_EDITOR, ""), "", "get_bullet_class_name");
	}

	virtual bool is_valid() { return material.is_valid(); }

	virtual std::unique_ptr<BulletsPool> _create_pool() { return std::unique_ptr<BulletsPool>(); }
};

#include "bullets_pool.h"

#endif