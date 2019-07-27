#pragma once
#include "common.h"
#include "cube.h"

class UVSphere : public Mesh
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

class IcoSphere : public Mesh
{
public:

};