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
		glEnableClientState(GL_INDEX_ARRAY);

		if (Shader)
			Shader->begin();
		if (ShaderCallback)
			ShaderCallback->OnSetConstants(reinterpret_cast<int>(getTextures()));

		const VBOVector& vbos = Mesh->getVBOs();
		for (size_t i=0; i<vbos.size(); i++)
		{
			const shape_t& shape = Mesh->getShapes()[i];

			const material_t& mat = shape.material;
			float value[4];
			value[3] = 1; // alpha
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shape.material.shininess);
			memcpy(value, mat.diffuse, 3 * sizeof(float));
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, value);
			memcpy(value, mat.ambient, 3 * sizeof(float));
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, value);
			memcpy(value, mat.specular, 3 * sizeof(float));
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, value);
			memcpy(value, mat.emission, 3 * sizeof(float));
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, value);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, getTexture(0));

			glBindBuffer(GL_ARRAY_BUFFER, vbos[i].buffer[VBO_VERTEX_BUFFER]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i].buffer[VBO_TEXCOORD_BUFFER]);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, vbos[i].buffer[VBO_NORMAL_BUFFER]);
			glNormalPointer(GL_FLOAT, 0, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[i].buffer[VBO_INDEX_BUFFER]);

			glDrawElements(GL_TRIANGLES, shape.mesh.indices.size(), GL_UNSIGNED_INT, 0);
		}

		if (Shader)
			Shader->end();

		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	CObject::render(); // render children

	glPopMatrix();
}