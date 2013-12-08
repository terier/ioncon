#ifndef CSHADER_DEFAULT_CALLBACK_H_INCL
#define CSHADER_DEFAULT_CALLBACK_H_INCL

#include "CShaderCallback.h"

class CShaderDefaultCallback : public CShaderCallback
{
public:
	CShaderDefaultCallback(cwc::glShader* shader) :
	  CShaderCallback(shader)
	{
	}

	virtual void OnSetConstants(int data)
	{
		uint* textures = reinterpret_cast<uint*>(data);
		Shader->setUniform1i("colorMap", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		Shader->setUniform1i("normalMap", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		Shader->setUniform1i("stencilMap", 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
	}
};

#endif