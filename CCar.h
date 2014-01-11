#ifndef CCAR_H_INCL
#define CCAR_H_INCL

#include "CPhysicsObject.h"
#include "SCarProperties.h"
#include "CObjectMesh.h"

class CCar : public CPhysicsObject
{
public:
	CCar(const SCarProperties& props, btVehicleRaycaster* raycaster, CObject* chasisObject, btCollisionShape* chasisShape, CMesh* wheelMesh);
	virtual ~CCar();

	CObject* getWheelObject(int i) { return Wheel[i]; }
	btRaycastVehicle* getVehicle() { return Vehicle; }

	float getEngineForce() { return EngineForce; }
	float getBrakeForce() { return BrakeForce; }
	float getMaxSteer() { return MaxSteer; }
	float getSteerSpeed() { return SteerSpeed; }
	float getSteer() { return Steer; }

	void setEngineForce(float ef) { EngineForce = ef; }
	void setBrakeForce(float bf) { BrakeForce = bf; }
	void setMaxSteer(float ms) { MaxSteer = ms; }
	void setSteerSpeed(float ss) { SteerSpeed = ss; }
	void setSteer(float st) { Steer = st; }

	void updateWheelTransform();

	void steerLeft(float dt);
	void steerRight(float dt);
	void steerStraight(float dt);
	void steer(float dir, float dt);

	void accelerate();
	void reverse();
	void idle();
	void brake();
	void brakeRelease();

private:
	btRaycastVehicle* Vehicle;
	CObject* Wheel[4];

	float EngineForce;
	float BrakeForce;
	float MaxSteer;
	float SteerSpeed;
	float Steer;
};

#endif