#pragma once
#include "common.h"
#define PI 3.14159265358979323846f
class Circle : public Mesh {
public:
	enum Style {
		SOLID,
		LINE,
	} style;

	Circle(float radius, int segments, Style s = Style::SOLID) {
		style = s;
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
		orientation = glm::quat(glm::vec3(0, 0, 0));
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
		//auto m = glm::toMat4(-mCamera.orientation())*this->model;
		//m = glm::translate(m, glm::vec3(0.5, 0.5, 0));
		auto m = glm::mat4(1.0f);
		//glm::translate(glm::toMat4(orientation), glm::vec3(0.5, 0.5, 0));
		m = glm::translate(glm::mat4(1.0f), glm::vec3(0.2, 0.2, 0)) * glm::toMat4(orientation)*m;
		//m = glm::toMat4(glm::normalize(glm::quat(glm::vec3(0,PI/2,0))))*m;
		
		s->setMat4("model", m);
		s->setColor("color", genesis.g_colors[0]);

		glBindVertexArray(vao);
		if(style == SOLID)
			glDrawArrays(GL_TRIANGLE_FAN, 0, this->vertices.size());
		if (style == LINE) {

			/*glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);*/
			glDrawArrays(GL_LINE_LOOP, 0, this->vertices.size());
		}
	}
};