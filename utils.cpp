#include "utils.h"

void printvec(const vec3& v)
{
	printf("%5.2f, %5.2f, %5.2f\n", v.X, v.Y, v.Z);
}

void printfloat(float f)
{
	printf("%5.2f\n", f);
}

unsigned int loadTexture(const char* fname)
{
	int w, h, n;
	unsigned char* data = stbi_load(fname, &w, &h, &n, 4);
	if (!data)
	{
		printf("Cannot load image %s\n", fname);
		return 0;
	}
	else
	{
		unsigned int img;
		glGenTextures(1, &img);
		glBindTexture(GL_TEXTURE_2D, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		return img;
	}
}