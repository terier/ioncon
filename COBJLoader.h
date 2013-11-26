#ifndef COBJ_LOADER_H_INCL
#define COBJ_LOADER_H_INCL

#include "CVertex.h"
#include "CMesh.h"

class COBJLoader
{
public:
	COBJLoader() {}
	CMesh* loadMesh(char* filename);
};

#endif