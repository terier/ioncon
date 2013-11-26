#include "CObjectMesh.h"

void CObjectMesh::render()
{
	glPushMatrix();
	transform();

	if (mesh)
	{
		const VertexBuffer& verts = mesh->getVertexBufferRef();
		const IndexBuffer& inds = mesh->getIndexBufferRef();

		vec3 pos(getPosition());
		vec3 rot(getRotation());

		glBegin(GL_TRIANGLES);
		for (uint i=0; i<inds.size(); i++)
		{
			const CVertex& v1 = verts[inds[i]];
			glColor3f(v1.col.X, v1.col.Y, v1.col.Z);
			glTexCoord2f(v1.tex.X, v1.tex.Y);
			glNormal3f(v1.nor.X, v1.nor.Y, v1.nor.Z);
			glVertex3f(v1.pos.X, v1.pos.Y, v1.pos.Z);
		}
		glEnd();
	}

	CObject::render(); // render children

	glPopMatrix();
}