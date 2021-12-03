#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "InstancedModel.h"
#include "Spaceships.h"

Color terrain_color;
Color spaceship_color;
glm::vec3 light_position_1;
glm::vec3 light_direction_1;

// Default window dimensions
#define WIDTH 1920
#define HEIGHT 1080

Spaceships spaceships_test;

GLuint instanced_model_shader = -1;
InstancedModel im_terrain;
InstancedModel im_bullet;

// Window related data
glm::vec2 window_dims = glm::vec2(WIDTH, HEIGHT);
float aspectRatio = (float)window_dims.x / window_dims.y; // Window aspect ratio

// Camera related data
glm::vec3 camera_pos   = glm::vec3(-4.92299f, 3.34283f, -2.61573f);
glm::vec3 camera_front = glm::vec3(0.519728f, -0.67944f, 0.517922f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = 90.0f;
float pitch =  0.0f;
float lastX =  window_dims.x / 2.0;
float lastY =  window_dims.y / 2.0;
float fov   =  45.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// World Related data
const glm::vec3 origin = glm::vec3(0.0f);
float moveFactor = 5.0f;
float flightSpeed = 0.5f;
float scaleFactor = 1.0f;

// Spaceship related
glm::vec3 spaceship_pos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 spaceship_scale = glm::vec3(1.0f);
glm::vec3 spaceship_rot = glm::vec3(0.0f, 1.0f, 0.0f);

// Terrain related data
glm::vec3 terrainStart_pos = glm::vec3(0.0f, -0.5f, 0.0f);
glm::vec3 terrainStart_scale = glm::vec3(3.0f);
glm::vec3 terrainStart_rot = glm::vec3(0.0f, 1.0f, 0.0f);

void init_game()
{
	/* Shader initialization */
	{
		std::string shader_folder = "shaders\\";

		// Model Shader (Testing)
		std::string model_vs_fname = shader_folder + "instanced_model_vs.glsl";
		std::string model_fs_fname = shader_folder + "instanced_model_fs.glsl";
		instanced_model_shader = InitShader(model_vs_fname.c_str(), model_fs_fname.c_str());
	}

	/* Colors initialization */
	{
		terrain_color.La = glm::vec3(0.2125f, 0.1275f, 0.054f);
		terrain_color.Ld = glm::vec3(0.714f, 0.428f, 0.181f);
		terrain_color.Ls = glm::vec3(0.393f, 0.271f, 0.166f);
		terrain_color.Ka = glm::vec3(1.0f);
		terrain_color.Kd = glm::vec3(1.0f);
		terrain_color.Ks = glm::vec3(1.0f);
		terrain_color.shininess = 32.0f;

		spaceship_color.La = glm::vec3(0.1745f, 0.01175f, 0.01175f);
		spaceship_color.Ld = glm::vec3(0.614f, 0.041f, 0.041f);
		spaceship_color.Ls = glm::vec3(0.727f, 0.626f, 0.626f);
		spaceship_color.Ka = glm::vec3(1.0f);
		spaceship_color.Kd = glm::vec3(1.0f);
		spaceship_color.Ks = glm::vec3(1.0f);
		spaceship_color.shininess = 32.0f;
	}

	/* Model initialization */
	{
		std::string model_folder = "data\\models\\";

		// terrain_model
		std::string tmp_fname_2 = model_folder + "terrain_start.model";
		im_terrain.init(tmp_fname_2, instanced_model_shader);
		im_terrain.add(glm::vec3(0, -2, 0), spaceship_color);
		im_terrain.add(glm::vec3(2, -2, 0), terrain_color);
		im_terrain.add(glm::vec3(-2, -2, 0), spaceship_color);
		im_terrain.add(glm::vec3(-4, -2, 0), terrain_color);

		std::string tmp_fname_1 = model_folder + "spaceship_main.model";
		spaceships_test.init(tmp_fname_1, instanced_model_shader);
		spaceships_test.add_new_spaceship(glm::vec3(0, 0, 0), spaceship_color);

		std::string tmp_fname_3 = model_folder + "bullet.model";
		im_bullet.init(tmp_fname_3, instanced_model_shader);
		im_bullet.add(spaceships_test.get_front_pos(0), terrain_color);
		im_bullet.change_scale(0, 0.25f * 0.5f);
	}

	/* Texture initialization */
	{
	}

	/* Others */
	light_position_1 = glm::vec3(0.0f, 2.0f, 0.0f);
	light_direction_1 = glm::vec3(0.0f, -1.0f, 0.0f);
}

void draw_gui(GLFWwindow *window)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	/*
		ImGui::Begin("Debug Game");
		ImGui::SetWindowFontScale(1.5f);
		ImGui::SetWindowSize(ImVec2(700, 350));
		{
		}
		ImGui::End();
	 */

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void display(GLFWwindow *window)
{
	glm::vec3 camLookAt = glm::vec3(0.0f, 0.0f, spaceship_pos.z);
	glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
	// glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.01f, 100.0f);
	glm::mat4 projection = glm::perspective(fov, window_dims.x / window_dims.y, 0.1f, 100.0f);

	{
		glUseProgram(instanced_model_shader);
		int projection_loc = glGetUniformLocation(instanced_model_shader, "projection");
		int view_loc = glGetUniformLocation(instanced_model_shader, "view");
		int lightDirection_loc = glGetUniformLocation(instanced_model_shader, "light_direction");
		int viewPosition_loc = glGetUniformLocation(instanced_model_shader, "view_position");
		glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));
		glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
		glUniform3fv(lightDirection_loc, 1, glm::value_ptr(light_direction_1));
		glUniform3fv(viewPosition_loc, 1, glm::value_ptr(camera_pos));
		im_terrain.draw();
		im_bullet.draw();
		spaceships_test.show();
	}

	// std::cout << "pos: " << camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z << std::endl;
	// std::cout << "front: " << camera_front.x << ", " << camera_front.y << ", " << camera_front.z << std::endl;

	draw_gui(window);
}

