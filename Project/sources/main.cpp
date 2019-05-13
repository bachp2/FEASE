
extern "C"
{
#include <lua535\include\lua.h>
#include <lua535\include\lauxlib.h>
#include <lua535\include\lualib.h>
}

#ifdef _WIN32
#pragma comment(lib, "lua535/liblua53.a")
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <obj_loader.h>
#include <file_system.h>
#include "fease_draw.h"
#include "text_render.h"
#include "shader_manager.h"
#include <iostream>
#include "custom_gui_widgets.h"
#include "render_scene.h"
#include "config_parser.h"
#include "fe_structs.h"

#define GLFW_INCLUDE_GLU // for gluErrorString
#include <GLFW/glfw3.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#include <camera.h>
#include <shader.h>
#include "mouse_listener.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int scrWidth = SCR_WIDTH, scrHeight = SCR_HEIGHT;
float deltaTime, lastFrame;
//ImGuiIO* imguiIO;

glm::mat4 perspective_projection, view, model, orthogonal_projection;

// camera

ArcBallCamera camera(glm::radians(-30.0f), glm::radians(20.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

//Shader textShader, solidShader, objectShader;
ShaderManager shaderTable;
ConfigParser configTable;
TextPainter text_painter;

std::vector<GUIForm*> gui_widget_container;
unsigned int VBO, VAO;
std::vector<OBJModel*> obj_model_container;
FEObject fe;
MouseListener mouse_event_listener;
void processInput(GLFWwindow *window);
void render_scene();
void setup_scene();
GLFWwindow* initApp();

std::vector< glm::vec3 > obj_vertices;

int main(int, char**)
{
	
	GLFWwindow* window = initApp();
	// set up scene
	// ------------------------------------------------------------------
	setup_scene();
	

	fe.fNodes.push_back(Eigen::Vector2f(0, 0));
	fe.fNodes.push_back(Eigen::Vector2f(10, 0));
	fe.fNodes.push_back(Eigen::Vector2f(10, 10));

	// Render Loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// Per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Input
		// -----
		processInput(window);

		// Render Scene
		// ------
		render_scene();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_point);
	//glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_point);
	axisLines.cleanup();
	grid.cleanup();
	for(auto& o : obj_model_container){
		delete o;
	}
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
static inline void processInput(GLFWwindow *window)
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
static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glfwGetWindowSize(window, &scrWidth, &scrHeight);
	perspective_projection = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
	orthogonal_projection = glm::ortho<float>(0, scrWidth, scrHeight, 0, -100, 100);
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------

bool getHitPtFromRaycastToGrid(glm::vec3& hit, float mx, float my, float lim);
bool selectGrid(glm::ivec2& coord, const glm::vec3& hit, float lim);

void inline mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		mouse_event_listener.button = button;
		mouse_event_listener.state = LIMBO;
	}

	if (action == GLFW_RELEASE && mouse_event_listener.state == LIMBO) {
		mouse_event_listener.state = CLICK;
		//printf("clicked!\n");
		mouse_event_listener.flag = true;
		for(auto& w : gui_widget_container){
			w->draggable = false;
		}
	} 
	else if (action == GLFW_RELEASE && mouse_event_listener.state == DRAG) mouse_event_listener.flag = true;

	if (mouse_event_listener.clickedBy(GLFW_MOUSE_BUTTON_LEFT))
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

		if (mouse_event_listener.agenda == SELECT_NODE && getHitPtFromRaycastToGrid(hit, mouseX, mouseY, lim)) {
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

		if (mouse_event_listener.agenda == ADD_NODE && getHitPtFromRaycastToGrid(hit, mouseX, mouseY, lim))
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

	if (mouse_event_listener.flag) mouse_event_listener.flag = false;
	if (mouse_event_listener.state != LIMBO) mouse_event_listener.resetState();
	//printf("mouse flag %d in mouse action callback\n", mouseListener.flag);
	
	double mouseX, mouseY;
	//getting cursor position
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//for(auto& w : gui_widget_container)
	//{
	//	if(w->isHover(mouseX, mouseY)){
	//		w->moveable = true;
	//		//printf("movealbe");
	//	}

	//	if(action == GLFW_RELEASE) {
	//		w->moveable = false;
	//	}
	//}
}

bool firstMouse = true;
static inline void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	
	mouse_event_listener._cx = xpos;
	mouse_event_listener._cy = ypos;
	mouse_event_listener._dx = xoffset;
	mouse_event_listener._dy = yoffset;

	if (mouse_event_listener.flag) {
		mouse_event_listener.state = NIL;
		mouse_event_listener.flag = false;
		//printf("staring at the abyss...\n");
	}

	if (mouse_event_listener.state == LIMBO) {
		mouse_event_listener.state = DRAG;
		//printf("dragging a dead mouse!\n");
	}

	lastX = xpos;
	lastY = ypos;
	
	if (mouse_event_listener.draggedBy(GLFW_MOUSE_BUTTON_MIDDLE))
		camera.ProcessMouseMovement(xoffset, yoffset);
	
	/*for(auto& w : gui_widget_container){
		if(w->moveable && mouse_event_listener.draggedBy(GLFW_MOUSE_BUTTON_LEFT)){
			w->move(xoffset, -yoffset);
		}
	}*/
	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
static inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

inline static bool getHitPtFromRaycastToGrid(glm::vec3& hit, float mx, float my, float lim) {
	float x = (2.0f * mx) / scrWidth - 1.0f;
	float y = 1.0f - (2.0f * my) / scrHeight;
	float z = 1.0f;
	glm::vec3 ray_nds(x, y, z);
	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(perspective_projection) * ray_clip;
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

inline static void glfw_error_callback(int error, const char* description)
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

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FEASE :: Finite Element Analysis Sandbox for Engineers", NULL, NULL);
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

	// set callback functions glfw
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);

	return window;
}

