#pragma once
#include "shader.h"
#include <vector>
#include <glm/glm.hpp>
extern glm::mat4 per_proj, view, model, ort_proj;
class Shape {
public:
	struct GDef {
		const static int MAX_ALLOWED = 5;
		unsigned int g_ebos[MAX_ALLOWED] = { 0 };
		std::vector<unsigned int> g_elems[MAX_ALLOWED];
		Color g_colors[MAX_ALLOWED];
	};

	virtual void render(Shader* s) {};
protected:
	unsigned int vao{0}, vbo{0};
	std::vector<glm::vec3> vertices;
	glm::mat4 model{1.0f};
	GDef genesis;
};
