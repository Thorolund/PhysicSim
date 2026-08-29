#include "ParameterizeBody.h"

ParamBody::ParamBody(b2World* world_,
					 b2BodyType type,
					 b2Vec2 position,
					 std::string name_) {
	world = world_;

	b2BodyDef bodyDef;
	bodyDef.type = type;
	bodyDef.position = position;

	body = world->CreateBody(&bodyDef);

	prevLV = b2Vec2(0, 0);
	prevAV = 0;
}

ParamBody::~ParamBody() {
	world->DestroyBody(body);
}

std::string ParamBody::GetName() {
	return name;
}

void ParamBody::AddFixture(b2FixtureDef* fixtDef) {
	body->CreateFixture(fixtDef);
}

b2Vec2 ParamBody::CountLinearAcceleration(float timestep) {
	b2Vec2 deltaVelosity = body->GetLinearVelocity() - prevLV;
	return b2Vec2(deltaVelosity.x/timestep, deltaVelosity.y/timestep);
}

float ParamBody::CountAngleAcceleration(float timestep) {
	float deltaAngleVelosity = body->GetAngularVelocity() - prevAV;
	return deltaAngleVelosity / timestep;
}

void ParamBody::UpdatePrevVelocites() {
	prevLV = body->GetLinearVelocity();
	prevAV = body->GetAngularVelocity();
}
