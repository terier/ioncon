#ifndef ICR_LOADER_H_INCL
#define ICR_LOADER_H_INCL

/* IonCon Road (.icr) loader */

#include "CObjectSpline.h"
#include "CScene.h"

CObjectSpline* loadRoad(const char* fname, CScene* scene = 0);

#endif