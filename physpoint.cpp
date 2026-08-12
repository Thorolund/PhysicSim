#include "physpoint.h"

PhysPoint::PhysPoint(vec prev_pos_, vec pos_, float mass_, bool stabled_) {
	stabled = stabled_;
	pos0 = pos_;
	pos_1 = prev_pos_;
	pos_2 = pos_1 * 2 - pos0;
	if (stabled) {
		if (pos0 != pos_1) {
			pos_1 = pos0;
			pos_2 = pos0;
		}
#ifndef NDEBUG
		log_message("PhysPoint::PhysPoint", "stabled point can't move",
			"definition position", "check ui");
#endif
	}
	mass = mass_;
	if (mass <= 0) {
		mass = 0.01;
#ifndef NDEBUG
		log_message("PhysPoint::PhysPoint", "can't exist 0-mass point",
			"definition mass", "check ui");
#endif
	}
}

void PhysPoint::clearAccelerations() {
	accelerations.clear();
}
