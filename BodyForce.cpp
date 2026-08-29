#include "BodyForce.h"

BodyForce::BodyForce(ParamBody& body, Force& force) {
	bodyPtr = &body;
	forcePtr = &force;
}

Force* BodyForce::GetForce() {
	return forcePtr;
}

ParamBody* BodyForce::GetBody() {
	return bodyPtr;
}

void BodyForce::ApplyForce() {
	b2Vec2 worldPointAttach = bodyPtr->body->GetWorldPoint(forcePtr->pointAttach);
	bodyPtr->body->ApplyForce(forcePtr->fVec,
							  worldPointAttach,
							  true);
}
