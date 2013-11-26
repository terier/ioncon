#include <stdlib.h>
#include <stdio.h>
#include "COBJLoader.h"
#include "CMesh.h"

CMesh* COBJLoader::loadMesh(char* filename)
{
	printf("Loading OBJ file %s...\n", filename);
	CMesh* ret = new CMesh();
	char ch;
	float v1, v2, v3, n1, n2, n3, t1, t2;
	uint f1, f2, f3;
	uint nTex = 0, nNor = 0;

	FILE* f = fopen(filename, "r");
	if(f == NULL)
	{
		printf("ERROR - file is null!\n");
		return NULL;
	}
	else
	{
		while( (ch = fgetc(f)) != EOF)
		{
			if(ch == 'v' || ch == 'V')
			{
				ch = fgetc(f);
				if(ch == ' ')
				{
					fscanf(f, "%f %f %f", &v1, &v2, &v3);
					//printf("POINT: %f, %f, %f\n", v1, v2, v3);
					vec3 pos(v1, v2, v3);
					CVertex vert(pos);
					ret->addVertex(vert);
				}
				else if(ch == 'n' || ch == 'N')
				{
					fscanf(f, "%f %f %f", &n1, &n2, &n3);
					CVertex& temp = ret->getVertex(nNor);
					vec3 Nor(n1, n2, n3);
					temp.nor = Nor;
					nNor ++;
				}
				else if(ch == 't' || ch == 'T')
				{
					fscanf(f, "%f %f", &t1, &t2);
					CVertex& temp = ret->getVertex(nTex);
					vec3 Tex(t1, t2, 0);
					temp.tex = Tex;
					nTex++;
				}
			}
			else if(ch == 'f' || ch == 'F')
			{
				fscanf(f, "%d %d %d", &f1, &f2, &f3);
				//printf("TRIANGLE: %u, %u, %u\n", f1, f2, f3);
				ret->addTriangle(f1-1, f2-1, f3-1);
			}
		}
	}

	fclose(f);

	return ret;
}