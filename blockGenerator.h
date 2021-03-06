#ifndef BLOCK_GENERATOR_H_INCL
#define BLOCK_GENERATOR_H_INCL

#include "CMesh.h"
#include "CObjectMesh.h"

typedef std::vector<CObjectMesh*> ObjectVector;

class blockGenerator
{
public:
	blockGenerator(const char* startF, const char* itemF, const char* endF,
					uint startTex, uint itemTex, uint endTex);
	virtual ~blockGenerator();
	std::vector<CObjectMesh*> generateBlock(int w, int h, int d, vec3 bPos);
private:
	CMesh* start;
	CMesh* item;
	CMesh* end;
	uint startT;
	uint itemT;
	uint endT;
};

#endif