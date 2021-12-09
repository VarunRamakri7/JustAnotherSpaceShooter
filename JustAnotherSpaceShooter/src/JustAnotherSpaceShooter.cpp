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
#include "InstancedModel.h"
#include "Spaceships.h"
#include "ParticleEffects.h"

Color terrain_color_1;
Color bullet_color_1;
Color bullet_color_2;
Color spaceship_color;
Color enemies_color_green;
Color enemies_color_blue;
Color enemies_color_black;
Color particles_color_1;
Color particles_color_2;
glm::vec3 light_position_1;
glm::vec3 light_direction_1;

Player main_player;
Enemies enemies;

GLuint instanced_model_shader = -1;
InstancedModel im_terrain_start;
InstancedModel im_terrain_mid;
InstancedModel im_terrain_end;

sf::Music background_music;

// Formation related data
float delay_to_next_formation = 2.0f;
float next_formation_start_tick = 0;
float next_formation_current_tick = 0;
bool start_formation_delay = false;

// Window related data
glm::vec2 window_dims = glm::vec2(1920, 1080);
float aspectRatio = (float)window_dims.x / window_dims.y; // Window aspect ratio

// Camera related data
const glm::vec3 initial_camera_pos = glm::vec3(0.33971f, 1.14269f, -5.36388f);
glm::vec3 camera_pos = initial_camera_pos;
glm::vec3 camera_front = glm::vec3(-0.0931682f, -0.297374f, 0.950204f);
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
float moveFactor = 1.5f;
float moveSideFactor = 0.3f; // Was 0.2
float flightSpeed = 0.1f;
float scaleFactor = 1.0f;

unsigned int current_enemy_formation_index = 0;
std::vector<std::vector<glm::vec3>> all_enemy_formations;

void reset_game();

void init_all_enemy_formations()
{
	/* Formation 1 */
	{
		std::vector<glm::vec3> positions;

		positions.push_back(glm::vec3(+0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.5f));
		positions.push_back(glm::vec3(-0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.5f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 1.5f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 1.2f));
		positions.push_back(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.2f));
		positions.push_back(glm::vec3(+0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.2f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.9f));
		positions.push_back(glm::vec3(-0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.9f));
		positions.push_back(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.9f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.6f));
		positions.push_back(glm::vec3(-0.1f, 0.2f, im_terrain_start.get_position(0).z + 0.6f));
		positions.push_back(glm::vec3(+0.1f, 0.2f, im_terrain_start.get_position(0).z + 0.6f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.3f));
		positions.push_back(glm::vec3(-0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.3f));
		positions.push_back(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.3f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.0f));
		positions.push_back(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 0.0f));
		positions.push_back(glm::vec3(+0.2f, 0.2f, im_terrain_start.get_position(0).z + 0.0f));

		all_enemy_formations.push_back(positions);
	}
	/* Formation 1 */

	/* Formation 2 */
	{
		std::vector<glm::vec3> positions;

		positions.push_back(glm::vec3(0.0f, 0.2f, im_terrain_start.get_position(0).z + 1.5f));
		positions.push_back(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.5f));
		positions.push_back(glm::vec3(+0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.5f));
		positions.push_back(glm::vec3(-0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.2f));
		positions.push_back(glm::vec3(0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.2f));
		positions.push_back(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 1.2f));
		positions.push_back(glm::vec3(-0.1f, 0.2f, im_terrain_start.get_position(0).z + 0.9f));
		positions.push_back(glm::vec3(-0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.9f));
		positions.push_back(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.9f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.6f));
		positions.push_back(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 0.6f));
		positions.push_back(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.6f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.3f));
		positions.push_back(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 0.3f));
		positions.push_back(glm::vec3(+0.2f, 0.2f, im_terrain_start.get_position(0).z + 0.3f));
		positions.push_back(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.0f));
		positions.push_back(glm::vec3(-0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.0f));
		positions.push_back(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.0f));

		all_enemy_formations.push_back(positions);
	}
	/* Formation 2 */
}

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
	for (int i = 1; i < 200; ++i) {
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
	glm::vec3 d2 = im_terrain_mid.get_dims(199);
	glm::vec3 last_mid_pos = im_terrain_mid.get_front_pos(199, false);
	im_terrain_end.move_position_to(0, glm::vec3(0, 0, last_mid_pos.z + d2.z / 2.0f * 0.9f));
}

