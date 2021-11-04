#include "bullets_pool.h"

using namespace godot;


BulletsPool::BulletsPool() {}

BulletsPool::~BulletsPool() {}

int32_t BulletsPool::get_available_bullets() {
	return available_bullets;
}

int32_t BulletsPool::get_active_bullets() {
	return active_bullets;
}