#include "CObjectMesh.h"

void CObjectMesh::render()
{
	glPushMatrix();
	transform();

	if (Mesh)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindTexture(GL_TEXTURE_2D, getTexture());
		const VBOVector& vbos = Mesh->getVBOs();
		for (size_t i=0; i<vbos.size(); i++)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i].buffer[VBO_VERTEX_BUFFER]);
			glVertexPointer(3, GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, vbos[i].buffer[VBO_TEXCOORD_BUFFER]);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, vbos[i].buffer[VBO_NORMAL_BUFFER]);
			glNormalPointer(GL_FLOAT, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[i].buffer[VBO_INDEX_BUFFER]);
			glIndexPointer(GL_UNSIGNED_INT, 0, 0);

			glDrawElements(GL_TRIANGLES, Mesh->getShapes()[i].mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	CObject::render(); // render children

	glPopMatrix();
}