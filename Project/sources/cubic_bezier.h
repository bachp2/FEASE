#pragma once
#include "mesh.h"

class BezierCurve : public Mesh
{
public:

	BezierCurve(glm::vec3 cpoints[4], int segments = 100)
	{
		const auto frac = 1.0f / segments;
		for (int i = 0; i < segments; i++)
		{
			auto t = frac * i;
			auto t2 = t * t;
			auto t3 = t2 * t;
			auto u = 1 - t;
			auto u2 = u * u;
			auto u3 = u2 * u;
			auto c = u3 * cpoints[0] + 3 * t * u2 * cpoints[1] + 3*t2*u*cpoints[2] + t3*cpoints[3];
			vertices.push_back(c);
		}
		basis.colors[0] = Color::hex("#2fbdb1");

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

		glBindVertexArray(0);
	}

	

	void render(Shader* s)
	{
		s->use();
		s->setMat4("projection", per_proj);
		s->setMat4("view", view);
		s->setMat4("model", this->model);
		s->setColor("objectColor", basis.colors[0]);

		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
	}
};