#pragma once
#include "vec.h"
#include "logger.h"
#include <vector>

struct PhysPoint {
	vec pos_2, pos_1, pos0;
	float mass;
	bool stabled = false;
	std::vector<vec> accelerations;
	PhysPoint(vec prev_pos_, vec pos_, float mass_, bool stabled_);
	void clearAccelerations();
};
