#ifndef CMESH_H_INCL
#define CMESH_H_INCL

#include "CVertex.h"
#include <vector>

typedef unsigned int uint;
typedef std::vector<CVertex> VertexBuffer;
typedef std::vector<uint> IndexBuffer;

class CMesh
{
public:
	CMesh() {}
	const VertexBuffer& getVertexBufferRef() const { return vertices; }
	const IndexBuffer& getIndexBufferRef() const { return indices; }
	void addVertex(const CVertex& v) { vertices.push_back(v); }
	CVertex& getVertex(int i) { return vertices[i]; }
	void addTriangle(uint v1, uint v2, uint v3);

private:
	VertexBuffer vertices;
	IndexBuffer indices;
};

#endif