#pragma once

#include "InstancedModel.h"
#include <vector>

class Spaceships {
private:
	InstancedModel spaceship_im;
	// std::vector<float> health;
	// std::vector<float> speed;
public:
	void show();
	void init(std::string model_filename, GLuint program_id);
	void add_new_spaceship(glm::vec3 position, Color color);
	void change_scale(int index, float factor);
	void move_position_to(int index, glm::vec3 delta);
	glm::vec3 get_front_pos(int index, bool flip_on_y_180);
	void move_position_by(int id, glm::vec3 delta);
	unsigned int get_current_num_spaceships();
	void rotate(int index, glm::vec3 axes, float degree);
	glm::vec3 get_dims(int index);
	glm::vec3 get_position(int index);
	void SetCollisionStatus(bool status);
	bool GetCollisionStatus();
};

class Player {
private:
	InstancedModel bullets;
	Spaceships ss;
	const unsigned int total_ammo = 50; /* @Note: This should be below MAX_INSTANCED_MODELS */
	unsigned int bullets_shot;
	float bullet_speed;
	Color bullet_color;

	void update();
public:
	void shoot_bullet();
	void init(GLuint program_id,
			  std::string player_model_filename, glm::vec3 player_position, Color player_color,
			  std::string bullet_model_filename, Color bullet_color);
	void change_scale(float factor);
	glm::vec3 get_front_position();
	void show();
	void move_position_by(glm::vec3 delta);
	void check_for_collion_with_enemies(Spaceships* ss);
};

class Enemies {
private:
	Spaceships ss;
	InstancedModel* bullets; /* @Note: Size is the MAX_INSTANCED_MODELS */
	Color bullet_color;
	const unsigned int total_ammo = 50; /* @Note: This should be below MAX_INSTANCED_MODELS */
	float bullet_speed;
	unsigned int *bullets_shot;

	void shoot_bullets();
	void update();
public:
	void init(GLuint program_id, std::string enemy_model_filename,
		std::string bullet_model_filename, Color bullet_color);
	void move_position_by(int index, glm::vec3 delta);
	void add(glm::vec3 position, Color color);
	void show();
	void change_scale_of_all(float enemy_scale_factor, float bullet_scale_factor);
	void move_position_of_all_by(glm::vec3 delta);
	void check_if_hit_player(Spaceships* ss);
};

bool BoxBoxIntersection(glm::vec3 objectMin, glm::vec3 objectMax, glm::vec3 colliderMin, glm::vec3 colliderMax);