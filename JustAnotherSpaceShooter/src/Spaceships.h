#pragma once

#include "InstancedModel.h"
#include "ParticleEffects.h"
#include <SFML/Audio.hpp>
#include <vector>

class Spaceships {
private:
	InstancedModel spaceship_im;
	// std::vector<float> health;
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

class Enemies;

class Player {
private:
	InstancedModel bullets;
	Spaceships ss;
	sf::SoundBuffer shoot_bullet_sound_buffer;
	sf::Sound enemy_hit_sound;
	sf::SoundBuffer enemy_hit_sound_buffer;
	sf::Sound shoot_bullet_sound;
	const unsigned int total_ammo = 50; /* @Note: This should be below MAX_INSTANCED_MODELS */
	unsigned int bullets_shot;
	float bullet_speed;
	Color bullet_color;
	unsigned int health;

	void update(Enemies *enemies);
public:
	void shoot_bullet();
	void init(GLuint program_id,
			  std::string player_model_filename, glm::vec3 player_position, Color player_color,
			  std::string bullet_model_filename, Color bullet_color);
	void change_scale(float factor);
	glm::vec3 get_front_position();
	void show(Enemies *enemies);
	void move_position_by(glm::vec3 delta);
	void check_for_collion_with_enemies(Enemies *enemies);
	Spaceships* get_spaceships();
};

class Enemies {
private:
	Spaceships ss;
	InstancedModel* bullets; /* @Note: Size is the MAX_INSTANCED_MODELS */
	ParticleEffect* particle_effects; /* @Note: Size is the MAX_INSTANCED_MODELS */
	Color bullet_color;
	const unsigned int total_ammo = 50; /* @Note: This should be below MAX_INSTANCED_MODELS */
	float bullet_speed;
	unsigned int *bullets_shot;

	GLuint particle_effect_program_id;
	std::string particle_effect_model_filename;

	void update(float current_global_tick, Spaceships *player_ss);
	void shoot_bullets();
public:
	void init(GLuint program_id, std::string enemy_model_filename,
		std::string bullet_model_filename, Color bullet_color,
		std::string particle_effect_model_filename, GLuint particle_effect_program_id);
	void move_position_by(int index, glm::vec3 delta);
	void add(glm::vec3 position, Color enemy_color, Color particle_effect_color);
	void show(float current_global_tick, Spaceships *player_ss);
	void change_scale_of_all(float enemy_scale_factor, float bullet_scale_factor);
	void move_position_of_all_by(glm::vec3 delta);
	void check_if_hit_player(Spaceships* ss);
	Spaceships* get_spaceships() { return &ss; }
	void start_particle_effect_for(int index);
};

bool BoxBoxIntersection(glm::vec3 objectMin, glm::vec3 objectMax, glm::vec3 colliderMin, glm::vec3 colliderMax);