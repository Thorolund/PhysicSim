#pragma once
#include <cmath>

struct vec {
	float x, y;
	vec(float x_ = 0, float y_ = 0);

	int xint();
	int yint();

	vec sum(vec other_vec);
	vec sub(vec other_vec);
	vec mlt(float a);
	vec div(float a);
	float scalmlt(vec other_vec);

	float len();
	vec len1();
	
	vec rnormal();
	vec lnormal();
};