#pragma once
#include <shader.h>
#include <set>
#include "camera.h"
#include "color.h"
#include "color_config.h"

#ifndef PI
#define PI 3.14159265358979323846
#define TAU 2*PI
#endif // !PI

//#define PRINT2F(X, Y) printf(#X ": %.2f, " #Y ": %.2f\n", X, Y);
//#define PRINT3F(X, Y, Z) printf(#X ": %.2f, " #Y ": %.2f, " #Z ": %.2f\n", X, Y, Z);

//////////////////////////////
//TERRY CUBE
//////////////////////////////
const float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

//////////////////////////////
//POINT ENTITY
//////////////////////////////

// a cube is a point!
const float cube_vertices[] = {
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
};

using Node = glm::vec3;

std::vector<Node> nodes;
std::vector<Node> elements;

inline void vector_insert(std::vector<Node>& nv, Node n) {
	if (nv.empty()) {
		nv.push_back(n); 
		return;
	}
	for (const auto &i : nv) {
		auto a = glm::length(i - n) < 0.0001;
		if (a) return;
	}
	nv.push_back(n);
}

inline Node* vector_find(std::vector<Node>& nv, Node n) {
	Node* r;
	for (auto &i : nv) {
		auto a = glm::length(i - n) < 0.0001;
		/*PRINT3F(i.x, i.y, i.z);
		PRINT3F(n.x, n.y, n.z);*/
		if (a) return r = &i;
	}
	//printf("\n");
	return nullptr;
}

inline bool insert_by_unique_pair(std::vector<Node>& c, Node* n1) {
	auto t = c.size() % 2;
	if (t == 0) {
		c.push_back(*n1);
		return true;
	}

	auto n2 = c.back();
	auto span = c.size() - 1;
	for (auto i = 0; i < span; i += 2) {
		if (glm::length(c[i] - *n1) < 0.0001 && glm::length(c[i+1] - n2) < 0.0001) return false;
		if (glm::length(c[i+1] - *n1) < 0.0001 && glm::length(c[i] - n2) < 0.0001) return false;
	}
	//printf("test\n");
	c.push_back(*n1);
	return true;
}

//////////////////////////////
//AXIS LINES ENTITY
//////////////////////////////
const float axis_vertices[] = {
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

struct Axis {
	unsigned int vbo, vao;
	ArcBallCamera* cam;

	inline void setup(ArcBallCamera *camera) {
		cam = camera;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	inline void render(Shader& shader, const int scrWidth, const int scrHeight) {
		glDisable(GL_DEPTH_TEST);
		shader.use();
		int ww = 320;
		glLineWidth(1.7f);
		glViewport(scrWidth - ww + 80, -100, ww, ww);
		auto model = glm::mat4(1.0f);
		model = glm::rotate(model, cam->Yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cam->Pitch, glm::vec3(1.0f, 0.0f, 0.0f));

		auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f))*glm::inverse(model);
		//auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("view", view1);

		auto projection1 = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		shader.setMat4("projection", projection1);

		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, 6);
		glViewport(0, 0, scrWidth, scrHeight);
		glEnable(GL_DEPTH_TEST);
		Shader::reset();
	}

	inline void cleanup() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
} axisLines;


//////////////////////////////
//GRID ENTITY
//////////////////////////////

ColorConfig colorConfig;
struct Grid {
	Shader* shader;
	unsigned int vbo, vao, ebo;
	unsigned int gnum; //number of grids
	std::vector<Vec3> grid_vertices;
	float gridThickness;
	float step;

	inline void setup(Shader* gridShader, unsigned int grid_num = 20) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		gnum = grid_num;
		step = 1.0f / gnum;

		for (float i = -0.5; i < 0.51; i += step)
		{
			//vertical lines
			grid_vertices.push_back(Vec3(i, 0.0f, -0.5f));
			grid_vertices.push_back(Vec3(i, 0.0f, 0.5f)); 
			
			// horizontal lines
			grid_vertices.push_back(Vec3(-0.5f, 0.0f, i));
			grid_vertices.push_back(Vec3(0.5f, 0.0f, i));
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3)*grid_vertices.size(), &grid_vertices[0], GL_STATIC_DRAW);

		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		shader = gridShader;
	}
	typedef glm::mat4 Mat4;
	inline void render(Mat4& view, Mat4& proj) {
		shader->use();
		shader->setMat4("projection", proj);
		shader->setMat4("view", view);
		shader->setMat4("model", glm::mat4(1.0f));
		auto grid_color = colorConfig.pallete["gridline"];
		shader->setVec3("color", grid_color.r, grid_color.g, grid_color.b);
		glLineWidth(0.5f);
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, grid_vertices.size());
		Shader::reset();
	}

	inline void cleanup() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
} grid;
