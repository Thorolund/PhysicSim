#pragma once
#include "vec.h"
#include "logger.h"
#include <vector>

struct PhysPoint {
public:
	vec pos1, pos2, pos3;
	std::vector<vec> forces;
	PhysPoint(vec prev_pos_, vec pos_);
	void clearForces();
};
