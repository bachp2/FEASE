#pragma once
#include "text.h"
#include "grad_bkgrnd.h"
class ArcBallCamera;
class Shader;
extern ArcBallCamera camera;
extern glm::mat4 perspective_projection, view, model, orthogonal_projection;

extern ShaderManager shaderTable;
extern MouseListener mouse_listener;
extern int scrWidth, scrHeight;
extern TextPainter* text_painter;
//terry cube
extern unsigned int VBO, VAO;
//IndexedModel sphere;
extern std::vector<OBJModel*> obj_model_container;
extern ConfigParser configTable;
//extern std::vector<GUIForm*> gui_widget_container;
extern FormContainer gui_container;
//#define STR(x) #x
Texture texture;

static const std::vector<std::string> icon_names = {
	"document_new",
	"document-save",
	"separator",
	"edit-copy",
	"edit-cut",
	"edit-redo",
	"edit-undo",
	"separator",
	"truss",
	"node",
	"media-playback-start"
};

inline static void setup_scene() {
	configTable = ConfigParser(FPATH(res/config.lua));

	//configTable.initialize_shader_program_from_config(&shaderTable, FPATH(resources/config.lua));

	shaderTable.emplaceShader("bitmapped_text", SHAD(texture.vs), SHAD(text.fs));
	shaderTable.emplaceShader("texture", SHAD(texture.vs), SHAD(texture.fs));
	shaderTable.emplaceShader("solid", SHAD(solid.vs), SHAD(solid.fs));
	shaderTable.emplaceShader("object", SHAD(object.vs), SHAD(object.fs));
	shaderTable.emplaceShader("2D", SHAD(_2dobject.vs), SHAD(object.fs));
	
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

	text_painter = new TextPainter(shaderTable.getShader("bitmapped_text"), configTable.color("text"));
	/*auto tw = new GUIForm(15, 50, 100, 100);
	gui_widget_container.push_back(tw);*/
	
	auto menu_bar = new MainMenu(icon_names);
	menu_bar->setPainter(text_painter);
	menu_bar->set_menu_items({"File", "Edit", "Tools"});
	gui_container.push_back((Form *)menu_bar);

	auto text_box = new cHelpText(30, 50, 400, 400);
	text_box->setPainter(text_painter);
	text_box->include_text("1234567890");
	gui_container.push_back((Form *) text_box);
	
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
	create_texture(&texture, FPATH(res/terry.jpg));
	
	textShader->setInt("texture1", 0);

	//text = RenderText();
	

	//bool res = loadOBJ(FPATH(resources/assets/suzanne.obj), obj_vertices, uvs, normals);
	obj_model_container.push_back( new OBJModel(FPATH(res/assets/suzanne.obj)) );
	//sphere = model.ToIndexedModel();
	for (auto& o : obj_model_container){
		o->render_setup();
	}

	perspective_projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
	orthogonal_projection = glm::ortho<float>(0, scrWidth, scrHeight, 0, -100, 100);
}

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
	/*for(auto& o : obj_model_container){
		o->render(&shaderTable);
	}*/

	// need identity matrix for model matrix
	model = glm::mat4(1.0f);

	//text.render("Sleep Deprived");

	//text_painter->writeBitmap("The GLFW_CURSOR input mode provides several cursor modes for special forms of mouse motion input. By default, the cursor mode is ", 8, 300);
	//text_painter->writeBitmap("Carole & Tuesday", 0, 300+text_painter->get_font_line_gap());

	// draw axis lines
	axisLines.render(&shaderTable, scrWidth, scrHeight);

	gui_container.update_widgets();
	gui_container.render_widgets();
}

