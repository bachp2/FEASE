#pragma once
#include "shape.h"
#include "cube.h"
#define PI 3.14159265358979323846

class UVSphere : public Shape
{
public:
	UVSphere(float radius, int longc, int latc);

	void render(Shader* s);
};

class NormalizedCube : public Cube 
{
public:
	NormalizedCube() {
	}
};

class SpherifiedCube : public Cube
{
public:

};

class IcoSphere : public Shape
{
public:

};