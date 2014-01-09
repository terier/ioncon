#ifndef COBJECT_CHECKPOINT_CONTROLLER
#define COBJECT_CHECKPOINT_CONTROLLER

#include "CObject.h"
#include <vector>

// a checkpoint is a sphere

struct SCheckpoint
{
	SCheckpoint() :
	Radius(1)
	{
	}

	SCheckpoint(const vec3& position, float radius) :
	Position(position), Radius(radius)
	{
	}

	vec3 Position;
	float Radius;
};

// checkpoint tracker tracks current position for one object

struct SCheckpointTracker
{
	SCheckpointTracker()
	{
		Object = 0;
		LapNum = 1;
		CurrentCheckpoint = 0;
	}

	SCheckpointTracker(CObject* object)
	{
		Object = object;
		LapNum = 1;
		CurrentCheckpoint = 0;
	}

	CObject* Object;
	uint LapNum;
	uint CurrentCheckpoint;
};

// tracks checkpoints and progress for all objects

class CObjectCheckpointController : public CObject
{
public:
	CObjectCheckpointController() : CObject(0, vec3(), vec3()) {}
	virtual ~CObjectCheckpointController() {}

	void addCheckpoint(const SCheckpoint& cp);
	void addObjectTracker(CObject* object);
	uint getLapNum(CObject* object);
	uint getCurrentCheckpoint(CObject* object);
	uint getNumberOfCheckpoints();

	virtual void animate(float dt);

private:
	std::vector<SCheckpoint> Checkpoints;
	std::vector<SCheckpointTracker> Trackers;
};

#endif