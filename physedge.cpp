#include "physedge.h"

PhysEdge::PhysEdge(PhysPoint* A_, PhysPoint* B_) {
	Aptr = A_;
	Bptr = B_;
	vecAB = (Bptr->pos0) - (Aptr->pos0);
	base_len = vecAB.len();
	current_len = base_len;
}

void PhysEdge::update() {
	vecAB = (Bptr->pos0) - (Aptr->pos0);
	current_len = vecAB.len();
}

void PhysEdge::shrink() {
	if (base_len == 0) {
		return;
	}

	float diff_len = base_len - current_len;
	if (diff_len == 0) {
		return;
	}

	float total_mass = Aptr->mass + Bptr->mass;
	float k_a_shrink;
	float k_b_shrink;
	if ((!Aptr->stabled) && (!Bptr->stabled)) {
		k_a_shrink = (Bptr->mass) / total_mass;
		k_b_shrink = (Aptr->mass) / total_mass;
	} else if (Aptr->stabled) {
		k_a_shrink = 0;
		k_b_shrink = 1;
	} else if (Bptr->stabled) {
		k_a_shrink = 1;
		k_b_shrink = 0;
	}

	vec dA = vecAB.len1() * diff_len * k_a_shrink;
	vec dB = vecAB.len1() * diff_len * k_b_shrink;

	Aptr->pos0 = Aptr->pos0 - dA;
	Bptr->pos0 = Bptr->pos0 + dB;
}
