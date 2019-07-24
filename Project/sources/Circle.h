#pragma once
#include "shape.h"

class Circle : public Mesh {
public:
	Circle(float radius, int segments) {
		float x, y;
		vertices.reserve(segments + 1);
		vertices.push_back({ 0,0,0 });
		for (int ii = 0; ii <= segments; ii++)
		{
			float theta = 2.0f * PI * float(ii) / segments;//get the current angle
			x = radius * cosf(theta);//calculate the x component
			y = radius * sinf(theta);//calculate the y component
			vertices.push_back({x,y,0});
		}

		genesis.g_colors[0] = Color::hex("#2fbdb1");

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	}

	~Circle() {
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		/*glDeleteBuffers(1, &this->genesis.g_ebos[0]);*/
	}

	void render(Shader* s) {
		s->use();
		s->setMat4("projection", per_proj);
		s->setMat4("view", view);
		s->setMat4("model", this->model);
		s->setColor("color", genesis.g_colors[0]);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size());
	}
};