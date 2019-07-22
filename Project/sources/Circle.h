#pragma once
#include "shape.h"

class Circle : public Mesh {
public:
	Circle(float radius, int segments) {
		float x, y;
		vertices.reserve(segments + 1);
		vertices.push_back({ 0,0,0 });
		for (int ii = 0; ii < segments; ii++)
		{
			float theta = 2.0f * PI * float(ii) / segments;//get the current angle
			x = radius * cosf(theta);//calculate the x component
			y = radius * sinf(theta);//calculate the y component
			vertices.push_back({x,y,0});
		}
	}

	void render(Shader* s) {
		
	}
};