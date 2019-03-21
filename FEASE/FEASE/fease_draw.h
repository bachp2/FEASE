#pragma once
#include <shader.h>
#include <set>
#include "camera.h"

#ifndef PI
#define PI 3.14159265358979323846
#define TAU 2*PI
#endif // !PI

//#define PRINT2F(X, Y) printf(#X ": %.2f, " #Y ": %.2f\n", X, Y);
//#define PRINT3F(X, Y, Z) printf(#X ": %.2f, " #Y ": %.2f, " #Z ": %.2f\n", X, Y, Z);

//////////////////////////////
//COLOR STRUCT AND UTILS
//////////////////////////////
struct Color {
	float r, g, b;
};

inline static Color hexCodeToRGB(std::string input) {
	if (input[0] == '#')
		input.erase(0, 1);

	unsigned int value = stoul(input, nullptr, 16);

	Color color;
	color.r = ((value >> 16) & 0xff) / 255.0;
	color.g = ((value >> 8) & 0xff) / 255.0;
	color.b = ((value >> 0) & 0xff) / 255.0;
	return color;
}

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
	}
	else {
		auto n2 = c.back();
		auto span = c.size() - 1;
		for (auto i = 0; i < span; i += 2) {
			if (glm::length(c[i] - *n1) < 0.0001 || glm::length(c[i+1] - n2) < 0.0001) return false;
			if (glm::length(c[i+1] - *n1) < 0.0001 || glm::length(c[i] - n2) < 0.0001) return false;
		}
		//printf("test\n");
		c.push_back(*n1);
	}
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
		glViewport(scrWidth - ww + 80, -100, ww, ww);
		auto model = glm::mat4(1.0f);
		model = glm::rotate(model, cam->Yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cam->Pitch, glm::vec3(1.0f, 0.0f, 0.0f));

		auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f))*glm::inverse(model);

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
const float grid_vertices[] = {
	0.5f, 0.0f,  0.5f, 1.0f, 1.0f,   // top right
	0.5f, 0.0f, -0.5f, 0.0f, 1.0f,   // bottom right
	-0.5f, 0.0f, -0.5f, 0.0f, 0.0f,  // bottom left
	-0.5f, 0.0f, 0.5f, 1.0f, 0.0f   // top left 
};

const unsigned int grid_indices[] = {
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

struct Grid {
	Shader* shader;
	unsigned int vbo, vao, ebo;
	unsigned int gnum; //number of grids
	float gridThickness;
	float step;
	inline void setup(Shader* gridShader, const unsigned int scrWidth, const unsigned int scrHeight) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(grid_vertices), grid_vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grid_indices), grid_indices, GL_STATIC_DRAW);
		
		shader = gridShader;
		shader->use();
		//Color major_color = hexCodeToRGB("#031641"); 
		Color grid_color = hexCodeToRGB("#C0C0C0");
		Color backgroundColor = hexCodeToRGB("#07254f");
		
		shader->setVec3("gridColor", glm::vec3(grid_color.r, grid_color.g, grid_color.b));
		shader->setVec3("backgroundColor", glm::vec3(backgroundColor.r, backgroundColor.g, backgroundColor.b));

		gnum = 20; //has to an even number!
		shader->setFloat("divisions", float(gnum));

		gridThickness = 0.03;
		shader->setFloat("thickness", gridThickness);

		step = 1.0f / gnum;

		//shader->setVec2("resolution", glm::vec2(float(scrWidth), float(scrHeight)) );
		Shader::reset();
	}

	inline void render() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	inline void cleanup() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
} grid;
