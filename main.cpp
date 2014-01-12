#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "CGLUTApplication.h"
#include "CGLUTInterface.h"

int main(int argc, char **argv)
{
	int car;
	int skyDome;

	char* cars[7] = {
		"models/cars/corvette.icc",
		"models/cars/california.icc",
		"models/cars/911.icc",
		"models/cars/carreraGT.icc",
		"models/cars/mustang.icc",
		"models/cars/SL500.icc",
		"models/cars/sls_amg.icc",
	};

	char* skyDomes[4] = {
		"images/sky1.jpg",
		"images/sky2.jpg",
		"images/sky3.jpg",
		"images/sky4.jpg"
	};

	srand((unsigned int) time(0));
	SGLUTParameters param;
	param.FSAA = true;
	param.WindowTitle = "IonCon 0.1";
	printf("Enter the number of desired sky dome (0-3): ");
	scanf("%d", &skyDome);
	param.SkyDome = skyDomes[skyDome];
	printf("\nEnter the number of desired car:\n0: Corvette, 1: California, 2: 911,\n3: Carrera GT, 4: Mustang, 5: SL500, 6: SLS AMG;\n");
	scanf("%d", &car);
	param.Car = cars[car];

	CGLUTApplication app(param);
	CGLUTInterface::connect(&app);
	glutMainLoop();
	
	return 1;
}