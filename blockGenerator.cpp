#include "blockGenerator.h"
#include "tiny_obj_loader.h"

blockGenerator::blockGenerator(const char* startF, const char* itemF, const char* endF,
								uint startTex, uint itemTex, uint endTex)
{
	start = new CMesh(startF);
	item = new CMesh(itemF);
	end = new CMesh(endF);
	startT = startTex;
	itemT = itemTex;
	endT = endTex;
}

blockGenerator::~blockGenerator()
{
	delete start;
	delete item;
	delete end;
}

std::vector<CObjectMesh*> blockGenerator::generateBlock(int w, int h, int d, vec3 bPos)
{
	ObjectVector finalObjectVect;

	vec3 basePos;
	vec3 prevPos;
	for(int k=0; k<d; k++)
	{
		for(int i=0; i<w; i++)
		{
			basePos = bPos + vec3(k*20.0f, 0, i*20.0f);
			//create base object
			CObjectMesh* startObjMesh = new CObjectMesh(start);
			startObjMesh->setTexture(0, startT);
			startObjMesh->setPosition(basePos);
			finalObjectVect.push_back(startObjMesh);

			//append middle parts
			prevPos = startObjMesh->getPosition();
			vec3 curPos = prevPos;
			if( (k==0 || k==d-1) || (i==0 || i==w-1)) //make it hallow
			{
				for(int j=0; j<h; j++)
				{
					//printf("Generating main part...\n");
					if(j==0)
						curPos = prevPos + vec3(0,20.0f,0);
					else
						curPos = prevPos + vec3(0,13.5f,0);
					CObjectMesh* tempObj = new CObjectMesh(item);
					tempObj->setTexture(0, itemT);
					tempObj->setPosition(curPos);
					finalObjectVect.push_back(tempObj);
					prevPos = curPos;
					CObjectMesh* middleObjMesh = new CObjectMesh(item);
				}
			}
			else
			{
				prevPos = prevPos + vec3(0,20.0f,0) + vec3(0,(h-1)*13.5f,0); //fix position
			}

			//append end part
			CObjectMesh* endObj = new CObjectMesh(end);
			endObj->setPosition(prevPos + vec3(0,13.5f,0));
			finalObjectVect.push_back(endObj);
		}
	}

	return finalObjectVect;
}