#pragma once
#include "text_render.h"
class ArcBallCamera;
class Shader;
extern ArcBallCamera camera;
extern glm::mat4 perspective_projection, view, model, orthogonal_projection;

extern ShaderManager shaderTable;
extern MouseListener mouse_event_listener;
extern int scrWidth, scrHeight;
extern TextureQuad tq;
extern std::vector<TextureQuad> gui_icons;
extern TextPainter* text_painter;
//terry cube
extern unsigned int VBO, VAO;
//IndexedModel sphere;
extern std::vector<OBJModel*> obj_model_container;
extern ConfigParser configTable;
//extern std::vector<GUIForm*> gui_widget_container;
extern WidgetContainer gui_widget_container;
//#define STR(x) #x
Texture texture;
inline static void setup_scene() {
	//colorConfig.parseColorConfig(FPATH(resources/_config.txt));

	configTable = ConfigParser(FPATH(resources/config.lua));

	shaderTable.emplaceShader("bitmapped_text", FPATH(resources/shaders/texture.vs), FPATH(resources/shaders/text.fs));
	shaderTable.emplaceShader("texture", FPATH(resources/shaders/texture.vs), FPATH(resources/shaders/texture.fs));
	shaderTable.emplaceShader("solid", FPATH(resources/shaders/solid.vs), FPATH(resources/shaders/solid.fs));
	shaderTable.emplaceShader("object", FPATH(resources/shaders/object.vs), FPATH(resources/shaders/object.fs));
	shaderTable.emplaceShader("2D", FPATH(resources/shaders/_2dobject.vs), FPATH(resources/shaders/object.fs));

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

	text_painter = new TextPainter(shaderTable.getShader("bitmapped_text"), configTable.getColor("text"));

	/*auto tw = new GUIForm(15, 50, 100, 100);
	gui_widget_container.push_back(tw);*/
	
	auto menu_bar = new cMainMenuBar();
	menu_bar->setPainter(text_painter);
	menu_bar->set_menu_items({"File", "Edit", "Tools"});
	gui_widget_container.push_back((GUIForm *)menu_bar);

	auto text_box = new cHelpText(30, 50, 200, 200);
	text_box->setPainter(text_painter);
	text_box->include_text("The quick brown fox");
	gui_widget_container.push_back((GUIForm *) text_box);
	
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
	auto textShader = shaderTable.getShader("texture");
	create_texture(&texture, FPATH(resources/terry.jpg));
	
	textShader->setInt("texture1", 0);

	//text = RenderText();
	

	//bool res = loadOBJ(FPATH(resources/assets/suzanne.obj), obj_vertices, uvs, normals);
	obj_model_container.push_back( new OBJModel(FPATH(resources/assets/suzanne.obj)) );
	//sphere = model.ToIndexedModel();
	for (auto& o : obj_model_container){
		o->render_setup();
	}

	std::vector<std::string> icon_names = {
		"document-new",
		"document-save",
		"edit-copy",
		"edit-cut",
		"edit-redo",
		"edit-undo"
	};

	gui_icons.reserve(10);
	for (int size = 22, xx = 500, i = 0; i < icon_names.size(); i++) {
		std::string path = FPATH(resources/gui_icons/);
		TextureQuad tq;
		tq = TextureQuad(xx, 100, size, size);
		tq.set_texture_ptr(new Texture(path + icon_names[i] + ".png", true));
		//printf("a\n");
		gui_icons.push_back(tq);
		//printf("b\n");
		xx += size;
	}

	for (int i = 0; i < icon_names.size(); i++){
		gui_icons[i].print_texture_ptr();
	}

	/*tq = TextureQuad(500, 100, 22, 22);
	tq.set_texture_ptr(new Texture(FPATH(resources/gui_icons/edit-undo.png), true));*/
	//tq.set_texture_ptr(new Texture(FPATH(resources/terry.jpg), true));
	//gui_icons.push_back(tq);

	perspective_projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
	orthogonal_projection = glm::ortho<float>(0, scrWidth, scrHeight, 0, -100, 100);
}

static void GradientBackground(float top_r, float top_g, float top_b, float top_a, float bot_r, float bot_g, float bot_b, float bot_a);

static inline void render_scene() {
	//auto background = colorConfig.pallete["background"];
	//auto bot = colorConfig.pallete["background"];
	//auto top = hexCodeToRGB("#F97B13");
	//mygl_GradientBackground(bot.r, bot.g, bot.b, 1.0f, top.r, top.g, top.b, 1.0f);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto top = hexCodeToRGB("#42b0ce");
	auto bot = hexCodeToRGB("#002530");
	GradientBackground(top.r, top.g, top.b, 1.0, bot.r, bot.g, bot.b, 1.0 );

	// View Projection Model matrices

	//float a = float(scrWidth)/scrHeight;
	//projection = glm::ortho(-a, a, -1.0f, 1.0f, -50.0f, 50.0f);

	view = camera.GetViewMatrix();

	// Draw grid

	grid.render(view, perspective_projection);


	//// Draw box
	//// bind textures on corresponding texture units
	auto textShader = shaderTable.getShader("texture");
	textShader->use();
	textShader->setMat4("projection", perspective_projection);
	textShader->setMat4("view", view);

	/*model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	textShader->setMat4("model", model);
	model = glm::mat4(1.0f);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture.tex_id);
	glDrawArrays(GL_TRIANGLES, 0, 36);*/

	// Draw lines
	render_lines(&shaderTable);

	// Draw points

	render_points(&shaderTable);

	//// render obj mesh
	for(auto& o : obj_model_container){
		o->render(&shaderTable);
	}

	// need identity matrix for model matrix
	model = glm::mat4(1.0f);

	//text.render("Sleep Deprived");

	//text_painter->writeBitmap("The GLFW_CURSOR input mode provides several cursor modes for special forms of mouse motion input. By default, the cursor mode is ", 8, 300);
	//text_painter->writeBitmap("Carole & Tuesday", 0, 300+text_painter->get_font_line_gap());

	// draw axis lines
	axisLines.render(&shaderTable, scrWidth, scrHeight);

	gui_widget_container.update_widgets();
	gui_widget_container.render_widgets();

	for(auto &a : gui_icons){
		a.render(shaderTable.getShader("texture"));
	}
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

