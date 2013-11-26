#include "CMesh.h"

void CMesh::addTriangle(uint v1, uint v2, uint v3)
{
	indices.push_back(v1);
	indices.push_back(v2);
	indices.push_back(v3);
}