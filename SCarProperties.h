#ifndef SCAR_PROPERTIES_H_INCL
#define SCAR_PROPERTIES_H_INCL

#include "CPhysicsObject.h"
#include "CMesh.h"

struct SCarProperties
{
	SCarProperties()
	{
		FrontWheelDrive = true;
		EngineForce = 100.f;
		BrakeForce = 5.f;
		Mass = 1.f;
		WheelMesh = 0;
		ChasisObject = 0;
		ChasisShape = 0;

		SuspensionStiffness = 20.f;
		SuspensionDamping = 10.f;
		SuspensionCompression = 0.f;
		FrictionSlip = 10.f;
		RollInfluence = 0.1f;

		WheelPositionFrontLeft = vec3(-2.7f, 0.f, -4.5f);
		WheelPositionBackRight = vec3(2.7f, 0.f, 5.2f);
		WheelRestLength = 0.3f;
		WheelRadius = 1.4f;

		MaxSteer = 0.15f;
		SteerSpeed = 0.8f;
	}

	CObject* ChasisObject;
	btCollisionShape* ChasisShape;

	CMesh* WheelMesh;
	vec3 WheelPositionFrontLeft;
	vec3 WheelPositionBackRight;
	float WheelRestLength;
	float WheelRadius;

	float MaxSteer;
	float SteerSpeed;

	bool FrontWheelDrive;
	float EngineForce;
	float BrakeForce;
	float Mass;

	float SuspensionStiffness;
	float SuspensionDamping;
	float SuspensionCompression;
	float FrictionSlip;
	float RollInfluence;
};

#endif