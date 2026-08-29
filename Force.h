#pragma once
#include <box2d.h>
#include <string>

class Force {
private:
	std::string name;
public:
	bool isLocal = false;
	b2Vec2 fVec;
	b2Vec2 pointAttach;
	Force(b2Vec2 vec,
		bool isLocal_,
		std::string name_,
		b2Vec2 pointAttach_ = b2Vec2(0, 0));
	std::string GetName();
};
