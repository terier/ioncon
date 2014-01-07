#ifndef ICR_LOADER_H_INCL
#define ICR_LOADER_H_INCL

/* IonCon Road (.icr) loader */

#include "CMesh.h"
#include "SRoadProperties.h"

CMesh* loadRoad(const char* fname, CSpline** splinedst);
void loadRoadProperties(const char* fname, SRoadProperties& props);

#endif