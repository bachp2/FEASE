#define PRINT2F(X, Y) printf(#X ": %.2f, " #Y ": %.2f\n", X, Y);
#define PRINT3F(X, Y, Z) printf(#X ": %.2f, " #Y ": %.2f, " #Z ": %.2f\n", X, Y, Z); 


#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vabo.h>
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

#include <camera.h>
#include <shader.h>
#include <mouse_listener.h>
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int scrWidth = SCR_WIDTH, scrHeight = SCR_HEIGHT;

//ImGuiIO* imguiIO;

glm::mat4 projection, view, model;
// camera

ArcBallCamera camera(0.0f, 0.0f);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float accumulate_deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow *window);
void create_texture(unsigned int* texture);
void handleGUILogic();

inline static GLFWwindow* initApp() {
	// glfw: initialize and configure
	// ------------------------------
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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	
	glfwMakeContextCurrent(window);
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
	//imguiIO = ImGui::GetIO();
	ImGui::GetIO().Fonts->AddFontFromFileTTF(FPATH(resources/Karla-Regular.ttf), 18.0f, NULL, NULL);
	ImGui::GetStyle().WindowRounding = 2.0f;// <- Set this on init or use ImGui::PushStyleVar()
	return window;
}

int main()
{
	// glfw window creation
	// --------------------
	GLFWwindow* window = initApp();

	
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	Shader texShader("texture.vs", "texture.fs");
	Shader solidShader("solid.vs", "solid.fs");
	Shader gridShader("grid.vs", "grid.fs");
	Shader cubeShader("cube.vs", "cube.fs");

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	cubeShader.use();
	auto dotColor = hexCodeToRGB("#ea5a20");
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
	

	// Render Loop
	// -----------
	static float FPS_display(0.0f);
	static float phi = 0.0f, theta = 0.0f;
	static bool cam_is_fixed = false;
	static int normal_to = -1;
	while (!glfwWindowShouldClose(window))
	{
		// IMGUI logic
		//handleGUILogic();
		ImGui_ImplGlfw_NewFrame();
		
		accumulate_deltaTime += deltaTime;
		if (accumulate_deltaTime > 0.08f) {
			FPS_display = ImGui::GetIO().Framerate;
			accumulate_deltaTime = 0.0f;
		}

		ImGui::Begin("Toolbar");

		ImGui::Text("FPS: %.1f", FPS_display);
		ImGui::Text("Dear ImGui (%s)", IMGUI_VERSION);

		ImGui::Text("Normal to:");
		//ImGui::BulletText("Double-click on title bar to collapse window.");
		ImGui::RadioButton("x", &normal_to, 0); ImGui::SameLine();
		ImGui::RadioButton("y", &normal_to, 1); ImGui::SameLine();
		ImGui::RadioButton("z", &normal_to, 2); ImGui::SameLine();
		ImGui::RadioButton("nil", &normal_to, -1);

		if (normal_to != -1) {
			static const float phi_theta[6] = {
				0, 0, 90, 0, 0, 90
			};
			camera.rotateTo(glm::radians(phi_theta[normal_to * 2 + 1]), glm::radians(-phi_theta[normal_to * 2]));
		}

		ImGui::Checkbox("fixed cam", &cam_is_fixed);

		if (cam_is_fixed) camera.cameraState = CAM_FIXED; //allowed zooming
		else camera.cameraState = CAM_FREE;

		//ImGui::Button("run")
		ImGui::End();

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
		auto backgroundColor = hexCodeToRGB("#004883");
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw grid
		gridShader.use();

		projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
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

		// Draw point
		cubeShader.use();

		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);
		glBindVertexArray(VAO);
		for (const auto& i : nodes) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, i);
			model = glm::scale(model, glm::vec3(0.007f));
			cubeShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		model = glm::mat4(1.0f);
		Shader::reset();

		axisLines.render(solidShader, scrWidth, scrHeight);
		//Render ImGUI
		ImGui::Render();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
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
	ImGui_ImplGlfw_Shutdown();
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
		auto lim = 0.1;
		//auto r = getHitPtFromRaycastToGrid(hit, mouseX, mouseY);
		//printf("hit? %d\n", r);
		if (getHitPtFromRaycastToGrid(hit, mouseX, mouseY, lim))
		{
			//printf("x: %.2f, y: %.2f, z: %.2f\n\n", hit.x, hit.y, hit.z);
			glm::ivec2 coord(0);
			if (selectGrid(coord, hit, lim))
			{
				//printf("i: %d, j: %d\n", coord.x, coord.y);
				auto  a = nodes.insert(grid.step*Vec3(coord.x, 0.0f, coord.y));
				if (!a.second) {
					// if insert failed, select that node
					if (mouseListener.state != SELECT) {
						Element ele;
						ele.nodes[0] = &(*a.first);
						elements.push_back(ele);
						mouseListener.state = SELECT;
					}
					else {
						auto it = elements.end();
						it->nodes[1] = &(*a.first);
						mouseListener.state = NIL;
					}
					return;
				}
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
	
	if (mouseListener.draggedBy(GLFW_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
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
	ImGui_ImplGlfw_NewFrame();
	static float FPS_display(0.0f);
	static float phi = 0.0f, theta = 0.0f;
	static bool cam_is_fixed = false;
	static int normal_to = -1;
	accumulate_deltaTime += deltaTime;
	if (accumulate_deltaTime > 0.08f) {
		FPS_display = ImGui::GetIO().Framerate;
		accumulate_deltaTime = 0.0f;
	}

	ImGui::Begin("Toolbar");

	ImGui::Text("FPS: %.1f", FPS_display);
	ImGui::Text("Dear ImGui (%s)", IMGUI_VERSION);

	ImGui::Text("Normal to:");
	//ImGui::BulletText("Double-click on title bar to collapse window.");
	ImGui::RadioButton("x", &normal_to, 0); ImGui::SameLine();
	ImGui::RadioButton("y", &normal_to, 1); ImGui::SameLine();
	ImGui::RadioButton("z", &normal_to, 2); ImGui::SameLine();
	ImGui::RadioButton("nil", &normal_to, -1);

	if (normal_to != -1) {
		static const float phi_theta[6] = {
			0, 0, 90, 0, 0, 90
		};
		camera.rotateTo(glm::radians(phi_theta[normal_to * 2 + 1]), glm::radians(-phi_theta[normal_to * 2]));
	}

	ImGui::Checkbox("fixed cam", &cam_is_fixed);

	if (cam_is_fixed) camera.cameraState = CAM_FIXED; //allowed zooming
	else camera.cameraState = CAM_FREE;

	//ImGui::Button("run")
	ImGui::End();

}