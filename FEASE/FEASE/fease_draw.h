#pragma once
#include <shader.h>
#ifndef PI
#define PI 3.14159265358979323846
#define TAU 2*PI
#endif // !PI


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

std::vector<glm::vec3> pointsLoc;
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

const float axis_vertices[] = {
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};


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
		Color grid_color = hexCodeToRGB("#004883");
		
		shader->setVec4("gridColor", glm::vec4(grid_color.r, grid_color.g, grid_color.b, 1.0));

		gnum = 10;
		shader->setInt("multiplicationFactor", gnum);

		gridThickness = 0.02;
		shader->setFloat("threshold", gridThickness);
		step = 1.0f / gnum;
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
