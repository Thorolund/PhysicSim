#include "ParameterizeBody.h"

void ParamBody::UpdateTotalForces() {
	totalForce.fVec = b2Vec2(0, 0);
	for (auto force_pair : permanentForces) {
		b2Vec2 force_vec = force_pair.second->fVec;
		totalForce.fVec += force_vec;
	}
	for (auto force_pair : instantForces) {
		b2Vec2 force_vec = force_pair.second->fVec;
		totalForce.fVec += force_vec;
	}
}

void ParamBody::UpdatePrevAV() {
	prevAV = body->GetAngularVelocity();
}

ParamBody::ParamBody(b2World* world_,
					 std::string name_,
					 b2BodyType type_,
					 b2Vec2 position_,
					 std::vector<b2FixtureDef*> fixtures) {
	name = name_;

	world = world_;
	b2BodyDef bodyDef;
	bodyDef.type = type_;
	bodyDef.position = position_;
	body = world->CreateBody(&bodyDef);

	for (b2FixtureDef* fixture : fixtures) {
		body->CreateFixture(fixture);
	}

	prevAV = 0;

	totalForce = Force(b2Vec2(0, 0), true, "Total");
}

ParamBody::~ParamBody() {
	world->DestroyBody(body);

	for (auto map_force : permanentForces) {
		DeleteForceByName(map_force.first);
	}
	ClearInstantForces();
}

std::string ParamBody::GetName() {
	return name;
}

bool ParamBody::isForceInPermanent(std::string force_name) {
	auto find_name = permanentForces.find(force_name);
	return (find_name != permanentForces.end());
}

bool ParamBody::isForceInInstant(std::string force_name) {
	auto find_name = instantForces.find(force_name);
	return (find_name != instantForces.end());
}

Force* ParamBody::GetForceByName(std::string force_name) {
	if (isForceInPermanent(force_name)) {
		return permanentForces[force_name];
	}
	else if (isForceInInstant(force_name)) {
		return instantForces[force_name];
	}
	else {
		return nullptr;
	}
}

Force ParamBody::GetTotalForce() {
	return totalForce;
}

std::vector<Force*> ParamBody::GetListPermanentForces() {
	std::vector<Force*> forceList;
	for (auto forcePair : permanentForces) {
		forceList.push_back(forcePair.second);
	}
	return forceList;
}

std::vector<Force*> ParamBody::GetListInstantForces() {
	std::vector<Force*> forceList;
	for (auto forcePair : instantForces) {
		forceList.push_back(forcePair.second);
	}
	return forceList;
}

void ParamBody::ApplyPermanentForces() {
	Force applyForce;
	for (auto forcePair : permanentForces) {
		applyForce = *(forcePair.second);
		b2Vec2 worldPointAttach = body->GetWorldPoint(applyForce.pointAttach);
		body->ApplyForce(applyForce.fVec, worldPointAttach, true);
	}
}

void ParamBody::AddPermanentForce(Force* force) {
	auto forceName = force->GetName();
	if (!isForceInPermanent(forceName)) {
		permanentForces[forceName] = force;
	}
}

void ParamBody::AddInstantForce(Force* force) {
	auto forceName = force->GetName();
	if (!isForceInInstant(forceName)) {
		instantForces[forceName] = force;
	}
}

void ParamBody::AddVelosity(b2Vec2 v) {
	b2Vec2 impulse = body->GetMass()*v;
	body->ApplyLinearImpulseToCenter(impulse, true);
}

void ParamBody::DeleteForceByName(std::string force_name) {
	if (isForceInPermanent(force_name)) {
		delete permanentForces[force_name];
		permanentForces[force_name] = nullptr;
		permanentForces.erase(force_name);
	}
	else if (isForceInInstant(force_name)) {
		delete instantForces[force_name];
		instantForces[force_name] = nullptr;
		instantForces.erase(force_name);
	}
}

void ParamBody::ClearInstantForces() {
	instantForces.clear();
}

void ParamBody::Update() {
	UpdateTotalForces();
	UpdatePrevAV();
}

float ParamBody::CountAngularAceleration(float dt) {
	float deltaAV = body->GetAngularVelocity() - prevAV;
	return deltaAV / dt;
}

float ParamBody::CountLinearAcceleration(float dt) {
	return (1 / body->GetMass()) * (totalForce.fVec);
}



