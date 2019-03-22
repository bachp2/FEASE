#define PRINT2F(X, Y) printf(#X ": %.2f, " #Y ": %.2f\n", X, Y);
#define PRINT3F(X, Y, Z) printf(#X ": %.2f, " #Y ": %.2f, " #Z ": %.2f\n", X, Y, Z); 

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <obj_loader.h>
#include <file_system.h>
#include "fease_draw.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>


#define GLFW_INCLUDE_GLU // for gluErrorString
#include <GLFW/glfw3.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <camera.h>
#include <shader.h>
#include "mouse_listener.h"


ImGuiIO* imguiIO;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int scrWidth = SCR_WIDTH, scrHeight = SCR_HEIGHT;

//ImGuiIO* imguiIO;

glm::mat4 projection, view, model;
// camera

ArcBallCamera camera(glm::radians(-30.0f), glm::radians(20.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float accumulate_deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow *window);
void create_texture(unsigned int* texture);
void handleGUILogic();

GLFWwindow* initApp();

int main(int, char**)
{
	GLFWwindow* window = initApp();
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	colorConfig.parseColorConfig(FPATH(resources/_config.txt));

	Shader texShader("texture.vs", "texture.fs");
	Shader solidShader("solid.vs", "solid.fs");
	Shader gridShader("grid.vs", "grid.fs");
	Shader cubeShader("object.vs", "object.fs");

	//terry cube
	unsigned int VBO, VAO;
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

	// point

	unsigned int VBO_point;
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_point);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_point);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	cubeShader.use();
	auto dotColor = colorConfig.pallete["dot"];
	cubeShader.setVec3("dotColor", glm::vec3(dotColor.r, dotColor.g, dotColor.b));
	
	//grid
	grid.setup(&gridShader, scrWidth, scrHeight);
	
	// load and create a texture 
	// -------------------------
	unsigned int texture;
	create_texture(&texture);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------


	texShader.use();
	texShader.setInt("texture1", 0);
	
	/*elements.push_back(Vec3(0.0f, 0.0f, 0.0f));
	elements.push_back(Vec3(0.6f, 1.2f, 4.0f));
	elements.push_back(Vec3(1.0f, 0.0f, 0.0f));
	elements.push_back(Vec3(0.0f, 5.0f, 0.0f));
	elements.push_back(Vec3(0.0f, 2.0f, 0.0f));
	elements.push_back(Vec3(0.6f, 0.0f, 0.5f));*/
	// Render Loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		handleGUILogic();
		// Per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		// -----
		processInput(window);

		// Render
		// ------
		auto backgroundColor = colorConfig.pallete["background"];
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw grid
		gridShader.use();
		//float a = float(scrWidth)/scrHeight;
		//projection = glm::ortho(-a, a, -1.0f, 1.0f, -1000.0f, 1000.0f);
		
		projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
		//glm::vec3 eye = glm::vec3(1.0f);

		//projection = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		
		gridShader.setMat4("projection", projection);
		gridShader.setMat4("view", view);

		grid.render();

		Shader::reset();

		// Draw box
		// bind textures on corresponding texture units
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		texShader.use();
		texShader.setMat4("projection", projection);
		texShader.setMat4("view", view);
		
		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		texShader.setMat4("model", model);
		model = glm::mat4(1.0f);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		Shader::reset();*/

		// Draw points
		cubeShader.use();
		//view = camera.getDefaultViewMatrix();

		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		glBindVertexArray(VAO);
		for (auto& i : nodes) {
			/*i.x += 0.5;
			i.y += 0.5;*/
			model = glm::mat4(1.0f);
			
			model = glm::translate(model, i);
			model = glm::scale(model, glm::vec3(0.01f));

			cubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Draw lines
		
		cubeShader.setMat4("model", glm::mat4(1.0f));
		glLineWidth(5.0f);
		if (!elements.empty() && elements.size() % 2 == 0)
		{
			/*for (auto e : elements) {
				PRINT3F(e.x, e.y, e.z);
			}*/
			unsigned int VBO_element, VAO_element;
			glGenVertexArrays(1, &VAO_element);
			glBindVertexArray(VAO_element);
			glGenBuffers(1, &VBO_element);
			glBindBuffer(GL_ARRAY_BUFFER, VBO_element);
			if(elements.size() % 2 == 0)
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*elements.size(), &elements[0], GL_DYNAMIC_DRAW);
			else
				glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*(elements.size()-1), &elements[0], GL_DYNAMIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(VAO_element);

			if(elements.size() % 2 == 0)
				glDrawArrays(GL_LINES, 0, elements.size());
			else
				glDrawArrays(GL_LINES, 0, elements.size() - 1);
		}
		glLineWidth(1.7f);
		Shader::reset();

		model = glm::mat4(1.0f);
		axisLines.render(solidShader, scrWidth, scrHeight);

		//Render ImGUI
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_point);
	axisLines.cleanup();
	grid.cleanup();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glfwGetWindowSize(window, &scrWidth, &scrHeight);
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------