void idle()
{
	// Steadily move ship and camera
	/*spaceships_test.move(0, glm::vec3(0, 0, moveFactor * deltaTime));
	camera_pos.z += moveFactor * deltaTime;*/
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	window_dims = glm::vec2(width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 1.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera_pos += cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera_pos -= cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
}


void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//std::cout << "key : " << key << ", " << char(key) << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << std::endl;

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case 'w':
		case 'W':
			// im_1.move_position(0, glm::vec3(0, 0, moveFactor * deltaTime));
			spaceships_test.move(0, glm::vec3(0, 0, moveFactor * deltaTime));
			camera_pos.z += moveFactor * deltaTime;
			break;
		case 's':
		case 'S':
			// im_1.move_position(0, glm::vec3(0, 0, -(moveFactor * deltaTime)));
			camera_pos.z += -(moveFactor * deltaTime);
			break;
		case 'a':
		case 'A':
			//im_1.move_position(0, glm::vec3(moveFactor * deltaTime, 0, 0));
			break;
		case 'd':
		case 'D':
			//im_1.move_position(0, glm::vec3(moveFactor * deltaTime, 0, 0));
			break;

		case GLFW_KEY_SPACE: // Shoot bullet
			// TODO: Spawn and shoot bullet
			break;

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		}
	}
}


void mouse_cursor(GLFWwindow *window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(direction);
}

void mouse_button(GLFWwindow *window, int button, int action, int mods)
{
}

/*
	void mouse_scroll(GLFWwindow *window, double xoffset, double yoffset)
	{
	switch ((int)yoffset)
	{
	case 1:                  // Scroll Up
	scaleFactor += 0.5f; // Increase scale
	break;

	case -1:                 // Scroll Down
	scaleFactor *= 0.5f; // Decrease scale
	break;
	}

// Update scales
//spaceship_scale *= scaleFactor;
terrainStart_scale *= scaleFactor;

// Print info
std::printf("New scale factor: %0.3f\n", scaleFactor);
//std::printf("New Spaceship Scale: (%.9f, %.9f, %.9f)\n", spaceship_scale.x, spaceship_scale.y, spaceship_scale.z);
std::printf("New Terrain Scale: (%.2f, %.2f, %.2f)\n", terrainStart_scale.x, terrainStart_scale.y, terrainStart_scale.z);
}
 */

void resize(GLFWwindow *window, int width, int height)
{
	std::printf("New window dimensions: (%.2u, %.2u)\n", width, height);

	if (height == 0)
		height = 1;

	aspectRatio = (float)width / height; // Update aspect ratio
	window_dims = glm::vec2(width, height);
	glViewport(0, 0, width, height);
}

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	const char *window_name = "Just Another Space Shooter";
	GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(primary_monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "My Title", primary_monitor, NULL);
	window_dims.x = mode->width;
	window_dims.y = mode->height;

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetKeyCallback(window, keyboard);
	glfwSetCursorPosCallback(window, mouse_cursor);
	glfwSetMouseButtonCallback(window, mouse_button);
	//glfwSetScrollCallback(window, mouse_scroll);
	glfwSetWindowSizeCallback(window, resize);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwMaximizeWindow(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	init_game();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1.0f);

		processInput(window);

		idle();
		display(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}
