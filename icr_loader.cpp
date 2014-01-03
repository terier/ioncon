#include "icr_loader.h"
#include <stdio.h>
#include <vector>
#include "CSpline.h"

CMesh* loadRoad(const char* fname, CSpline** splinedst)
{
	printf("loading %s...\n", fname);
	FILE* f = fopen(fname, "r");
	if (!f)
	{
		printf("cannot open file %s!\n", fname);
		return 0;
	}

	CSpline* spline = new CSpline();
	std::vector<vec3> stencil;
	vec3 stencilscale;
	vec3 b1, b2, b3, b4; // buffers
	float scale = 1.f;
	float texscale = 0.01f;
	int subdiv = 200;

	char type[50];
	while (fscanf(f, "%s", &type) > 0)
	{
		if (!strcmp(type, "stencil")) {
			fscanf(f, "%f %f %f", &b1.X, &b1.Y, &b1.Z);
			stencil.push_back(b1);
		} else if (!strcmp(type, "control")) {
			fscanf(f, "%f %f %f", &b1.X, &b1.Y, &b1.Z);
			fscanf(f, "%f %f %f", &b2.X, &b2.Y, &b2.Z);
			fscanf(f, "%f %f %f", &b3.X, &b3.Y, &b3.Z);
			fscanf(f, "%f %f %f", &b4.X, &b4.Y, &b4.Z);
			spline->addControlPoint(SControlPoint(b1, b2, b3, b4) * scale);
		} else if (!strcmp(type, "scalespline")) {
			fscanf(f, "%f", &scale);
		} else if (!strcmp(type, "scaletexture")) {
			fscanf(f, "%f", &texscale);
		} else if (!strcmp(type, "scalestencil")) {
			fscanf(f, "%f %f", &stencilscale.X, &stencilscale.Y);
		} else if (!strcmp(type, "subdiv")) {
			fscanf(f, "%d", &subdiv);
		} else if (!strcmp(type, "#")) {
			while (fgetc(f) != '\n')
			{ // deliberately empty
			}
		} else if (!strcmp(type, "break")) {
			break;
		}
	}
	fclose(f);

	*splinedst = spline;

	return new CMesh(spline, stencil, subdiv, texscale, stencilscale);
}