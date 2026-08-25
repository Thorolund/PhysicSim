#include "physoperations.h"

vec newPointPos(PhysPoint* point) {
	vec acceleration_effect;
	for (vec a : point->accelerations) {
		acceleration_effect = acceleration_effect + a * 0.00001;
	}
	vec new_pos = point->npos + (point->npos - point->cpos + acceleration_effect);

	return new_pos;
}
