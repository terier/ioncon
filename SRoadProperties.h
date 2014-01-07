#ifndef SROAD_PROPERTIES_H_INCL
#define SROAD_PROPERTIES_H_INCL

#include <vector>
#include "CSpline.h"

struct SRoadProperties
{
	SRoadProperties()
	{
		ScaleSpline = 1;
		ScaleTexture = 1;
		ScaleStencil = vec3(1, 1, 1);
		Subdiv = 100;
	}

	std::vector<vec3> Stencil;
	float ScaleSpline;
	float ScaleTexture;
	vec3 ScaleStencil;
	int Subdiv;
};

#endif