#ifndef BULLET_H
#define BULLET_H

#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/transform2d.hpp>

using namespace godot;


struct BulletID {
	int32_t index;
	int32_t cycle;
	int32_t set;

	BulletID(int32_t index, int32_t cycle, int32_t set): 
		index(index), cycle(cycle), set(set) {}
};

class Bullet : public Object {
	GDCLASS(Bullet, Object)

	RID item_rid;
	int32_t cycle = 0;
	int32_t shape_index = -1;
	Transform2D transform;
	Vector2 velocity;
	float lifetime;
	Variant data;

protected:
	static void _bind_methods();

public:
	Bullet() {}
	~Bullet() {}

	RID get_item_rid();

	int32_t get_cycle();

	int32_t get_shape_index();

	Transform2D get_transform();
	void set_transform(Transform2D transform);

	Vector2 get_velocity();
	void set_velocity(Vector2 velocity);

	float get_lifetime();
	void set_lifetime(float lifetime);

	Variant get_data();
	void set_data(Variant data);
};

#endif