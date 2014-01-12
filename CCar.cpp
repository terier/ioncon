#include "CCar.h"

CCar::CCar(const SCarProperties& props, btVehicleRaycaster* raycaster, CObject* chasisObject, btCollisionShape* chasisShape, CMesh* wheelMesh) :
CPhysicsObject(chasisObject, chasisShape, props.Mass),
	EngineForce(props.EngineForce), BrakeForce(props.BrakeForce),
	MaxSteer(props.MaxSteer), SteerSpeed(props.SteerSpeed), Steer(0)
{
	btRaycastVehicle::btVehicleTuning tuning;
	tuning.m_suspensionStiffness = props.SuspensionStiffness;
	tuning.m_suspensionDamping = props.SuspensionDamping;
	tuning.m_suspensionCompression = props.SuspensionCompression;
	tuning.m_frictionSlip = props.FrictionSlip;

	Vehicle = new btRaycastVehicle(tuning, getPhysicsObject(), raycaster);

	// wheels
	btVector3 fl = createBulletVector(props.WheelPositionFrontLeft);
	btVector3 fr = fl;
	fr.setX(-fr.x());
	btVector3 br = createBulletVector(props.WheelPositionBackRight);
	btVector3 bl = br;
	bl.setX(-bl.x());
	Vehicle->addWheel(fl, btVector3(0,-1,0), btVector3(-1,0,0), props.WheelRestLength, props.WheelRadius, tuning, true);
	Vehicle->addWheel(fr, btVector3(0,-1,0), btVector3(1,0,0), props.WheelRestLength, props.WheelRadius, tuning, true);
	Vehicle->addWheel(bl, btVector3(0,-1,0), btVector3(-1,0,0), props.WheelRestLength, props.WheelRadius, tuning, false);
	Vehicle->addWheel(br, btVector3(0,-1,0), btVector3(1,0,0), props.WheelRestLength, props.WheelRadius, tuning, false);
	for (int i=0; i<4; i++)
	{
		Vehicle->getWheelInfo(i).m_rollInfluence = props.RollInfluence;
		//Wheel[i] = scene->addObjectMesh(props.WheelMesh);
		Wheel[i] = new CObjectMesh(wheelMesh);
	}
	getPhysicsObject()->setDamping(0.2f, 0.3f);
}

CCar::~CCar()
{
	for (int i=0; i<4; i++)
		delete Wheel[i];
	// TODO destroy btvehicle
}

void CCar::updateWheelTransform()
{
	for (int i=0; i<4; i++)
	{
		btTransform trans = Vehicle->getWheelTransformWS(i);
		trans.getOpenGLMatrix(Wheel[i]->getTransformationPointer().M);
	}
}

void CCar::steerLeft(float dt)
{
	Steer += SteerSpeed * dt;
	if (Steer > MaxSteer)
		Steer = MaxSteer;
	Vehicle->setSteeringValue(Steer, 2);
	Vehicle->setSteeringValue(Steer, 3);
	updateWheelTransform();
}

void CCar::steerRight(float dt)
{
	Steer -= SteerSpeed * dt;
	if (Steer < -MaxSteer)
		Steer = -MaxSteer;
	Vehicle->setSteeringValue(Steer, 2);
	Vehicle->setSteeringValue(Steer, 3);
	updateWheelTransform();
}

void CCar::steerStraight(float dt)
{
	if (Steer < 0)
		Steer += SteerSpeed * dt;
	else
		Steer -= SteerSpeed * dt;
	Vehicle->setSteeringValue(Steer, 2);
	Vehicle->setSteeringValue(Steer, 3);
	updateWheelTransform();
}

void CCar::steer(float dir, float dt)
{
	if (Steer < dir)
		Steer = min(Steer + SteerSpeed * dt, dir);
	else
		Steer = max(Steer - SteerSpeed * dt, dir);
	Vehicle->setSteeringValue(Steer, 2);
	Vehicle->setSteeringValue(Steer, 3);
	updateWheelTransform();
}

void CCar::accelerate()
{
	Vehicle->applyEngineForce(EngineForce, 0);
	Vehicle->applyEngineForce(-EngineForce, 1);
	Vehicle->applyEngineForce(EngineForce, 2);
	Vehicle->applyEngineForce(-EngineForce, 3);
}

void CCar::reverse()
{
	Vehicle->applyEngineForce(-EngineForce, 2);
	Vehicle->applyEngineForce(EngineForce, 3);
}

void CCar::idle()
{
	Vehicle->applyEngineForce(0, 0);
	Vehicle->applyEngineForce(0, 1);
	Vehicle->applyEngineForce(0, 2);
	Vehicle->applyEngineForce(0, 3);
}

void CCar::brake()
{
	Vehicle->setBrake(BrakeForce, 0);
	Vehicle->setBrake(BrakeForce, 1);
	Vehicle->setBrake(BrakeForce*0.9f, 2);
	Vehicle->setBrake(BrakeForce*0.9f, 3);
}

void CCar::brakeRelease()
{
	Vehicle->setBrake(0, 0);
	Vehicle->setBrake(0, 1);
	Vehicle->setBrake(0, 2);
	Vehicle->setBrake(0, 3);
}