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
			const FloatVector& tex = m.texcoords;
			const UintVector& ind = m.indices;
			glBindTexture(GL_TEXTURE_2D, getTexture());
			glBegin(GL_TRIANGLES);
			for (size_t i=0; i<ind.size(); i++)
			{
				glTexCoord2f(tex[2*ind[i]], tex[2*ind[i]+1]);
				glNormal3f(nor[3*ind[i]], nor[3*ind[i]+1], nor[3*ind[i]+2]);
				glVertex3f(pos[3*ind[i]], pos[3*ind[i]+1], pos[3*ind[i]+2]);
			}
			glEnd();
		}
	}

	CObject::render(); // render children

	glPopMatrix();
}