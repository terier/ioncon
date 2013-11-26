#include "CObjectMesh.h"

void CObjectMesh::render()
{
	glPushMatrix();
	transform();

	if (Mesh)
	{
		const ShapeVector& shapes = Mesh->getShapes();
		for (ShapeVector::const_iterator shape = shapes.begin(); shape != shapes.end(); shape++)
		{
			const mesh_t& m = shape->mesh;
			const FloatVector& pos = m.positions;
			const FloatVector& nor = m.normals;
			const UintVector& ind = m.indices;
			glBegin(GL_TRIANGLES);
			glColor3f(1.f,0.f, 0.5f);
			for (size_t i=0; i<ind.size(); i++)
			{
				glNormal3f(nor[3*ind[i]], nor[3*ind[i]+1], nor[3*ind[i]+2]);
				glVertex3f(pos[3*ind[i]], pos[3*ind[i]+1], pos[3*ind[i]+2]);
			}
			glEnd();
		}
	}

	CObject::render(); // render children

	glPopMatrix();
}