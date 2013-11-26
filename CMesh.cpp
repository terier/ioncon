#include "CMesh.h"
#include <iostream>
#include <assert.h>

CMesh::CMesh(const char* fname, const char* base)
{
	std::string err = LoadObj(Shapes, fname, base);
	if (!err.empty())
		std::cout << err << "\n";
	else
		printf("Loaded %s\n", fname);
}