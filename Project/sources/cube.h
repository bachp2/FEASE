#pragma once
#include "shape.h"

class Cube : public Shape {
public:
	Cube();
	Cube(int sectors_on_face) 
	{
		static const float vertices[] = {
			-0.5f, 0.5f, -0.5f,
			 0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f,  0.5f,
			 0.5f, 0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
		};

		genesis.g_colors[0] = Color::hex("#2fbdb1");

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	}

	void render(Shader* s);;
};