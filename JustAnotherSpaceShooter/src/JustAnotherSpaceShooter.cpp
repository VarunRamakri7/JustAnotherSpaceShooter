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
#include <ctime>

#include <SFML/Audio.hpp>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "InstancedModel.h"
#include "Spaceships.h"
#include "ParticleEffects.h"

Color terrain_color_1;
Color bullet_color_1;
Color bullet_color_2;
Color spaceship_color;
Color enemies_color_1;
glm::vec3 light_position_1;
glm::vec3 light_direction_1;

// Default window dimensions
#define WIDTH 1920
#define HEIGHT 1080

Player main_player;
Enemies enemies;

GLuint instanced_model_shader = -1;
InstancedModel im_terrain_start;
InstancedModel im_terrain_mid;
InstancedModel im_terrain_end;

ParticleEffect pe_1;

sf::Music background_music;

// Window related data
glm::vec2 window_dims = glm::vec2(WIDTH, HEIGHT);
float aspectRatio = (float)window_dims.x / window_dims.y; // Window aspect ratio

// Camera related data
// glm::vec3 camera_pos   = glm::vec3(0.00445641f, 0.254251f, -2.54383f);
// glm::vec3 camera_front = glm::vec3(0.138021f, -0.201077f, 0.969803f);
glm::vec3 camera_pos   = glm::vec3(0.00217126f, 0.228749f, -2.51031f);
glm::vec3 camera_front = glm::vec3(0.00866673f, -0.116671f, 0.993133f);
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
float moveFactor = 3.0f;
float moveSideFactor = 0.2f;
float flightSpeed = 0.1f;
float scaleFactor = 1.0f;

// Spaceship related
glm::vec3 spaceship_pos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 spaceship_scale = glm::vec3(1.0f);
glm::vec3 spaceship_rot = glm::vec3(0.0f, 1.0f, 0.0f);

// Terrain related data
glm::vec3 terrainStart_pos = glm::vec3(0.0f, -0.5f, 0.0f);
glm::vec3 terrainStart_scale = glm::vec3(3.0f);
glm::vec3 terrainStart_rot = glm::vec3(0.0f, 1.0f, 0.0f);

void init_terrain()
{
	std::string model_folder = "data\\models\\";
	float upscale_factor = 4.0f;

	im_terrain_start.init(model_folder + "terrain_start.model", instanced_model_shader);
	im_terrain_mid.init(model_folder + "terrain_mid.model", instanced_model_shader);
	im_terrain_start.add(glm::vec3(0, 0, 0), terrain_color_1);
	im_terrain_start.change_scale(0, upscale_factor);
	im_terrain_start.rotate(0, glm::vec3(0, 1, 0), 180.0f);

	glm::vec3 d1 = im_terrain_start.get_dims(0);
	im_terrain_start.move_position_by(0, glm::vec3(0, 0, -d1.z / 2.0f * 1.2f));

	glm::vec3 start_position = glm::vec3(0, 0, 0);
	im_terrain_mid.add(start_position, terrain_color_1);
	im_terrain_mid.change_scale(0, 0.3784f * upscale_factor);
	im_terrain_mid.rotate(0, glm::vec3(0, 1, 0), 0);
	float terrain_mid_depth = im_terrain_mid.get_dims(0).z * 0.95f;
	for (int i = 1; i < 100; ++i) {
		glm::vec3 position = glm::vec3(start_position.x, start_position.y, i * terrain_mid_depth);
		im_terrain_mid.add(position, terrain_color_1);
		im_terrain_mid.change_scale(i, 0.3784f * upscale_factor);
		if (i % 2 != 0)
			im_terrain_mid.rotate(i, glm::vec3(0, 1, 0), 180);
		else
			im_terrain_mid.rotate(i, glm::vec3(0, 1, 0), 0);
	}

	im_terrain_end.init(model_folder + "terrain_end.model", instanced_model_shader);
	im_terrain_end.add(glm::vec3(0, 0, 0), terrain_color_1);
	im_terrain_end.change_scale(0, 0.3784f * upscale_factor);
	glm::vec3 d2 = im_terrain_mid.get_dims(99);
	glm::vec3 last_mid_pos = im_terrain_mid.get_front_pos(99, false);
	im_terrain_end.move_position_to(0, glm::vec3(0, 0, last_mid_pos.z + d2.z / 2.0f * 0.9f));
}

