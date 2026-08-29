#include "Force.h"

Force::Force(b2Vec2 vec,
			 bool isLocal_,
			 std::string name_,
			 b2Vec2 pointAttach_=b2Vec2(0, 0)) {
	fVec = vec;
	isLocal = isLocal_;
	name = name_;
	pointAttach = pointAttach_;
}

std::string Force::GetName() {
	return name;
}