void init_enemies()
{
	std::string model_folder = "data\\models\\";

	enemies.init(instanced_model_shader, model_folder + "enemy_1.model",
		model_folder + "bullet.model", bullet_color_2,
		model_folder + "point.model", instanced_model_shader);	

	// Add all enemies and set theire position
	enemies.add(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 1.2f),
		enemies_color_green, particles_color_1);
	enemies.add(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.2f),
		enemies_color_green, particles_color_1);
	enemies.add(glm::vec3(+0.2f, 0.2f, im_terrain_start.get_position(0).z + 1.2f),
		enemies_color_green, particles_color_1);
	enemies.add(glm::vec3(+0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.5f),
		enemies_color_black, particles_color_1);
	enemies.add(glm::vec3(-0.1f, 0.2f, im_terrain_start.get_position(0).z + 1.5f),
		enemies_color_black, particles_color_1);
	enemies.add(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 1.5f),
		enemies_color_black, particles_color_1);
	enemies.add(glm::vec3(0, 0.2f, im_terrain_start.get_position(0).z + 0.9f),
		enemies_color_blue, particles_color_1);
	enemies.add(glm::vec3(-0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.9f),
		enemies_color_blue, particles_color_1);
	enemies.add(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.9f),
		enemies_color_blue, particles_color_1);
	enemies.add(glm::vec3(+0.0f, 0.2f, im_terrain_start.get_position(0).z + 0.6f),
		enemies_color_green, particles_color_1);
	enemies.add(glm::vec3(-0.1f, 0.2f, im_terrain_start.get_position(0).z + 0.6f),
		enemies_color_green, particles_color_1);
	enemies.add(glm::vec3(+0.1f, 0.2f, im_terrain_start.get_position(0).z + 0.6f),
		enemies_color_green, particles_color_1);
	enemies.add(glm::vec3(+0.0f, 0.2f, im_terrain_start.get_position(0).z + 0.3f),
		enemies_color_black, particles_color_1);
	enemies.add(glm::vec3(-0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.3f),
		enemies_color_black, particles_color_1);
	enemies.add(glm::vec3(+0.3f, 0.2f, im_terrain_start.get_position(0).z + 0.3f),
		enemies_color_black, particles_color_1);
	enemies.add(glm::vec3(+0.0f, 0.2f, im_terrain_start.get_position(0).z + 0.0f),
		enemies_color_blue, particles_color_1);
	enemies.add(glm::vec3(-0.2f, 0.2f, im_terrain_start.get_position(0).z + 0.0f),
		enemies_color_blue, particles_color_1);
	enemies.add(glm::vec3(+0.2f, 0.2f, im_terrain_start.get_position(0).z + 0.0f),
		enemies_color_blue, particles_color_1);

	enemies.change_scale_of_all(0.1f, 0.025f);

	init_all_enemy_formations();
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
		terrain_color_1.Ld = glm::vec3(0.214f, 0.428f, 0.181f);
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

		bullet_color_2.La = glm::vec3(1.0f, 1.0f, 1.0f);
		bullet_color_2.Ld = glm::vec3(0.24f, 0.29f, 0.23f);
		bullet_color_2.Ls = glm::vec3(0.016228f, 0.016228f, 0.016228f);
		bullet_color_2.Ka = glm::vec3(1.0f);
		bullet_color_2.Kd = glm::vec3(1.0f);
		bullet_color_2.Ks = glm::vec3(1.0f);
		bullet_color_2.shininess = 32.0f;

		enemies_color_green.La = glm::vec3(0.0215f,	0.1745f,	0.0215f);
		enemies_color_green.Ld = glm::vec3(0.07568f,	0.61424f,	0.07568f);
		enemies_color_green.Ls = glm::vec3(0.633f,	0.727811f,	0.633f);
		enemies_color_green.Ka = glm::vec3(1.0f);
		enemies_color_green.Kd = glm::vec3(1.0f);
		enemies_color_green.Ks = glm::vec3(1.0f);
		enemies_color_green.shininess = 32.0f;

		enemies_color_blue.La = glm::vec3(0.0215f, 0.0745f, 0.615f);
		enemies_color_blue.Ld = glm::vec3(0.07568f, 0.61424f, 0.07568f);
		enemies_color_blue.Ls = glm::vec3(0.633f, 0.727811f, 0.633f);
		enemies_color_blue.Ka = glm::vec3(1.0f);
		enemies_color_blue.Kd = glm::vec3(1.0f);
		enemies_color_blue.Ks = glm::vec3(1.0f);
		enemies_color_blue.shininess = 32.0f;

		enemies_color_black.La = glm::vec3(0.1f,	0.1f,	0.0f);
		enemies_color_black.Ld = glm::vec3(0.7f,	0.7f,	0.0f);
		enemies_color_black.Ls = glm::vec3(0.70f,	0.70f,	0.30f);
		enemies_color_black.Ka = glm::vec3(1.0f);
		enemies_color_black.Kd = glm::vec3(1.0f);
		enemies_color_black.Ks = glm::vec3(1.0f);
		enemies_color_black.shininess = 64.0f;

		spaceship_color.La = glm::vec3(0.1745f, 0.01175f, 0.01175f);
		spaceship_color.Ld = glm::vec3(0.614f, 0.041f, 0.041f);
		spaceship_color.Ls = glm::vec3(0.727f, 0.626f, 0.626f);
		spaceship_color.Ka = glm::vec3(1.0f);
		spaceship_color.Kd = glm::vec3(1.0f);
		spaceship_color.Ks = glm::vec3(1.0f);
		spaceship_color.shininess = 32.0f;

		particles_color_1.La = glm::vec3(1.0f, 1.0f, 1.0f);
		particles_color_1.Ld = glm::vec3(0.614f, 0.041f, 0.041f);
		particles_color_1.Ls = glm::vec3(0.727f, 0.626f, 0.626f);
		particles_color_1.Ka = glm::vec3(1.0f);
		particles_color_1.Kd = glm::vec3(1.0f);
		particles_color_1.Ks = glm::vec3(1.0f);
		particles_color_1.shininess = 32.0f;

		particles_color_2.La = glm::vec3(1.0f, 0.0f, 1.0f);
		particles_color_2.Ld = glm::vec3(0.614f, 0.041f, 0.041f);
		particles_color_2.Ls = glm::vec3(0.727f, 0.626f, 0.626f);
		particles_color_2.Ka = glm::vec3(1.0f);
		particles_color_2.Kd = glm::vec3(1.0f);
		particles_color_2.Ks = glm::vec3(1.0f);
		particles_color_2.shininess = 32.0f;
	}

	/* Main Entities initialization */
	{
		std::string model_folder = "data\\models\\";

		init_terrain();
		init_enemies();
		
		// Player
		main_player.init(instanced_model_shader, model_folder + "spaceship_main.model",
			glm::vec3(0, +0.2f, im_terrain_start.get_position(0).z - 0.8f), spaceship_color,
			model_folder + "bullet.model", bullet_color_1, particles_color_2);
		main_player.change_scale(0.1f);
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

	{
		ImGui::Begin("Stats");
		ImGui::SetWindowFontScale(1.5f);
		ImGui::SetWindowSize(ImVec2(300, 220));
		ImGui::SetWindowPos(ImVec2(0, window_dims.y - 220));
		{
			ImGui::Text("Controls:\n   Space: shoot\n   A: Move left\n   D: Move right\n   Esc: Quit");
			ImGui::Text("Ammo remaining: %d\n", main_player.get_remaining_ammo());
			ImGui::Text("Current level: %d\n", current_enemy_formation_index + 1);
			if (ImGui::Button("Reset Game"))
				reset_game();
		}
		ImGui::End();
	}
	 

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void display(GLFWwindow *window)
{
	// glm::vec3 camLookAt = glm::vec3(0.0f, 2.0f, 5.0f);
	glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
	glm::mat4 projection = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.01f, 10.0f);
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
		
		im_terrain_start.draw(GL_TRIANGLES);
		im_terrain_mid.draw(GL_TRIANGLES);
		im_terrain_end.draw(GL_TRIANGLES);
		main_player.show(glfwGetTime(), &enemies);
		enemies.show(glfwGetTime(), &main_player);
	}

	// std::cout << "pos: " << camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z << std::endl;
	// std::cout << "front: " << camera_front.x << ", " << camera_front.y << ", " << camera_front.z << std::endl;

	draw_gui(window);
}

