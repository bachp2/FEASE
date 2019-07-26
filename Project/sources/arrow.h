#pragma once
#include "common.h"

class Arrow : public Mesh {
public:
	Arrow()
	{
		float vertices[] = {
			   0, 0, 0,
			   0, 0, 0.5,
			-0.02, 0, 0.4,
			 0.02, 0, 0.4
		};

		unsigned int line[] = {0, 1};
		unsigned int head[] = {1, 2, 3};

		genesis.g_colors[0] = Color::hex("#2fbdb1");

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &genesis.g_ebos[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, genesis.g_ebos[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(line), line, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &genesis.g_ebos[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, genesis.g_ebos[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(head), head, GL_DYNAMIC_DRAW);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	}

	void render(Shader* s)
	{
		s->use();
		s->setMat4("projection", per_proj);
		s->setMat4("view", view);
		s->setMat4("model", this->model);
		s->setColor("color", genesis.g_colors[0]);
		glBindVertexArray(vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, genesis.g_ebos[0]);
		glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, genesis.g_ebos[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	}
};