bool getHitPtFromRaycastToGrid(glm::vec3& hit, float mx, float my, float lim);
bool selectGrid(glm::ivec2& coord, const glm::vec3& hit, float lim);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		mouseListener.button = button;
		mouseListener.state = LIMBO;
	}

	if (action == GLFW_RELEASE && mouseListener.state == LIMBO) {
		mouseListener.state = CLICK;
		//printf("clicked!\n");
		mouseListener.flag = true;
	} 
	else if (action == GLFW_RELEASE && mouseListener.state == DRAG) mouseListener.flag = true;

	if (mouseListener.clickedBy(GLFW_MOUSE_BUTTON_LEFT))
	{
		double mouseX, mouseY;
		//getting cursor position
		glfwGetCursorPos(window, &mouseX, &mouseY);
		//std::cout << "Cursor Position at (" << mouseX << " : " << mouseY << ")" << std::endl;
		glm::vec3 hit;
		auto lim = 0.3;
		//auto r = getHitPtFromRaycastToGrid(hit, mouseX, mouseY);
		//printf("hit? %d\n", r);
		//assert(mouseListener.agenda == SELECT_NODE);
		if (mouseListener.agenda == SELECT_NODE && getHitPtFromRaycastToGrid(hit, mouseX, mouseY, lim)) {
			//printf("select node success");
			glm::ivec2 coord(0);
			if (selectGrid(coord, hit, lim))
			{
				//printf("i: %d, j: %d\n", coord.x, coord.y);
				auto n = vector_find(nodes, grid.step*Vec3(coord.x, 0.0f, coord.y));
				if (n!=nullptr)
				{
					//printf("tes!!");
					insert_by_unique_pair(elements, n);
				}

			}
		}

		if (mouseListener.agenda == ADD_NODE && getHitPtFromRaycastToGrid(hit, mouseX, mouseY, lim))
		{
			//printf("x: %.2f, y: %.2f, z: %.2f\n\n", hit.x, hit.y, hit.z);
			//printf("add node success");
			glm::ivec2 coord(0);
			if (selectGrid(coord, hit, lim))
			{
				printf("i: %d, j: %d\n", coord.x, coord.y);
				vector_insert(nodes, grid.step*Vec3(coord.x, 0.0f, coord.y));
			}
		}

	}

	if (mouseListener.flag) mouseListener.flag = false;
	if (mouseListener.state != LIMBO) mouseListener.resetState();
	//printf("mouse flag %d in mouse action callback\n", mouseListener.flag);
}

bool firstMouse = true;
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	
	if (mouseListener.flag) {
		mouseListener.state = NIL;
		mouseListener.flag = false;
		//printf("staring at the abyss...\n");
	}

	if (mouseListener.state == LIMBO) {
		mouseListener.state = DRAG;
		//printf("dragging a dead mouse!\n");
	}

	lastX = xpos;
	lastY = ypos;
	
	if (mouseListener.draggedBy(GLFW_MOUSE_BUTTON_MIDDLE) && !ImGui::GetIO().WantCaptureMouse)
		camera.ProcessMouseMovement(xoffset, yoffset);
	//printf("mouse flag %d in mouse position callback\n", mouseListener.flag);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

