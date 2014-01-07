#ifndef ROAD_GENERATOR_H_INCL
#define ROAD_GENERATOR_H_INCL

#include "CSpline.h"

CSpline* generateRoad(int numControlPoints, float radius, float radiusJitter, float directionJitterRad);

#endif