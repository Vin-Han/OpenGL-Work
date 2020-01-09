#include "SnowSystem.h"

SnowSystem::SnowSystem(Vector3 position, Vector3 velocity,
	float lifelength, Vector3 scale) {
	shape = Mesh::GenerateQuad();
	this->originPosition = position;
	this->position = position;
	this->velocity = velocity;
	this->lifelength = lifelength;
	this->scale = scale;
	elapsedTime = 0;
}


void SnowSystem::Update(float mesc) {
	position -= velocity * mesc / 1000;
	elapsedTime += mesc;
	if (elapsedTime > lifelength){
		elapsedTime = 0;
		position = originPosition;
	}
}


void SnowSystem::DrawA() {
	shape->Draw();
}
