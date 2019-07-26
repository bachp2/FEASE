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

#include "obj_loader.h"
#include "fease_draw.h"
#include "gui.h"
#include "render_scene.h"
#include "config_parser.h"
#include "bm_parser.h"

#define GLFW_INCLUDE_GLU // for gluErrorString
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <chrono>
#include <thread>

#include <camera.h>
#include <shader.h>
#include "mouse_listener.h"

#pragma warning (disable: 4838)

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int scrWidth = SCR_WIDTH, scrHeight = SCR_HEIGHT;
float deltaTime{0}, lastFrame;

glm::mat4 per_proj, view, model, ort_proj;
GLFWwindow* window;
std::thread* console_thread = nullptr;
std::vector< glm::vec3 > obj_vertices;
bool window_resized = false;
// camera

ArcBallCamera mCamera(glm::radians(-30.0f), glm::radians(20.0f));
ShaderManager shaderTable;
ConfigParser configTable;
Printer* mPrinter;

FormContainer gui_container;
unsigned int VBO, VAO;
std::vector<OBJModel*> asset_container;
MouseListener mouse_listener;

void run_data_analysis();
void render_loop();
void processInput(GLFWwindow *window);
void render_scene();
void setup_scene();
void prog_cleanup();
GLFWwindow* prog_init();

int main(int, char**)
{
	window = prog_init();
	// set up scene
	// ------------------------------------------------------------------
	
	setup_scene();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwMakeContextCurrent(NULL);
	
	std::thread main_renderer(render_loop);
	
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	while(!glfwWindowShouldClose(window)){
		// Per-frame time logic
		// --------------------
		if (mouse_listener.agenda == MouseListener::Agenda::RUN_ANALYSIS && mouse_listener.state == MouseListener::State::NIL) {
			// if condition allows one click only
			run_data_analysis();
			mouse_listener.agenda = MouseListener::Agenda::ADD_NODE;
		}

		// Input
		// -----
		processInput(window);
		glfwWaitEvents();
		
	}
	// to do tidy this up, hidden state everywhere
	if(console_thread) {
		console_thread->join();
		delete console_thread;
	}
	main_renderer.join();
	prog_cleanup();
	//gui_widget_container.~WidgetContainer();
	return 0;
}

void inline static render_loop(){
	// Render Loop
	// ------------------------
	glfwMakeContextCurrent(window);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	// https://stackoverflow.com/questions/54893063/glew-causes-screen-to-flicker-between-red-and-black-glad-works
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	
	glfwSwapInterval(1);
	auto const start_time = std::chrono::steady_clock::now();
	auto const wait_time = std::chrono::milliseconds{ 17 };
	auto next_time = start_time + wait_time;

	while (!glfwWindowShouldClose(window))
	{
		// execute once after thread wakes up every 17ms which is theoretically 60 frames per second
		//auto then = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_until(next_time);

		if (window_resized) {
			per_proj = glm::perspective(glm::radians(45.0f), (float)scrWidth / (float)scrHeight, 0.1f, 100.0f);
			ort_proj = glm::ortho<float>(0, float(scrWidth), float(scrHeight), 0, -100, 100);
			gui_container.resize_widgets();
			glViewport(0, 0, scrWidth, scrHeight);
			window_resized = false;
		}

		// Render Scene
		// ------

		/*if(mouse_listener.left_click()){
			mouse_listener.agenda == MouseListener::ADD_NODE;
		}*/

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glClear(GL_COLOR_BUFFER_BIT);
		render_scene();
		glfwSwapBuffers(window);

		/*auto elasped_time =
			std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::high_resolution_clock::now() - then);
		std::cout << "seconds: " << elasped_time.count() << '\n';*/
		next_time += wait_time;
	}
	
	glfwMakeContextCurrent(NULL);
	//std::terminate();
}

void console_input_loop();
static inline void run_data_analysis()
{
	printf("Running truss analysis...\n");
	printf("Geometry definition stage...\n");

	printf("Truss node count %d\n", nodes.size());
	printf("Truss element count %d\n", elements.size());
	printf("Writing to lua script...");
	std::string content = "require(\"structs_fea\")\n";

	content.append("--- FEM NODES\n");
	for (int i = 0; i < nodes.size(); ++i){
		content.append(str_format("fe_nodes[%d] = {x=%.3f,y=%.3f}\n", i, nodes[i].x, nodes[i].z));
	}

	int elementSize;
	if (elements.size() % 2) elementSize = elements.size() - 1;
	else elementSize = elements.size();
	content.append("--- FEM ELEMENTS\n");
	for (int i = 0; i < elementSize; i+=2){
		auto idx = vector_findi(nodes, elements[i]);
		auto idx1 = vector_findi(nodes, elements[i+1]);
		content.append(str_format("fe_elems[%d] = {n0=%d,n1=%d}\n", i/2, idx, idx1));
	}
	printf("Saving to %s\n", FPATH(Project/fem_solver/geometry.lua));
	writeFile(FPATH(sources/fem_solver/geometry.lua), content);
	printf("Stage 1: geometry definition completed\n");
	console_thread = new std::thread(console_input_loop);
}

