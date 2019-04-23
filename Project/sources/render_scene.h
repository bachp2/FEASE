#pragma once

class ArcBallCamera;
class Shader;
extern ArcBallCamera camera;
extern glm::mat4 perspective_projection, view, model, orthogonal_projection;

extern ShaderManager shaderTable;

extern int scrWidth, scrHeight;

extern RenderText text;
//terry cube
extern unsigned int VBO, VAO;
//IndexedModel sphere;
extern OBJModel object;
extern ConfigParser configTable;
//#define STR(x) #x
inline static void setup_scene() {
	//colorConfig.parseColorConfig(FPATH(resources/_config.txt));

	configTable = ConfigParser(FPATH(resources/config.lua));

	shaderTable.emplaceShader("bitmapped_text", FPATH(resources/shaders/texture.vs), FPATH(resources/shaders/text.fs));
	shaderTable.emplaceShader("solid", FPATH(resources/shaders/solid.vs), FPATH(resources/shaders/solid.fs));
	shaderTable.emplaceShader("object", FPATH(resources/shaders/object.vs), FPATH(resources/shaders/object.fs));

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
	setup_points();

	// lines
	setup_lines();

	//grid
	grid.setup(&shaderTable);

	// load and create a texture 
	// -------------------------
	//unsigned int texture;
	//create_texture(&texture, FPATH(resources/terry.jpg));
	Shader* textShader = shaderTable.getShader("bitmapped_text");
	textShader->use();
	//textShader.setInt("texture1", 0);

	//text = RenderText();
	text = RenderText(textShader, configTable.getColor("text"));

	//bool res = loadOBJ(FPATH(resources/assets/suzanne.obj), obj_vertices, uvs, normals);
	auto model = OBJModel(FPATH(resources/assets/suzanne.obj));
	//sphere = model.ToIndexedModel();
	object = model;

	glGenBuffers(1, &object.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, object.vbo);
	glBufferData(GL_ARRAY_BUFFER, object.vertices.size() * sizeof(glm::vec3), &object.vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &object.face_vao);
	glBindVertexArray(object.face_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &object.face_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.face_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*object.face_indices.size(), &object.face_indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &object.line_vao);
	glBindVertexArray(object.line_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	if(!object.line_indices.empty())
	{
		glGenBuffers(1, &object.line_ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.line_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*object.line_indices.size(), &object.line_indices[0], GL_STATIC_DRAW);
	}
	

	glBindVertexArray(0);
	/*glGenBuffers(1, &sphere.line_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.line_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 2 * sphere.line_indices.size(), &sphere.line_indices[0], GL_STATIC_DRAW);*/

	text.setCharacterSize(16 * 1.5f / scrHeight);

	perspective_projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
	orthogonal_projection = glm::ortho<float>(-float(scrWidth) / scrHeight, float(scrWidth) / scrHeight, -1, 1, -100, 100);

}

static void GradientBackground(float top_r, float top_g, float top_b, float top_a, float bot_r, float bot_g, float bot_b, float bot_a);

static inline void render_scene() {
	//auto background = colorConfig.pallete["background"];
	//auto bot = colorConfig.pallete["background"];
	//auto top = hexCodeToRGB("#F97B13");
	//mygl_GradientBackground(bot.r, bot.g, bot.b, 1.0f, top.r, top.g, top.b, 1.0f);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GradientBackground( 0.6, 0.8, 0.3, 1.0, 0.3, 0.0, 0.6, 1.0 );

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

	// Draw lines
	render_lines(&shaderTable);

	// Draw points

	render_points(&shaderTable);

	//// render obj mesh
	object.render(&shaderTable);

	// we need identity matrix for model matrix
	model = glm::mat4(1.0f);

	//render text
	Shader* textShader = shaderTable.getShader("bitmapped_text");
	textShader->use();
	
	textShader->setMat4("model", Mat4(1.0f));
	textShader->setMat4("view", view);
	textShader->setMat4("projection", perspective_projection);
	text.render("Sleep Deprived");

	text.writeBitmap(0, 0, "Sleep Deprived");
	// draw axis lines
	axisLines.render(&shaderTable, scrWidth, scrHeight);
}

#define SHADER_HEADER "#version 330 core\n"
#define SHADER_STR(x) #x
inline static void GradientBackground( float top_r, float top_g, float top_b, float top_a, 
	float bot_r, float bot_g, float bot_b, float bot_a )
{
	glDisable(GL_DEPTH_TEST);

	static GLuint background_vao = 0;
	static GLuint background_shader = 0;

	if (background_vao == 0)
	{
		glGenVertexArrays(1, &background_vao);

		const char* vs_src = (const char*) SHADER_HEADER SHADER_STR
			(
				out vec2 v_uv;
				void main()
				{
					uint idx = uint(gl_VertexID);
					gl_Position = vec4( idx & 1U, idx >> 1U, 0.0, 0.5 ) * 4.0 - 1.0;
					v_uv = vec2( gl_Position.xy * 0.5 + 0.5 );
				}
		);

		const char* fs_src = (const char*) SHADER_HEADER SHADER_STR
			(
				uniform vec4 top_color;
				uniform vec4 bot_color;
				in vec2 v_uv;
				out vec4 frag_color;

				void main()
				{
					frag_color = bot_color * (1 - v_uv.y) + top_color * v_uv.y;
				}
		);
		GLuint vs_id, fs_id;
		vs_id = glCreateShader( GL_VERTEX_SHADER );
		fs_id = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource(vs_id, 1, &vs_src, NULL);
		glShaderSource(fs_id, 1, &fs_src, NULL);
		glCompileShader(vs_id);
		glCompileShader(fs_id);
		background_shader = glCreateProgram();
		glAttachShader( background_shader, vs_id );
		glAttachShader( background_shader, fs_id );
		glLinkProgram(  background_shader );
		glDetachShader( background_shader, fs_id );
		glDetachShader( background_shader, vs_id );
		glDeleteShader( fs_id );
		glDeleteShader( vs_id );
		glUseProgram( background_shader );
	}

	glUseProgram( background_shader );
	GLuint top_color_loc = glGetUniformLocation( background_shader, "top_color" );
	GLuint bot_color_loc = glGetUniformLocation( background_shader, "bot_color" );
	glUniform4f( top_color_loc, top_r, top_g, top_b, top_a );
	glUniform4f( bot_color_loc, bot_r, bot_g, bot_b, bot_a );

	glBindVertexArray( background_vao );
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
}