static void create_texture(unsigned int* texture) {
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	int width, height, nrChannels;
	unsigned char *data = stbi_load(FPATH(resources/terry.jpg), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

inline static bool getHitPtFromRaycastToGrid(glm::vec3& hit, float mx, float my, float lim) {
	float x = (2.0f * mx) / scrWidth - 1.0f;
	float y = 1.0f - (2.0f * my) / scrHeight;
	float z = 1.0f;
	glm::vec3 ray_nds(x, y, z);
	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
	ray_wor = glm::normalize(ray_wor);

	//printf("x: %.2f, y: %.2f, z: %.2f\n", ray_wor.x, ray_wor.y, ray_wor.z);
	auto camPos = camera.getPosition();
	//printf("radius: %.2f ", camera.Radius);
	//printf("x: %.2f, y: %.2f, z: %.2f\n", camPos.x, camPos.y, camPos.z);
	// find t
	auto t = -camPos.y / ray_wor.y;
	if (t >= -0.00001f) {
		hit = camPos + t*ray_wor;
		hit.y = 0.0f;
		//printf("x: %.2f, y: %.2f, z: %.2f\n\n", hit.x, hit.y, hit.z);
		auto grid_halft_size_after_scaling = 1.0f / camera.Zoom / 2;
		if (hit.x > grid_halft_size_after_scaling + lim || hit.x < -grid_halft_size_after_scaling - lim) return false;
		if (hit.z > grid_halft_size_after_scaling + lim || hit.z < -grid_halft_size_after_scaling - lim) return false;
		return true;
	}
	return false;
}

bool numCloseWithin(float num, float lim);
inline static bool selectGrid(glm::ivec2& coord, const glm::vec3& hit, float lim)
{
	auto grid_halft_size_after_scaling = 1.0f / camera.Zoom / 2;
	auto grid_step = grid_halft_size_after_scaling * 2 / grid.gnum;
	auto a = hit.x / grid_step;
	auto b = hit.z / grid_step;
	//PRINT2F(a, b);
	//auto halfgrid = grid_step / 2;
	if (!numCloseWithin(a, lim) || !numCloseWithin(b, lim))
		return false; //can't select a grid coordinate near its vicinity
	a = round(a);
	b = round(b);
	//printf("a: %.2f, b: %.2f\n", a, b);
	coord = glm::ivec2(a, b);
	return true;
}

inline static bool numCloseWithin(float num, float lim) {
	auto center = round(num);
	if (num >= center - lim && num <= center + lim) return true;
	return false;
}

inline static void handleGUILogic() 
{
	static float FPS_display(0.0f);
	static float phi = 0.0f, theta = 0.0f;
	static bool cam_is_fixed = false;
	static int normal_to = -1;
	static int radio_select_once = -1;
	static int mouse_agenda = ADD_NODE;
	// Start the Dear ImGui frame
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	accumulate_deltaTime += deltaTime;
	if (accumulate_deltaTime > 0.08f) {
		FPS_display = ImGui::GetIO().Framerate;
		accumulate_deltaTime = 0.0f;
	}

	ImGui::Begin("Toolbar");

	ImGui::Text("FPS: %.1f", FPS_display);
	ImGui::Text("Dear ImGui (%s)", IMGUI_VERSION);

	ImGui::Text("Normal to:");
	ImGui::RadioButton("x", &normal_to, 0); ImGui::SameLine();
	ImGui::RadioButton("y", &normal_to, 1); ImGui::SameLine();
	ImGui::RadioButton("z", &normal_to, 2);

	if (normal_to != radio_select_once) {
		radio_select_once = normal_to;
		static const float phi_theta[6] = {
			0, 0, 90, 0, 0, 90
		};
		camera.rotateTo(glm::radians(phi_theta[normal_to * 2 + 1]), glm::radians(-phi_theta[normal_to * 2]));
	}

	ImGui::Checkbox("lock camera to view", &cam_is_fixed);
	if (cam_is_fixed) camera.cameraState = CAM_FIXED; //allowed zooming
	else camera.cameraState = CAM_FREE;

	ImGui::RadioButton("Add Node", &mouse_agenda, ADD_NODE);
	ImGui::RadioButton("Connect Ele.", &mouse_agenda, CONNECT_ELE);
	ImGui::RadioButton("Select Node", &mouse_agenda, SELECT_NODE);

	mouseListener.agenda = Mouse_Agenda(mouse_agenda);

	ImGui::Text("element count: %d", elements.size());
	ImGui::Text("node count: %d", nodes.size());

	////ImGui::Button("run")
	ImGui::End();

}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

inline static GLFWwindow* initApp() {
	// glfw window creation
	// --------------------
	//GLFWwindow* window = initApp();
	// glfw: initialize and configure
	// ------------------------------
	glfwSetErrorCallback(glfw_error_callback);
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FEASE", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	bool err = gladLoadGL() == 0;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	imguiIO = ImGui::GetIOPtr();

	// Setup Dear ImGui style
	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// set callback functions glfw
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// configure global opengl state
	// -----------------------------
	glLineWidth(1.7f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);

	////////////////////////////
	ImGui::GetIO().Fonts->AddFontFromFileTTF(FPATH(resources/Karla-Regular.ttf), 12.0f, NULL, NULL);
	ImGui::GetStyle().WindowRounding = 2.0f;// <- Set this on init or use ImGui::PushStyleVar()
	return window;
}

//#version 330 core
//uniform int divisions;
//uniform lowp float thickness;
//uniform vec3 gridColor;
//
//in vec2 vUV;
//
//void main() {
	//// multiplicationFactor scales the number of stripes
	//vec2 t = vUV *divisions;

	//// the threshold constant defines the with of the lines
	//if (abs(t.x - round(t.x)) <= thickness || abs(t.y - round(t.y)) < thickness)
	//	gl_FragColor = vec4(gridColor, 1.0);
	//else
	//	discard;
//}

//float x = fract(vUV.x * divisions);
//x = min(x, 1.0 - x);
//
//float xdelta = fwidth(x);
//x = smoothstep(x - xdelta, x + xdelta, thickness);
//
//float y = fract(vUV.y * divisions);
//y = min(y, 1.0 - y);
//
//float ydelta = fwidth(y);
//y = smoothstep(y - ydelta, y + ydelta, thickness);
//
//float r = clamp(x + y, backgroundColor.x, gridColor.x);
//float g = clamp(x + y, backgroundColor.y, gridColor.y);
//float b = clamp(x + y, backgroundColor.z, gridColor.z);
//gl_FragColor = vec4(r, g, b, 1.0);

//#version 330 core
//#extension GL_OES_standard_derivatives : enable
//
//precision highp float;
//
//uniform int multiplicationFactor;
//uniform float threshold;
//uniform vec4 gridColor;
//uniform vec2 resolution;  // width and height of the viewport
//						  //uniform float minthres = 0.000002;
//						  //uniform float maxthres = 0.00005;
//in vec2 vUV;
//
//void main() {
//	//float m   = float(multiplicationFactor);
//	vec2 t = vUV * multiplicationFactor;
//	vec2 dist = abs(fract(t - 0.5) - 0.5) / resolution;
//	vec2 th = threshold / resolution;
//
//	//th = clamp(th, minthres, maxthres);
//
//	if (dist.x > th.x  && dist.y > th.y)
//		discard;
//	//gl_FragColor = vec4(vec3(1.0 - min(line, 1.0)), 1.0);
//	gl_FragColor = gridColor;
//}