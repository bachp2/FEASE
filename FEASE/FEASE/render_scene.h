#pragma once
class ArcBallCamera;
class Shader;
extern ArcBallCamera camera;
extern glm::mat4 perspective_projection, view, model, orthogonal_projection;
extern Shader textShader, solidShader, objectShader;
extern int scrWidth, scrHeight;
extern RenderText text;
//terry cube
extern unsigned int VBO, VAO;
//IndexedModel sphere;
extern OBJModel sphere;

inline static void setup_scene() {
	colorConfig.parseColorConfig(FPATH(resources/_config.txt));

	textShader = Shader(FPATH(resources/shaders/texture.vs), FPATH(resources/shaders/text.fs));
	solidShader = Shader(FPATH(resources/shaders/solid.vs), FPATH(resources/shaders/solid.fs));
	objectShader = Shader(FPATH(resources/shaders/object.vs), FPATH(resources/shaders/object.fs));

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// cartesian axis lines
	axisLines.setup(&camera);

	// points 
	points_setup();

	//grid
	grid.setup(&objectShader);

	// load and create a texture 
	// -------------------------
	//unsigned int texture;
	//create_texture(&texture, FPATH(resources/terry.jpg));
	textShader.use();
	//texShader.setInt("texture1", 0);

	text = RenderText(&textShader, colorConfig.pallete["text"]);

	//bool res = loadOBJ(FPATH(resources/assets/suzanne.obj), obj_vertices, uvs, normals);
	auto model = OBJModel(FPATH(resources/assets/suzanne.obj));
	//sphere = model.ToIndexedModel();
	sphere = model;

	glGenBuffers(1, &sphere.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, sphere.vbo);
	glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), &sphere.vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &sphere.face_vao);
	glBindVertexArray(sphere.face_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &sphere.face_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.face_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*sphere.face_indices.size(), &sphere.face_indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &sphere.line_vao);
	glBindVertexArray(sphere.line_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &sphere.line_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.line_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*sphere.line_indices.size(), &sphere.line_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	/*glGenBuffers(1, &sphere.line_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.line_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * sphere.line_indices.size(), &sphere.line_indices[0], GL_STATIC_DRAW);*/

	text.setCharacterSize(16 * 1.5f / scrHeight);

	perspective_projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
	orthogonal_projection = glm::ortho<float>(-float(scrWidth) / scrHeight, float(scrWidth) / scrHeight, -1, 1, -100, 100);
}

static inline void render_scene() {
	auto backgroundColor = colorConfig.pallete["background"];
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// View Projection Model matrices

	//float a = float(scrWidth)/scrHeight;
	//projection = glm::ortho(-a, a, -1.0f, 1.0f, -50.0f, 50.0f);

	view = camera.GetViewMatrix();

	// Draw grid

	grid.render(view, perspective_projection);


	//// Draw box
	//// bind textures on corresponding texture units

	//textShader.use();
	//textShader.setMat4("projection", perspective_projection);
	//textShader.setMat4("view", view);

	///*model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	//texShader.setMat4("model", model);
	//model = glm::mat4(1.0f);
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);*/

	// Draw points

	objectShader.use();
	render_points(&objectShader);

	// Draw lines
	glLineWidth(1.0f);

	objectShader.setColor("color", colorConfig.pallete["line"]);

	unsigned int VBO_element, VAO_element;
	glGenVertexArrays(1, &VAO_element);
	glBindVertexArray(VAO_element);
	glGenBuffers(1, &VBO_element);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_element);

	int elementsSize = (elements.size() % 2 == 0) ? elements.size() : elements.size() - 1;

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*elementsSize, &elements[0], GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO_element);

	glDrawArrays(GL_LINES, 0, elementsSize);


	//// render obj mesh
	objectShader.use();
	objectShader.setColor("color", colorConfig.pallete["arrow_force"]);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBindVertexArray(sphere.face_vao);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawElements(GL_TRIANGLES, sphere.face_indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(sphere.line_vao);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	objectShader.setColor("color", colorConfig.pallete["arrow_line"]);
	glDrawElements(GL_LINES, sphere.line_indices.size(), GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, sphere.positions.size());
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBindVertexArray(0);

	//render text
	textShader.use();
	//textShader.setMat4("model", Mat4(1.0f));
	//textShader.setMat4("view", Mat4(1.0f));
	textShader.setMat4("projection", orthogonal_projection);
	text.render("Sup dpt");

	// draw axis lines
	model = glm::mat4(1.0f);
	axisLines.render(solidShader, scrWidth, scrHeight);
}