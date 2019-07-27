#pragma once
#include "shader.h"
#include "camera.h"
#include <vector>
#include <glm/glm.hpp>

extern glm::mat4 per_proj, view, model, ort_proj;
extern ArcBallCamera mCamera;

class Mesh {
public:
	struct GDef {
		const static int MAX_ALLOWED = 5;
		unsigned int ebos[MAX_ALLOWED] = { 0 };
		Color colors[MAX_ALLOWED];
		int elems_count[MAX_ALLOWED];
	};

	// function params should be in radians
	virtual void set_orientation(float pitch, float yaw, float roll)
	{
		this->orientation = glm::quat(glm::vec3(pitch, yaw, roll));
	}

	virtual void set_orientation(glm::quat q)
	{
		this->orientation = q;
	}

	virtual void rotate_to(glm::quat& to)
	{
		
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
		for (auto& a : basis.ebos)
		{
			glDeleteBuffers(1, &a);
		}
	}

	virtual void render(Shader* s) {};
	glm::quat orientation;
protected:
	unsigned int vao{0}, vbo{0};
	std::vector<glm::vec3> vertices;
	glm::mat4 model{1.0f};
	GDef basis;
};