void init_enemies()
{
	std::string model_folder = "data\\models\\";

	enemies.init(instanced_model_shader, model_folder + "enemy_1.model",
		model_folder + "bullet.model", bullet_color_2);
	enemies.add(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 1.2f),
		enemies_color_1);
	enemies.add(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.2f),
		enemies_color_1);
	enemies.add(glm::vec3(+0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.2f),
		enemies_color_1);
	enemies.add(glm::vec3(+0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.5f),
		enemies_color_1);
	enemies.add(glm::vec3(-0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.5f),
		enemies_color_1);
	enemies.add(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 1.5f),
		enemies_color_1);

	for (unsigned int i = 0; i < im_terrain_mid.get_current_length(); i += 5)
	{
		glm::vec3 position = im_terrain_mid.get_position(i);

		enemies.add(glm::vec3(0, 0.2f, position.z), enemies_color_1);
	}

	enemies.change_scale_of_all(0.1f, 0.025f);
}

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
		terrain_color_1.La = glm::vec3(0.2125f, 0.1275f, 0.054f);
		terrain_color_1.Ld = glm::vec3(0.714f, 0.428f, 0.181f);
		terrain_color_1.Ls = glm::vec3(0.393f, 0.271f, 0.166f);
		terrain_color_1.Ka = glm::vec3(1.0f);
		terrain_color_1.Kd = glm::vec3(1.0f);
		terrain_color_1.Ks = glm::vec3(1.0f);
		terrain_color_1.shininess = 32.0f;

		bullet_color_1.La = glm::vec3(0.135f, 0.2225f, 0.1575f);
		bullet_color_1.Ld = glm::vec3(0.54f, 0.89f, 0.63f);
		bullet_color_1.Ls = glm::vec3(0.316228f, 0.316228f, 0.316228f);
		bullet_color_1.Ka = glm::vec3(1.0f);
		bullet_color_1.Kd = glm::vec3(1.0f);
		bullet_color_1.Ks = glm::vec3(1.0f);
		bullet_color_1.shininess = 32.0f;

		bullet_color_2.La = glm::vec3(0.8f, 0.8f, 0.8f);
		bullet_color_2.Ld = glm::vec3(0.54f, 0.69f, 0.63f);
		bullet_color_2.Ls = glm::vec3(0.716228f, 0.316228f, 0.716228f);
		bullet_color_2.Ka = glm::vec3(1.0f);
		bullet_color_2.Kd = glm::vec3(1.0f);
		bullet_color_2.Ks = glm::vec3(1.0f);
		bullet_color_2.shininess = 32.0f;

		enemies_color_1.La = glm::vec3(0.0215f,	0.1745f,	0.0215f);
		enemies_color_1.Ld = glm::vec3(0.07568f,	0.61424f,	0.07568f);
		enemies_color_1.Ls = glm::vec3(0.633f,	0.727811f,	0.633f);
		enemies_color_1.Ka = glm::vec3(1.0f);
		enemies_color_1.Kd = glm::vec3(1.0f);
		enemies_color_1.Ks = glm::vec3(1.0f);
		enemies_color_1.shininess = 32.0f;

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

		init_terrain();
		init_enemies();
		
		// Player
		main_player.init(instanced_model_shader, model_folder + "spaceship_main.model",
			glm::vec3(0, +0.2f, im_terrain_start.get_position(0).z), spaceship_color,
			model_folder + "bullet.model", bullet_color_1);
		main_player.change_scale(0.1f);

		// Enemies
		
	}

	/* Texture initialization */
	{
	}

	/* Others */
	light_position_1 = glm::vec3(0.0f, 2.0f, 0.0f);
	light_direction_1 = glm::vec3(0.0f, -1.0f, 0.0f);

	pe_1.init("data\\models\\point.model", instanced_model_shader,
		glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.2f),
		enemies_color_1);
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
	glm::mat4 projection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 5.0f);
	// glm::mat4 projection = glm::perspective(fov, window_dims.x / window_dims.y, 0.1f, 20.0f);

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
		
		
		im_terrain_start.draw();
		im_terrain_mid.draw();
		im_terrain_end.draw();
		main_player.show(enemies.get_spaceships());
		enemies.show(glfwGetTime(), main_player.get_spaceships());
		
		// pe_1.show(glfwGetTime());
	}

	// std::cout << "pos: " << camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z << std::endl;
	// std::cout << "front: " << camera_front.x << ", " << camera_front.y << ", " << camera_front.z << std::endl;

	draw_gui(window);
}

void idle()
{
	main_player.move_position_by(glm::vec3(0, 0, deltaTime * moveFactor));
	// enemies.move_position_of_all_by(glm::vec3(0, 0, deltaTime * moveFactor));
	camera_pos.z += deltaTime * moveFactor;
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

	float cameraSpeed = 0.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera_pos += cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera_pos -= cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		main_player.move_position_by(glm::vec3(0, 0, moveFactor * deltaTime));
		enemies.move_position_of_all_by(glm::vec3(0, 0, moveFactor * deltaTime));
		camera_pos.z += moveFactor * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		main_player.move_position_by(glm::vec3(+moveSideFactor * deltaTime, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		main_player.move_position_by(glm::vec3(-moveSideFactor * deltaTime, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		main_player.move_position_by(glm::vec3(0, 0.1f * deltaTime, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		main_player.move_position_by(glm::vec3(0, -0.1f * deltaTime, 0));
	}
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		switch (key)
		{
		case 32: // Space
			main_player.shoot_bullet();
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

void init_game_music()
{
	/* Game Background Music */
	if (!background_music.openFromFile("data\\wav\\arcade_music_loop.wav")) {
		std::cout << "Could not load file: data\\wav\\arcade_music_loop.wav" << std::endl;
	}
	else {
		background_music.play();
		background_music.setVolume(15.0f);
		background_music.setLoop(true);
	}
	/* Game Background Music */
}


int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	const char *window_name = "Just Another Space Shooter";
	GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(primary_monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//GLFWwindow *window = glfwCreateWindow(mode->width, mode->height,"Just Another Space Shooter",
	//	primary_monitor, NULL);
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Just Another Space Shooter", NULL, NULL);
	
	window_dims.x = mode->width;
	window_dims.y = mode->height;

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetKeyCallback(window, keyboard);
	// glfwSetCursorPosCallback(window, mouse_cursor);
	glfwSetMouseButtonCallback(window, mouse_button);
	//glfwSetScrollCallback(window, mouse_scroll);
	glfwSetWindowSizeCallback(window, resize);
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwMaximizeWindow(window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 150");

	srand(unsigned int(time(NULL)));

	init_game();
	init_game_music();

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
