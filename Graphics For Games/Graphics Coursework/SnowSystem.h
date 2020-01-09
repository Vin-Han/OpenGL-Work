#pragma once

#include"../../nclgl/Vector3.h"
#include"../../nclgl/Vector4.h"
#include"../../nclgl/common.h"
#include"../../nclgl/GameTimer.h"
#include"../../nclgl/Mesh.h"
#include"../../nclgl/HeightMap.h"
#include <CMath>

class SnowSystem{
public:
	Mesh* shape;
	SnowSystem() { }
	SnowSystem(Vector3 position, Vector3 velocity, float lifelength, Vector3 scale);
	~SnowSystem(){}

	void Update(float mesc);
	void DrawA();

	float GetLifelength() { return lifelength; }
	void SetLifelength(float lifelength) { this->lifelength = lifelength; }

	Vector3 GetOriginPosition() { return originPosition; }
	void SetOriginPosition(Vector3 originPosition) { this->originPosition = originPosition; }

	Vector3 GetVelocity() { return velocity; }
	void SetVelocity(Vector3 velocity) { this->velocity = velocity; }

	Vector3 GetScale() { return scale; }
	void SetScale(Vector3 scale) { this->scale = scale; }

	Vector3 GetPosition() {return position;}
	void SetPosition(Vector3 position) { this->position = position; }


protected:
	Vector3 originPosition;
	Vector3 position;
	Vector3 velocity;
	Vector3 scale;
	float lifelength;
	float elapsedTime;
	
};

