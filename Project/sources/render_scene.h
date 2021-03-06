﻿#pragma once
#include "text.h"
#include "grad_bkgrnd.h"
#include "error.h"
#include "sphere.h"
#include "circle.h"
#include "arrow.h"
#include "cubic_bezier.h"

class ArcBallCamera;
class Shader;
extern ArcBallCamera mCamera;
extern glm::mat4 per_proj, view, model, ort_proj;

extern MouseListener mouse_listener;
extern int scrWidth, scrHeight;
extern Printer* mPrinter;
//terry cube
extern unsigned int VBO, VAO;
//IndexedModel sphere;
extern std::vector<OBJModel*> asset_container;
extern ConfigParser configTable;
//extern std::vector<GUIForm*> gui_widget_container;
extern FormContainer gui_container;
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

	Shader("bitmapped_text", SHAD(texture.vs), SHAD(text.fs));
	Shader("texture", SHAD(texture.vs), SHAD(texture.fs));
	Shader("phong_lighting", SHAD(phong.vs), SHAD(phong.fs));
	Shader("sline", SHAD(solid.vs), SHAD(solid.fs));
	Shader("object", SHAD(object.vs), SHAD(object.fs));
	Shader("billboard", SHAD(object.vs), SHAD(object.fs));
	Shader("screen", SHAD(screen.vs), SHAD(object.fs));
	
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
	
	mPrinter = new Printer(&Shader::Table["bitmapped_text"], configTable.color("text"));
	
	auto menu_bar = new MainMenu(icon_names);
	menu_bar->set_menu_items({"File", "View", "Tools"});
	gui_container.push_back((Form *)menu_bar);

	/*auto static_text = new StaticTextMessage("1234567890", 30, 52);
	gui_container.push_back((Form *) static_text);*/

	auto text_box = new TextBox(2, 50, 500, 500);
	gui_container.push_back((Form*)text_box);
	
	// cartesian axis lines
	axisLines.setup(&mCamera);

	// points 
	setup_points();

	// lines
	setup_lines();
	
	//grid
	grid.setup(&Shader::Table["object"]);
	
	// load and create a texture 
	// -------------------------
	auto textShader = Shader::Table["texture"];
	create_texture(&texture, FPATH(res/terry.jpg));
	
	//textShader->setInt("texture1", texture.tex_id);

	asset_container.push_back( new OBJModel(FPATH(res/assets/sun-dial-arrow.obj)) );
	for (auto& o : asset_container){
		o->render_setup();
	}
	asset_container[0]->translate(0.25, 0, 0.25);
	asset_container[0]->scale(0.1);

	per_proj = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
	ort_proj = glm::ortho<float>(0, scrWidth, scrHeight, 0, -100, 100);
}

static inline void render_scene() {
	//auto background = colorConfig.pallete["background"];
	//auto bot = colorConfig.pallete["background"];
	//auto top = hexCodeToRGB("#F97B13");
	//mygl_GradientBackground(bot.r, bot.g, bot.b, 1.0f, top.r, top.g, top.b, 1.0f);
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto top = Color::White();
	auto bot = Color::hex("#a8caff");
	GradientBackground(top.r, top.g, top.b, 1.0, bot.r, bot.g, bot.b, 1.0 );
	
	// View Projection Model matrices

	//float a = float(scrWidth)/scrHeight;
	//projection = glm::ortho(-a, a, -1.0f, 1.0f, -50.0f, 50.0f);

	view = mCamera.GetViewMatrix();
	
	// Draw grid
	
	grid.render();
	
	//// Draw box
	//// bind textures on corresponding texture units
	auto textShader = &Shader::Table["texture"];
	textShader->use();
	
	textShader->setMat4("projection", per_proj);
	textShader->setMat4("view", view);
	
	/*model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
	textShader->setMat4("model", model);
	model = glm::mat4(1.0f);
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture.tex_id);
	glDrawArrays(GL_TRIANGLES, 0, 36);*/

	// Draw lines
	render_lines(&Shader::Table["object"]);

	// Draw points

	render_points(&Shader::Table["object"]);

	//// render obj mesh
	auto s = Shader::Table["billboard"];
	for(auto& o : asset_container){
		o->render(&s);
	}

	//mPrinter->print_to_world("0.020", 0.2, 0.2, 0);

	//Arrow arrow;
	//static float i = 0.0;
	//auto facing_cam = -mCamera.orientation();
	//static auto original = arrow.orientation;
	//arrow.orientation = glm::slerp(original, facing_cam, abs(sin(i+=0.02)));
	//arrow.render(shaderTable.shader("object"));

	glm::vec3 cp[4] = {
		{0,0,0}, {0.5,0.5,0}, {0.4,0.2,0}, {0.8,0,0}
	};
	BezierCurve curve(cp);
	curve.render(&Shader::Table["object"]);

	// need identity matrix for model matrix
	model = glm::mat4(1.0f);

	// draw axis lines
	axisLines.render(&Shader::Table["sline"], scrWidth, scrHeight);

	gui_container.update_widgets();
	gui_container.render_widgets();
}

