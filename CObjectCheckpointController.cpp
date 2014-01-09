#include "CObjectCheckpointController.h"

void CObjectCheckpointController::addCheckpoint(const SCheckpoint& cp)
{
	Checkpoints.push_back(cp);
}

void CObjectCheckpointController::addObjectTracker(CObject* object)
{
	Trackers.push_back(SCheckpointTracker(object));
}

uint CObjectCheckpointController::getLapNum(CObject* object)
{
	for (uint i=0; i<Trackers.size(); i++)
	{
		if (Trackers[i].Object == object)
			return Trackers[i].LapNum;
	}
	return 0;
}

uint CObjectCheckpointController::getCurrentCheckpoint(CObject* object)
{
	for (uint i=0; i<Trackers.size(); i++)
	{
		if (Trackers[i].Object == object)
			return Trackers[i].CurrentCheckpoint;
	}
	return -1;
}

uint CObjectCheckpointController::getNumberOfCheckpoints()
{
	return Checkpoints.size();
}

void CObjectCheckpointController::animate(float dt)
{
	for (size_t i=0; i<Trackers.size(); i++)
	{
		SCheckpointTracker& ct = Trackers[i];
		if (!ct.Object)
			continue;

		SCheckpoint& cp = Checkpoints[ct.CurrentCheckpoint];
		if ((ct.Object->getPosition() - cp.Position).len() < cp.Radius)
		{
			ct.CurrentCheckpoint++;
			if (ct.CurrentCheckpoint >= Checkpoints.size())
			{
				ct.CurrentCheckpoint = 0;
				ct.LapNum++;
			}
		}
	}
}