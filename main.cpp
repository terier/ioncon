#include <stdlib.h>
#include <stdio.h>
#include "CGLUTApplication.h"
#include "CGLUTInterface.h"

int main(int argc, char **argv)
{
	SGLUTParameters param;
	param.FSAA = true;
	param.WindowTitle = "IonCon 0.1";
	CGLUTApplication app(param);
	CGLUTInterface::connect(&app);
	glutMainLoop();
	
	return 1;
}