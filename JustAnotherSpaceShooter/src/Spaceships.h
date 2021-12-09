#pragma once

#include "InstancedModel.h"
#include "ParticleEffects.h"
#include <SFML/Audio.hpp>
#include <vector>

#include <iostream> // @TMP

class Spaceships {
private:
	InstancedModel spaceship_im;
	// std::vector<float> health;
public:
	void show();
	void init(std::string model_filename, GLuint program_id);
	void add_new_spaceship(glm::vec3 position, Color color);
	void change_scale(int index, float factor);
	void move_position_to(int index, glm::vec3 new_position);
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
	ParticleEffect pe;
	sf::SoundBuffer shoot_bullet_sound_buffer;
	sf::Sound enemy_hit_sound;
	sf::SoundBuffer enemy_hit_sound_buffer;
	sf::Sound shoot_bullet_sound;
	sf::SoundBuffer player_destroyed_sound_buffer;
	sf::Sound player_destroyed_sound;
	const unsigned int total_ammo = 100; /* @Note: This should be below MAX_INSTANCED_MODELS */
	unsigned int bullets_shot;
	float bullet_speed;
	Color bullet_color;
	bool player_destroyed;

	void update(Enemies *enemies);
public:
	void shoot_bullet();
	void init(GLuint program_id,
			  std::string player_model_filename, glm::vec3 player_position, Color player_color,
			  std::string bullet_model_filename, Color bullet_color, Color particle_color);
	void change_scale(float factor);
	glm::vec3 get_front_position();
	void show(float current_global_tick, Enemies *enemies);
	void move_position_by(glm::vec3 delta);
	void move_position_to(glm::vec3 new_position);
	void check_for_collision_with_enemies(Enemies *enemies);
	Spaceships* get_spaceships();
	void reset(glm::vec3 new_position);
	void destroy();
	bool is_destroyed() { return player_destroyed; }
	unsigned int get_remaining_ammo() { return total_ammo - bullets_shot; }
	void set_player_bullet_speed(float speed);
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
	bool* enemy_destroyed;
	float initial_shoot_bullet_delay;
	float init_start_tick;

	GLuint particle_effect_program_id;
	std::string particle_effect_model_filename;

	void update(float current_global_tick, Player *player);
	void shoot_bullets();
public:
	void init(GLuint program_id, std::string enemy_model_filename,
		std::string bullet_model_filename, Color bullet_color,
		std::string particle_effect_model_filename, GLuint particle_effect_program_id);
	void move_position_by(int index, glm::vec3 delta);
	void add(glm::vec3 position, Color enemy_color, Color particle_effect_color);
	void show(float current_global_tick, Player *player);
	void change_scale_of_all(float enemy_scale_factor, float bullet_scale_factor);
	void move_position_of_all_by(glm::vec3 delta);
	void check_if_hit_player(Player *player);
	void reset(std::vector<glm::vec3> new_enemy_positions);
	
	Spaceships* get_spaceships() { return &ss; }
	void start_particle_effect_for(int index);
	void set_enemy_destroyed(int index) { enemy_destroyed[index] = true; }
	bool is_enemy_destroyed(int index) { return enemy_destroyed[index]; }
	bool are_all_enemies_destroyed();
	void set_enemy_bullet_speed(float speed);
};

bool BoxBoxIntersection(glm::vec3 objectMin, glm::vec3 objectMax, glm::vec3 colliderMin, glm::vec3 colliderMax);