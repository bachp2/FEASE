#pragma once
#include <shader.h>
#include <set>
#include "camera.h"
#include "color.h"
#include "shader_manager.h"
#include <array>

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

extern ConfigParser configTable;
//ColorConfig colorConfig;

//////////////////////////////
//POINT ENTITY
//////////////////////////////
using Node = glm::vec3;

std::vector<Node> nodes;
std::vector<Node> elements;
unsigned int VAO_point, VBO_point;

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

inline void static setup_points() {
	// set point size
	glPointSize(5.0f);

	glGenVertexArrays(1, &VAO_point);
	glBindVertexArray(VAO_point);

	glGenBuffers(1, &VBO_point);
}

inline void static render_points(ShaderManager* sm) {
	glDisable(GL_DEPTH_TEST);
	Shader* shader = sm->getShader("object");
	shader->use();
	shader->setColor("color", configTable.getColor("dot"));
	
	glBindVertexArray(VAO_point);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_point);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3)*nodes.size(), &nodes[0], GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_POINTS, 0, nodes.size());
	
	glEnable(GL_DEPTH_TEST);
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

inline int vector_findi(std::vector<Node>& nv, Node n) {
	int index = 0;
	for (auto &i : nv) {
		auto a = glm::length(i - n) < 0.0001;
		/*PRINT3F(i.x, i.y, i.z);
		PRINT3F(n.x, n.y, n.z);*/
		if (a) return index;
		index++;
	}
	//printf("\n");
	return -1;
}

inline bool insert_by_unique_pair(std::vector<Node>& c, Node* n1) {
	auto t = c.size() % 2;
	if (t == 0) {
		c.push_back(*n1);
		return true;
	}

	auto n2 = c.back();
	auto span = c.size() - 1;

	if(c.size() == 1 && glm::length(n2 - *n1) < 0.0001)
	{
		// entered this condition only when a pair is selected
		// deselect node and fall through
		c.pop_back();
		return false;
	}

	for (auto i = 0; i < span; i += 2) {
		if (glm::length(c[i] - *n1) < 0.0001 && glm::length(c[i+1] - n2) < 0.0001)
		{
			// entered this condition only when a pair is selected
			// deselect node and fall through
			c.pop_back();
			return false;
		}
		if (glm::length(c[i+1] - *n1) < 0.0001 && glm::length(c[i] - n2) < 0.0001)
		{
			// entered this condition only when a pair is selected
			// deselect node and fall through
			c.pop_back();
			return false;
		}
		if (glm::length(n2 - *n1) < 0.0001)
		{
			// entered this condition only when a pair is selected
			// deselect node and fall through
			c.pop_back();
			return false;
		} 
	}
	c.push_back(*n1);
	return true;
}

//////////////////////////////
//LINE ENTITY
//////////////////////////////

unsigned int VBO_element, VAO_element;

inline static void setup_lines(){
	glGenVertexArrays(1, &VAO_element);

	glBindVertexArray(VAO_element);
	glGenBuffers(1, &VBO_element);
}

inline static void render_lines(ShaderManager* sm){
	Shader* s = sm->getShader("object");
	s->use();

	glDisable(GL_DEPTH_TEST);
	
	glLineWidth(1.0f);

	s->setColor("color", configTable.getColor("line"));

	int elementsSize = (elements.size() % 2 == 0) ? elements.size() : elements.size() - 1;

	glBindVertexArray(VAO_element);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_element);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*elementsSize, &elements[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_LINES, 0, elementsSize);

	glEnable(GL_DEPTH_TEST);
}

//////////////////////////////
//AXIS LINES ENTITY
//////////////////////////////

struct Axis {
	unsigned int vbo, vao;
	ArcBallCamera* cam;

	inline void setup(ArcBallCamera *camera) {
		const static float axis_vertices[] = {
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

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

	inline void render(ShaderManager* sm, const int scrWidth, const int scrHeight) {
		glDisable(GL_DEPTH_TEST);
		Shader* shader = sm->getShader("solid");
		shader->use();
		int ww = 320;
		glLineWidth(1.7f);
		glViewport(scrWidth - ww + 80, -100, ww, ww);
		auto model = glm::mat4(1.0f);
		model = glm::rotate(model, cam->Yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, cam->Pitch, glm::vec3(1.0f, 0.0f, 0.0f));

		auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f))*glm::inverse(model);
		//auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
		shader->setMat4("view", view1);

		auto projection1 = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		shader->setMat4("projection", projection1);

		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, 6);
		glViewport(0, 0, scrWidth, scrHeight);
		glEnable(GL_DEPTH_TEST);
	}

	inline void cleanup() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
} axisLines;


//////////////////////////////
//GRID ENTITY
//////////////////////////////


struct Grid {
	Shader* shader;
	unsigned int vbo, vao, ebo;
	unsigned int gnum; //number of grids
	float gridThickness;
	float step;
	unsigned int vertices_size;

	inline void setup(ShaderManager* sm, unsigned int grid_num = 20) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		gnum = grid_num;
		step = 1.0f / gnum;
		std::vector<Vec3> grid_vertices;
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

		shader = sm->getShader("object");
		vertices_size = grid_vertices.size();
	}

	inline void render(Mat4& view, Mat4& proj) {
		shader->use();
		shader->setMat4("projection", proj);
		shader->setMat4("view", view);
		shader->setMat4("model", glm::mat4(1.0f));
		shader->setColor("color", configTable.getColor("grid"));
		glLineWidth(0.5f);
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, vertices_size);
	}

	inline void cleanup() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
} grid;




