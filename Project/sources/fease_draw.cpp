#include "fease_draw.h"

void vector_insert(std::vector<Node>& nv, Node n) {
	if (nv.empty()) {
		nv.push_back(n);
		return;
	}
	for (const auto& i : nv) {
		auto a = glm::length(i - n) < 0.0001;
		if (a) return;
	}
	nv.push_back(n);
}

inline void Grid::setup(Shader* s, unsigned int grid_num) {
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

	glBufferData(
		GL_ARRAY_BUFFER, sizeof(Vec3) * grid_vertices.size(),
		grid_vertices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	shader = s;
	vertices_size = grid_vertices.size();
}

inline void Grid::render() {
	shader->use();
	shader->setMat4("projection", perspective_projection);
	shader->setMat4("view", view);
	shader->setMat4("model", glm::mat4(1.0f));
	shader->setColor("color", configTable.color("grid"));
	glLineWidth(0.5f);
	glBindVertexArray(vao);
	//printf("%d\n", vertices_size);
	glDrawArrays(GL_LINES, 0, vertices_size);
}

inline void Grid::cleanup() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

inline void Axis::setup(ArcBallCamera* camera) {
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

inline void Axis::render(Shader* shader, const int scrWidth, const int scrHeight) {
	glDisable(GL_DEPTH_TEST);
	shader->use();
	int ww = 320;
	glLineWidth(1.7f);
	glViewport(scrWidth - ww + 80, -100, ww, ww);
	auto model = glm::mat4(1.0f);
	model = glm::rotate(model, cam->Yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, cam->Pitch, glm::vec3(1.0f, 0.0f, 0.0f));

	auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 8.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f)) * glm::inverse(model);
	//auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	shader->setMat4("view", view1);

	auto projection1 = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	shader->setMat4("projection", projection1);

	glBindVertexArray(vao);
	glDrawArrays(GL_LINES, 0, 6);
	glViewport(0, 0, scrWidth, scrHeight);
	glEnable(GL_DEPTH_TEST);
}

inline void Axis::cleanup() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}
