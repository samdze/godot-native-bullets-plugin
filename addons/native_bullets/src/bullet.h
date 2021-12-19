#ifndef BULLET_H
#define BULLET_H

#include <Godot.hpp>
#include <Transform2D.hpp>

using namespace godot;


struct BulletID {
	int32_t index;
	int32_t cycle;
	int32_t set;

	BulletID(int32_t index, int32_t cycle, int32_t set): 
		index(index), cycle(cycle), set(set) {}
};

class Bullet : public Object {
	GODOT_CLASS(Bullet, Object)

public:
	RID item_rid;
	int32_t cycle = 0;
	int32_t shape_index = -1;
	Transform2D transform;
	Vector2 velocity;
	float lifetime;
	Variant data;

	void _init() {}

	RID get_item_rid() { return item_rid; }
	void set_item_rid(RID value) { ERR_PRINT("Can't edit the item rid of bullets!"); }

	int32_t get_cycle() { return cycle; }
	void set_cycle(int32_t value) { ERR_PRINT("Can't edit the cycle of bullets!"); }

	int32_t get_shape_index() { return shape_index; }
	void set_shape_index(int32_t value) { ERR_PRINT("Can't edit the shape index of bullets!"); }

	static void _register_methods() {
		register_property<Bullet, RID>("item_rid", &Bullet::set_item_rid, &Bullet::get_item_rid, RID());
		register_property<Bullet, int32_t>("cycle", &Bullet::set_cycle, &Bullet::get_cycle, 0);
		register_property<Bullet, int32_t>("shape_index", &Bullet::set_shape_index, &Bullet::get_shape_index, 0);

		register_property<Bullet, Transform2D>("transform", &Bullet::transform, Transform2D());
		register_property<Bullet, Vector2>("velocity", &Bullet::velocity, Vector2());
		register_property<Bullet, float>("lifetime", &Bullet::lifetime, 0.0f);
		register_property<Bullet, Variant>("data", &Bullet::data, Variant());
	}
};

#endif