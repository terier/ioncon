#ifndef SCAR_PROPERTIES_H_INCL
#define SCAR_PROPERTIES_H_INCL

#include "CPhysicsObject.h"
#include "CMesh.h"

struct SCarProperties
{
	SCarProperties()
	{
		FrontWheelDrive = true;
		EngineForce = 9.f;
		BrakeForce = 0.2f;
		Mass = 0.5f;
		WheelMesh = 0;
		ChasisObject = 0;
		ChasisShape = 0;

		SuspensionStiffness = 30.f;
		SuspensionDamping = 20.f;
		SuspensionCompression = 0.f;
		FrictionSlip = 3.5f;
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