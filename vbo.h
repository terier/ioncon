#ifndef VBO_H_INCL
#define VBO_H_INCL

#include "opengl.h"
#include "utils.h"

#define VBO_VERTEX_BUFFER 0
#define VBO_TEXCOORD_BUFFER 1
#define VBO_NORMAL_BUFFER 2
#define VBO_INDEX_BUFFER 3

struct vbo_t
{
	GLuint buffer[4];
};

#endif