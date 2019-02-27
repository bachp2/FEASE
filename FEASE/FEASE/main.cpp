﻿#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

#define GLFW_INCLUDE_GLU // for gluErrorString
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vabo.h>
#include <file_system.h>
#include "fease_draw.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

#include <camera.h>
#include <shader.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int scrWidth = SCR_WIDTH, scrHeight = SCR_HEIGHT;

ImGuiIO* imguiIO;

glm::mat4 projection, view, model;
// camera

ArcBallCamera camera(1.0f);

Grid grid;

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

int main()
{
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

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FEASE", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	ImGui_ImplGlfwGL3_Init(window, true);
	
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
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glLineWidth(1.7f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_CULL_FACE);

	////////////////////////////
	imguiIO = ImGui::GetIOPtr();
	imguiIO->Fonts->AddFontFromFileTTF(FPATH(resources/Karla-Regular.ttf), 18.0f, NULL, NULL);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	
	Shader texShader("texture.vs", "texture.fs");
	Shader solidShader("solid.vs", "solid.fs");
	Shader gridShader("grid.vs", "grid.fs");

	//cube
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

	// cartesian axis
	unsigned int VBO_axis, VAO_axis;
	glGenVertexArrays(1, &VAO_axis);
	glBindVertexArray(VAO_axis);

	glGenBuffers(1, &VBO_axis);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_axis);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertices), axis_vertices, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
	float FPS_display(0.0f);
	while (!glfwWindowShouldClose(window))
	{
		// Textbox 
		ImGui_ImplGlfwGL3_NewFrame();
		accumulate_deltaTime += deltaTime;
		if (accumulate_deltaTime > 0.08f) {
			FPS_display = imguiIO->Framerate;
			accumulate_deltaTime = 0.0f;
		}
		ImGui::Text("FPS: %.1f", FPS_display);
	
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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
		glActiveTexture(GL_TEXTURE0);
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
		Shader::reset();

		// Draw axis
		glDisable(GL_DEPTH_TEST);
		solidShader.use();
		int ww = 320;
		glViewport(scrWidth - ww + 80, -100, ww, ww);
		model = glm::rotate(model, camera.Yaw, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, camera.Pitch, glm::vec3(1.0f, 0.0f, 0.0f));

		auto view1 = glm::lookAt(glm::vec3(0.0f, 0.0f, 7.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f))*glm::inverse(model);

		solidShader.setMat4("view", view1);

		auto projection1 = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		solidShader.setMat4("projection", projection1);

		glBindVertexArray(VAO_axis);
		glDrawArrays(GL_LINES, 0, 6);
		glViewport(0, 0, scrWidth, scrHeight);
		glEnable(GL_DEPTH_TEST);
		Shader::reset();

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

	glDeleteVertexArrays(1, &VAO_axis);
	glDeleteBuffers(1, &VBO_axis);

	grid.cleanup();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	ImGui_ImplGlfwGL3_Shutdown();
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
bool firstMouse = true;
bool getHitPtFromRaycast(glm::vec3& hit, float mx, float my);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double mouseX, mouseY;
		//getting cursor position
		glfwGetCursorPos(window, &mouseX, &mouseY);
		//std::cout << "Cursor Position at (" << mouseX << " : " << mouseY << ")" << std::endl;
		glm::vec3 hit;
		getHitPtFromRaycast(hit, mouseX, mouseY);
	}
}

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

	lastX = xpos;
	lastY = ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) && !imguiIO->WantCaptureMouse)
		camera.ProcessMouseMovement(xoffset, yoffset);
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

inline static bool getHitPtFromRaycast(glm::vec3& hit, float mx, float my) {
	float x = (2.0f * mx) / scrWidth - 1.0f;
	float y = 1.0f - (2.0f * my) / scrHeight;
	float z = 1.0f;
	glm::vec3 ray_nds(x, y, z);
	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
	ray_wor = glm::normalize(ray_wor);
	printf("x: %.2f, y: %.2f, z: %.2f\n", ray_wor.x, ray_wor.y, ray_wor.z);
	return true;
}