static inline void console_input_loop(){
	printf("Boundary conditions stage...\n");
	printf("Specify boundary conditions through command prompt...\n");
	printf("Type away; press '~' to complete:\n");
	printf("> ");
	std::string content = "require(\"structs_fea\")\n";
	char c = 0;
	std::string line;
	while(c != '~'){
		c = getchar();
		if (c == '\n') { 
			printf("> "); 
			content += line;
			line.clear(); 
		}
		if (c == 127 || c == 8) { line.pop_back(); continue; }
		line += c;
	}

	printf("Saving to %s\n", FPATH(Project/fem_solver/boundary.lua));
	writeFile(FPATH(sources/fem_solver/boundary.lua), content);
	printf("Stage 2: boundary conditions setup completed\n");
	printf("--END--\n");
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------
static inline void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mCamera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mCamera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mCamera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mCamera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
static inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glfwGetWindowSize(window, &scrWidth, &scrHeight);
	window_resized = true;
}

//---------------------------------------------------------------------------------------------
// SCROLL CALLBACK FUNCTION
//---------------------------------------------------------------------------------------------
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
static inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mCamera.ProcessMouseScroll(yoffset);
}

inline static bool getHitPtFromRaycastToGrid(glm::vec3& hit, float mx, float my, float lim) {
	float x = (2.0f * mx) / scrWidth - 1.0f;
	float y = 1.0f - (2.0f * my) / scrHeight;
	float z = 1.0f;
	glm::vec3 ray_nds(x, y, z);
	glm::vec4 ray_clip(ray_nds.x, ray_nds.y, -1.0, 1.0);
	glm::vec4 ray_eye = glm::inverse(per_proj) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
	glm::vec3 ray_wor = (glm::inverse(view) * ray_eye);
	ray_wor = glm::normalize(ray_wor);
	//printf("x: %.2f, y: %.2f, z: %.2f\n", ray_wor.x, ray_wor.y, ray_wor.z);
	auto camPos = mCamera.getPosition();
	//printf("radius: %.2f ", camera.Radius);
	//printf("x: %.2f, y: %.2f, z: %.2f\n", camPos.x, camPos.y, camPos.z);
	// find t
	auto t = -camPos.z / ray_wor.z;
	if (t >= -0.00001f) {
		hit = camPos + t*ray_wor;
		hit.z = 0;
		//printf("x: %.2f, y: %.2f, z: %.2f\n\n", hit.x, hit.y, hit.z);
		auto grid_half_size_after_scaling = 1.0f / mCamera.Zoom / 2;
		if (hit.x > grid_half_size_after_scaling + lim || hit.x < -grid_half_size_after_scaling - lim) return false;
		if (hit.y > grid_half_size_after_scaling + lim || hit.y < -grid_half_size_after_scaling - lim) return false;
		return true;
	}
	return false;
}

bool numCloseWithin(float num, float lim);
inline static bool selectGrid(glm::ivec2& coord, const glm::vec3& hit, float lim)
{
	auto grid_half_size_after_scaling = 1.0f / mCamera.Zoom / 2;
	auto grid_step = grid_half_size_after_scaling * 2 / grid.gnum;
	auto a = hit.x / grid_step;
	auto b = hit.y / grid_step;
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

//---------------------------------------------------------------------------------------------
// ERROR CALLBACK
//---------------------------------------------------------------------------------------------
inline static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//---------------------------------------------------------------------------------------------
// MOUSE BUTTON CALLBACK FUNCTION
//---------------------------------------------------------------------------------------------
bool getHitPtFromRaycastToGrid(glm::vec3& hit, float mx, float my, float lim);
bool selectGrid(glm::ivec2& coord, const glm::vec3& hit, float lim);
void inline mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	if (action == GLFW_PRESS) {
		mouse_listener.button = button;
		mouse_listener.state = MouseListener::CLICK;
		//printf("Click\n");
	}

	if (action == GLFW_RELEASE) {
		mouse_listener.state = MouseListener::NIL;
	} 

	if (mouse_listener.clickedBy(GLFW_MOUSE_BUTTON_LEFT))
	{
		if (gui_container.generic_hit_testing_widgets()) return;

		double mouseX, mouseY;
		//getting cursor position
		glfwGetCursorPos(window, &mouseX, &mouseY);
		//std::cout << "Cursor Position at (" << mouseX << " : " << mouseY << ")" << std::endl;
		glm::vec3 hit;
		auto lim = 0.2;

		if (mouse_listener.agenda == MouseListener::CONNECT_ELE && getHitPtFromRaycastToGrid(hit, mouseX, mouseY, lim)) {
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

		if (mouse_listener.agenda == MouseListener::ADD_NODE && getHitPtFromRaycastToGrid(hit, mouseX, mouseY, lim))
		{
			//printf("x: %.2f, y: %.2f, z: %.2f\n\n", hit.x, hit.y, hit.z);
			//printf("add node success");
			glm::ivec2 coord(0);
			if (selectGrid(coord, hit, lim))
			{
				printf("i: %d, j: %d\n", coord.x, coord.y);
				vector_insert(nodes, grid.step*Vec3(coord.x, coord.y, 0));
			}
		}
	}
	mouse_listener.callback = MouseListener::BUTTON_CALLBACK;
}
void character_callback(GLFWwindow* window, unsigned int codepoint);
//---------------------------------------------------------------------------------------------
// PROGRAM INIT FUNCTION
//---------------------------------------------------------------------------------------------
inline static GLFWwindow* prog_init() {
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
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);

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

//---------------------------------------------------------------------------------------------
// PROGRAM'S CLEANUP FUNC
//---------------------------------------------------------------------------------------------
inline void static prog_cleanup(){
	glDeleteVertexArrays(1, &VAO_point);
	//glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO_point);
	axisLines.cleanup();
	grid.cleanup();

	for(auto& p : asset_container){
		delete p;
		p = nullptr;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}