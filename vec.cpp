#include "vec.h"

vec::vec(float x_, float y_) {
	x = x_;
	y = y_;
	self_len = sqrt(x * x + y * y);
}

float vec::getx() const {
	return x;
}

float vec::gety() const {
	return y;
}

int vec::getxint() const {
	return (int)x;
}

int vec::getyint() const {
	return (int)y;
}

vec vec::operator+(vec other_vec) const {
	return vec(x + other_vec.x, y + other_vec.y);
}

vec vec::operator-(vec other_vec) const {
	return vec(x - other_vec.x, y - other_vec.y);
}

vec vec::operator*(float a) const {
	return vec(x * a, y * a);
}

vec vec::operator/(float a) const {
	if (a == 0) {
		log_message("vec::division", "division by zero(0)", "");
		return vec(0, 0);
	}
	return vec(x/a, y/a);
}

float vec::operator*(vec other_vec) const {
	return x*other_vec.x + y*other_vec.y;
}

float vec::len() const {
	return self_len;
}

vec vec::len1() const {
	if (self_len == 0) {
		#ifndef NDEBUG
		log_message("vec::len1", "from 0 to 1", "");
		#endif
		return vec(0, 0);
	}
	return (*this) / (self_len);
}

vec vec::rnormal() const {
	if (self_len == 0) {
		#ifndef NDEBUG
		log_message("vec::rnormal", "0-vec hasn't normal", "");
		#endif
	}
	return vec(y, -x);
}

vec vec::lnormal() const {
	if (self_len == 0) {
		#ifndef NDEBUG
		log_message("vec::lnormal", "0-vec hasn't normal", "");
		#endif
	}
	return vec(-y, x);
}
