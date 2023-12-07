#include <godot_cpp/core/class_db.hpp>

#include "bullet.h"


RID Bullet::get_item_rid() { return item_rid; }

int32_t Bullet::get_cycle() { return cycle; }

int32_t Bullet::get_shape_index() { return shape_index; }

Transform2D Bullet::get_transform() { return transform; }
void Bullet::set_transform(Transform2D p_transform) {
	transform = p_transform;
}

Vector2 Bullet::get_velocity() { return velocity; }
void Bullet::set_velocity(Vector2 velocity) {
	this->velocity = velocity;
}

float Bullet::get_lifetime() { return lifetime; }
void Bullet::set_lifetime(float lifetime) {
	this->lifetime = lifetime;
}

Variant Bullet::get_data() { return data; }
void Bullet::set_data(Variant data) {
	this->data = data;
}

void Bullet::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_item_rid"), &Bullet::get_item_rid);
	ClassDB::bind_method(D_METHOD("get_cycle"), &Bullet::get_cycle);
	ClassDB::bind_method(D_METHOD("get_shape_index"), &Bullet::get_shape_index);
	ClassDB::bind_method(D_METHOD("get_transform"), &Bullet::get_transform);
	ClassDB::bind_method(D_METHOD("set_transform", "value"), &Bullet::set_transform);
	ClassDB::bind_method(D_METHOD("get_velocity"), &Bullet::get_velocity);
	ClassDB::bind_method(D_METHOD("set_velocity", "value"), &Bullet::set_velocity);
	ClassDB::bind_method(D_METHOD("get_lifetime"), &Bullet::get_lifetime);
	ClassDB::bind_method(D_METHOD("set_lifetime", "value"), &Bullet::set_lifetime);
	ClassDB::bind_method(D_METHOD("get_data"), &Bullet::get_data);
	ClassDB::bind_method(D_METHOD("set_data", "value"), &Bullet::set_data);

	ADD_PROPERTY(PropertyInfo(Variant::RID, "item_rid", PROPERTY_HINT_NONE, "",
		PROPERTY_USAGE_DEFAULT, ""), "", "get_item_rid");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "cycle", PROPERTY_HINT_NONE, "", 
		PROPERTY_USAGE_DEFAULT, ""), "", "get_cycle");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape_index", PROPERTY_HINT_NONE, "", 
		PROPERTY_USAGE_DEFAULT, ""), "", "get_shape_index");
	ADD_PROPERTY(PropertyInfo(Variant::TRANSFORM2D, "transform", PROPERTY_HINT_NONE, "", 
		PROPERTY_USAGE_DEFAULT, ""), "set_transform", "get_transform");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity", PROPERTY_HINT_NONE, "", 
		PROPERTY_USAGE_DEFAULT, ""), "set_velocity", "get_velocity");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "lifetime", PROPERTY_HINT_NONE, "", 
		PROPERTY_USAGE_DEFAULT, ""), "set_lifetime", "get_lifetime");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_NONE, "", 
		PROPERTY_USAGE_DEFAULT, ""), "set_data", "get_data");
}