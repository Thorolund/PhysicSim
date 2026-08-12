#pragma once
#include "physpoint.h"

class PhysEdge {
private:
	PhysPoint* Aptr;
	PhysPoint* Bptr;
public:
	vec vecAB;
	float base_len, current_len;
	PhysEdge(PhysPoint* A_, PhysPoint* B_);
	void update();
	void shrink();
};
