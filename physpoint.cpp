#include "physpoint.h"

PhysPoint::PhysPoint(vec prev_pos_, vec pos_, float mass_, bool stabled_) {
	stabled = stabled_;
	npos = pos_;
	cpos = prev_pos_;
	ppos = cpos * 2 - npos;
	if (stabled) {
		if (npos != cpos) {
			cpos = npos;
			ppos = npos;
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

void PhysPoint::clear_accelerations() {
	accelerations.clear();
}

void PhysPoint::set_new_pos(vec new_pos) {
	if (!stabled) {
		ppos = cpos;
		cpos = npos;
		npos = new_pos;
	}
	else {
#ifndef NDEBUG
		log_message("PhysPoint::set_new_pos", "can't change Verle position",
			"", "");
#endif
	}
}

void PhysPoint::move_to(vec new_pos) {
	vec delta = new_pos - cpos;
	cpos = cpos + delta;
	ppos = ppos + delta;
	npos = npos + delta;
}