void reset_and_goto_next_formation()
{
	start_formation_delay = false;
	next_formation_current_tick = 0;
	next_formation_start_tick = 0;

	current_enemy_formation_index += 1;
	if (current_enemy_formation_index >= all_enemy_formations.size())
		current_enemy_formation_index = 0;

	enemies.reset(all_enemy_formations[current_enemy_formation_index]);
	main_player.reset(glm::vec3(0, +0.2f, im_terrain_start.get_position(0).z - 0.8f));
	camera_pos = initial_camera_pos;
}

void reset_game()
{
	start_formation_delay = false;
	next_formation_current_tick = 0;
	next_formation_start_tick = 0;

	current_enemy_formation_index = 0;

	enemies.reset(all_enemy_formations[current_enemy_formation_index]);
	main_player.reset(glm::vec3(0, +0.2f, im_terrain_start.get_position(0).z - 0.8f));
	camera_pos = initial_camera_pos;
}

void idle()
{
	if ((enemies.are_all_enemies_destroyed() || main_player.is_destroyed())
		&& start_formation_delay == false) {
		start_formation_delay = true;
		next_formation_start_tick = glfwGetTime();
		next_formation_current_tick = next_formation_start_tick;
	}

	if (start_formation_delay)
		next_formation_current_tick = glfwGetTime();

	if (start_formation_delay && (next_formation_current_tick - next_formation_start_tick)
		>= delay_to_next_formation) {
		if (enemies.are_all_enemies_destroyed())
			reset_and_goto_next_formation();
		else if (main_player.is_destroyed())
			reset_game();
	}

	// Move camera and all Spaceships
	main_player.move_position_by(glm::vec3(0, 0, deltaTime * moveFactor));
	enemies.move_position_of_all_by(glm::vec3(0, 0, deltaTime * moveFactor));
	camera_pos.z += deltaTime * moveFactor;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
	window_dims = glm::vec2(width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// Move camera
	float cameraSpeed = 0.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		camera_pos += cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		camera_pos -= cameraSpeed * camera_front;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;

	// Flight controls
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

/*void mouse_button(GLFWwindow* window, int button, int action, int mods)
{
}*/

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
		background_music.setVolume(20.0f);
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
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, window_name, primary_monitor, NULL);
	// GLFWwindow* window = glfwCreateWindow(1024, 768, "Just Another Space Shooter", NULL, NULL);
	
	window_dims.x = mode->width;
	window_dims.y = mode->height;

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSetKeyCallback(window, keyboard);
	// glfwSetCursorPosCallback(window, mouse_cursor);
	///glfwSetMouseButtonCallback(window, mouse_button);
	glfwSetWindowSizeCallback(window, resize);
	glEnable(GL_DEPTH_TEST);
	glfwSwapInterval(1);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwMaximizeWindow(window);
	glEnable(GL_PROGRAM_POINT_SIZE);

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
