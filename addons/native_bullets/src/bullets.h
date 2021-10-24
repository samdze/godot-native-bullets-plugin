#ifndef BULLETS_H
#define BULLETS_H

#include <Godot.hpp>
#include <Node2D.hpp>
#include <AtlasTexture.hpp>
#include <Material.hpp>
#include <Color.hpp>
#include <Array.hpp>
#include <RegEx.hpp>

#include "bullet_kit.h"
#include "bullets_pool.h"

using namespace godot;


class Bullets : public Node2D {
	GODOT_CLASS(Bullets, Node2D)
	
private:
	// A pool internal representation with related properties.
	struct PoolKit {
		BulletsPool* pool = nullptr;
		Ref<BulletKit> bullet_kit;
		int32_t size;
		int32_t z_index;
	};
	// Represents the actual internal organization of the bullet pools.
	// It may be reordered if needed compared to the inspector order.
	// E.g. to better support non-colliding bullets.
	PoolKit* pools;
	Dictionary kits_to_pools;

	int32_t bullets_collision_layer = 0;
	int32_t bullets_collision_mask = 0;
	Array bullet_kits;
	Array pools_sizes;
	Array z_indices;

	Ref<RegEx> prop_regex;
	//bool debug = false;

	int32_t available_bullets = 0;
	int32_t active_bullets = 0;
	int32_t total_bullets = 0;

	RID shared_area;
	PoolIntArray invalid_id;

	int32_t _get_pool_index(int32_t bullet_index);

public:
	static void _register_methods();

	Bullets();
	~Bullets();

	void _init();

	void _ready();
	//void _process(float delta);
	void _physics_process(float delta);
	//void _draw();

	Variant _get(String property);
	bool _set(String property, Variant value);
	Array _get_property_list();

	void spawn_bullet(Ref<BulletKit> kit, Dictionary properties);
	Variant obtain_bullet(Ref<BulletKit> kit);
	bool release_bullet(Variant id);
	bool is_bullet_valid(Variant id);

	int32_t get_available_bullets();
	int32_t get_active_bullets();

	Variant get_bullet_from_shape(int32_t shape_index);
	Ref<BulletKit> get_kit_from_bullet(Variant id);

	void set_bullet_property(Variant id, String property, Variant value);
	Variant get_bullet_property(Variant id, String property);
};

#endif