#include "icc_loader.h"

#include <stdio.h>

void loadCarProperties(const char* fname, SCarProperties& props, char* carMesh, char* hullMesh, char* wheelMesh)
{
	FILE* f = fopen(fname, "r");
	if (!f)
	{
		fprintf(stderr, "Could not open file %s!\n", fname);
		return;
	}

	char type[50];
	while (fscanf(f, "%s", &type) > 0)
	{
		if (!strcmp(type, "WheelPositionFrontLeft")) {
			fscanf(f, "%f %f %f", &props.WheelPositionFrontLeft.X, &props.WheelPositionFrontLeft.Y, &props.WheelPositionFrontLeft.Z);
		} else if (!strcmp(type, "WheelPositionBackRight")) {
			fscanf(f, "%f %f %f", &props.WheelPositionBackRight.X, &props.WheelPositionBackRight.Y, &props.WheelPositionBackRight.Z);
		} else if (!strcmp(type, "WheelRestLength")) {
			fscanf(f, "%f", &props.WheelRestLength);
		} else if (!strcmp(type, "WheelRadius")) {
			fscanf(f, "%f", &props.WheelRadius);
		} else if (!strcmp(type, "BumperCamera")) {
			fscanf(f, "%f %f %f", &props.BumperCamera.X, &props.BumperCamera.Y, &props.BumperCamera.Z);
		} else if (!strcmp(type, "MaxSteer")) {
			fscanf(f, "%f", &props.MaxSteer);
		} else if (!strcmp(type, "SteerSpeed")) {
			fscanf(f, "%f", &props.SteerSpeed);
		} else if (!strcmp(type, "EngineForce")) {
			fscanf(f, "%f", &props.EngineForce);
		} else if (!strcmp(type, "BrakeForce")) {
			fscanf(f, "%f", &props.BrakeForce);
		} else if (!strcmp(type, "Mass")) {
			fscanf(f, "%f", &props.Mass);
		} else if (!strcmp(type, "SuspensionStiffness")) {
			fscanf(f, "%f", &props.SuspensionStiffness);
		} else if (!strcmp(type, "SuspensionDamping")) {
			fscanf(f, "%f", &props.SuspensionDamping);
		} else if (!strcmp(type, "SuspensionCompression")) {
			fscanf(f, "%f", &props.SuspensionCompression);
		} else if (!strcmp(type, "FrictionSlip")) {
			fscanf(f, "%f", &props.FrictionSlip);
		} else if (!strcmp(type, "RollInfluence")) {
			fscanf(f, "%f", &props.RollInfluence);
		} else if (!strcmp(type, "CarMesh")) {
			fscanf(f, "%s", carMesh);
		} else if (!strcmp(type, "HullMesh")) {
			fscanf(f, "%s", hullMesh);
		} else if (!strcmp(type, "WheelMesh")) {
			fscanf(f, "%s", wheelMesh);
		}
	}

	fclose(f);
}