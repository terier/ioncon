#ifndef ICC_LOADER_H_INCL
#define ICC_LOADER_H_INCL

#include "SCarProperties.h"

void loadCarProperties(const char* fname, SCarProperties& props, char* carMesh, char* hullMesh, char* wheelMesh);

#endif