#include "vec.h"

vec::vec(float x_, float y_) {
	x = x_;
	y = y_;
}

int vec::xint() {
	return (int)x;
}

int vec::yint() {
	return (int)y;
}

vec vec::sum(vec other_vec) {
	return vec(x + other_vec.x, y + other_vec.y);
}

vec vec::sub(vec other_vec) {
	return vec(x - other_vec.x, y - other_vec.y);
}

vec vec::mlt(float a) {
	return vec(x * a, y * a);
}

vec vec::div(float a) {
	if (a == 0) {
		log_message("vec::division", "division by zero(0)", "");
		return vec(0, 0);
	}
	return vec(x/a, y/a);
}

float vec::scalmlt(vec other_vec) {
	return x*other_vec.x + y*other_vec.y;
}

float vec::len() {
	return sqrt(x*x + y*y);
}

vec vec::len1() {
	float self_len = len();
	if (self_len == 0) {
		log_message("vec::len1", "from 0 to 1", "");
		return vec(0, 0);
	}
	return div(self_len);
}

vec vec::rnormal() {
	return vec(y, -x);
}

vec vec::lnormal() {
	return vec(-y, x);
}
