#include <stdlib.h>
#include <stdio.h>
#include "CGLUTApplication.h"
#include "CGLUTInterface.h"

int main(int argc, char **argv)
{
	SGLUTParameters param;
	CGLUTApplication app(param);
	CGLUTInterface::connect(&app);
	glutMainLoop();
	
	return 1;
}