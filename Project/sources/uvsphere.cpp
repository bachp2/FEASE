#include "sphere.h"

UVSphere::~UVSphere() {
	glDeleteVertexArrays(1, &this->vao);
	glDeleteBuffers(1, &this->vbo);
	glDeleteBuffers(1, &this->genesis.g_ebos[0]);
}

UVSphere::UVSphere(float radius, int longc, int latc) {

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float sectorStep = 2 * PI / longc;
	float stackStep = PI / latc;
	float sectorAngle, stackAngle;
	this->vertices.reserve(latc * longc * 2);
	for (int i = 0; i <= latc; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, different tex coords
		for (int j = 0; j <= longc; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(glm::vec3(x, y, z));
			// normalized vertex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			vertices.push_back(glm::vec3(nx, ny, nz));
		}
	}

	genesis.g_colors[0] = Color::hex("#2fbdb1");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);

	int k1, k2;
	for (int i = 0; i < latc; ++i)
	{
		k1 = i * (longc + 1);     // beginning of current stack
		k2 = k1 + longc + 1;      // beginning of next stack

		for (int j = 0; j < longc; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				genesis.g_elems[0].push_back(k1);
				genesis.g_elems[0].push_back(k2);
				genesis.g_elems[0].push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (latc - 1))
			{
				genesis.g_elems[0].push_back(k1 + 1);
				genesis.g_elems[0].push_back(k2);
				genesis.g_elems[0].push_back(k2 + 1);
			}
		}
	}

	glGenBuffers(1, &genesis.g_ebos[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, genesis.g_ebos[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * genesis.g_elems[0].size(),
		genesis.g_elems[0].data(), GL_DYNAMIC_DRAW);

	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));

	glBindVertexArray(0);
}

void UVSphere::render(Shader* s) {
	s->use();
	s->setMat4("projection", per_proj);
	s->setMat4("view", view);
	s->setMat4("model", this->model);
	s->setVec3("lightPos", camera.getPosition());
	s->setColor("objectColor", genesis.g_colors[0]);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, genesis.g_ebos[0]);
	glDrawElements(GL_TRIANGLES, genesis.g_elems[0].size(), GL_UNSIGNED_INT, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
