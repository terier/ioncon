#ifndef COBJECT_CHECKPOINT_CONTROLLER_H_INCL
#define COBJECT_CHECKPOINT_CONTROLLER_H_INCL

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
	SCheckpointTracker(CObject* object = 0, const std::string& name = "Default")
	{
		Object = object;
		LapNum = 1;
		CurrentCheckpoint = 0;
		Name = name;
	}

	CObject* Object;
	std::string Name;
	uint LapNum;
	uint CurrentCheckpoint;
};

struct SCheckpointTrackerComparator
{
	bool operator() (const SCheckpointTracker& a, const SCheckpointTracker& b)
	{
		return a.LapNum > b.LapNum || (a.LapNum == b.LapNum && a.CurrentCheckpoint > b.CurrentCheckpoint);
	}
};

// tracks checkpoints and progress for all objects

class CObjectCheckpointController : public CObject
{
public:
	CObjectCheckpointController() : CObject(0, vec3(), vec3()) {}
	virtual ~CObjectCheckpointController() {}

	void addCheckpoint(const SCheckpoint& cp);
	void addObjectTracker(CObject* object, const std::string& name = "Default");

	uint getLapNum(CObject* object);
	uint getCurrentCheckpoint(CObject* object);

	SCheckpointTracker* getTracker(uint i);
	SCheckpointTracker* getTracker(CObject* object);

	SCheckpoint* getCheckpoint(uint i);

	uint getNumberOfCheckpoints();
	uint getNumberOfTrackers();

	void sort();

	virtual void animate(float dt);

private:
	std::vector<SCheckpoint> Checkpoints;
	std::vector<SCheckpointTracker> Trackers;
};

#endif