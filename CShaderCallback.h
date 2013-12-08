#ifndef CSHADER_CALLBACK_H_INCL
#define CSHADER_CALLBACK_H_INCL

#include "glsl.h"

class CShaderCallback
{
public:
	CShaderCallback(cwc::glShader* shader) : Shader(shader)
	{
	}

	virtual ~CShaderCallback()
	{
	}

	virtual void OnSetConstants(int data) = 0;

	cwc::glShader* Shader;
};

#endif