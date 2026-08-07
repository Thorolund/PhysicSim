#include "physpoint.h"

PhysPoint::PhysPoint(vec prev_pos_, vec pos_) {
	pos3 = pos_;
	pos2 = prev_pos_;
	pos1 = pos3 - (pos3 - pos1) * 2;
}

void PhysPoint::clearForces() {
	forces.clear();
}
