#include "Spaceships.h"
#include <iostream>

void Spaceships::init(std::string model_filename, GLuint program_id) 
{
	spaceship_im.init(model_filename, program_id);
}

glm::vec3 Spaceships::get_position(int index)
{
	return spaceship_im.get_position(index);
}

void Spaceships::add_new_spaceship(glm::vec3 position, Color color) 
{
	spaceship_im.add(position, color);
}

glm::vec3 Spaceships::get_dims(int index)
{
	return spaceship_im.get_dims(index);
}

void Spaceships::rotate(int index, glm::vec3 axes, float degree)
{
	spaceship_im.rotate(index, axes, degree);
}

unsigned int Spaceships::get_current_num_spaceships()
{
	return spaceship_im.get_current_length();
}

void Spaceships::change_scale(int index, float factor)
{
	spaceship_im.change_scale(index, factor);
}

glm::vec3 Spaceships::get_front_pos(int index, bool flip_on_y_180)
{
	return spaceship_im.get_front_pos(index, flip_on_y_180);
}

void Spaceships::move_position_to(int index, glm::vec3 delta)
{
	spaceship_im.move_position_to(index, delta);
}

void Spaceships::move_position_by(int id, glm::vec3 delta) 
{
	spaceship_im.move_position_by(id, delta);
}

void Spaceships::SetCollisionStatus(bool status)
{
	spaceship_im.SetCollisionStatus(status);
}

bool Spaceships::GetCollisionStatus()
{
	return spaceship_im.GetCollisionStatus();
}

/* @NOTE: This function is incorrect */
void Player::check_for_collision_with_enemies(Enemies *enemies)
{
	Spaceships* enemies_ss = enemies->get_spaceships();

	for (unsigned int i = 0; i < bullets_shot; ++i)
	{
		if (bullets.GetCollisionStatus())
		{
			glm::vec3 bullet_position = bullets.get_position(i);
			glm::vec3 bullet_dims = bullets.get_dims(i);
			glm::vec3 bullet_max_dims = bullet_position + (0.5f * bullet_dims);
			glm::vec3 bullet_min_dims = bullet_position - (0.5f * bullet_dims);

			for (unsigned int j = 0; j < enemies_ss->get_current_num_spaceships(); ++j)
			{
				if (enemies->is_enemy_destroyed(i))
					continue;

				glm::vec3 enemy_position = enemies_ss->get_position(j);
				glm::vec3 enemy_dims = enemies_ss->get_dims(j);
				glm::vec3 enemy_max_dims = enemy_position + (0.5f * enemy_dims);
				glm::vec3 enemy_min_dims = enemy_position - (0.5f * enemy_dims);

				if (BoxBoxIntersection(bullet_max_dims, bullet_min_dims, enemy_min_dims, enemy_max_dims))
				{
					bullets.move_position_to(i, glm::vec3(i, 0.0f, 0.0f));
					enemies->start_particle_effect_for(j);
					enemies->set_enemy_destroyed(j);
					enemy_hit_sound.play();
					enemies_ss->move_position_to(j, glm::vec3(j * 2.0f, 0.0f, -50.0f));
				}
			}
		}
	}
}

void Player::reset(glm::vec3 new_position)
{
	this->bullets_shot = 0;
	move_position_to(new_position);
}

void Player::update(Enemies *enemies)
{
	for (unsigned int i = 0; i < bullets_shot; ++i)
	{
		bullets.move_position_by(i, glm::vec3(0, 0, bullet_speed));
		check_for_collision_with_enemies(enemies);
	}
}

void Player::shoot_bullet()
{
	if (bullets_shot < total_ammo) {
		bullets_shot += 1;
		shoot_bullet_sound.play();
	}
}

void Player::move_position_by(glm::vec3 delta)
{
	ss.move_position_by(0, delta);
	for (unsigned int i = bullets_shot; i < total_ammo; ++i) {
		bullets.move_position_to(i, ss.get_front_pos(0, false));
	}
}

void Player::move_position_to(glm::vec3 new_position)
{
	ss.move_position_to(0, new_position);
	for (unsigned int i = bullets_shot; i < total_ammo; ++i) {
		bullets.move_position_to(i, ss.get_front_pos(0, false));
	}
}

void Player::show(Enemies *enemies)
{
	ss.show();
	update(enemies);
	bullets.draw(GL_TRIANGLES);
}

void Player::change_scale(float factor)
{
	ss.change_scale(0, factor);
}

Spaceships* Player::get_spaceships()
{
	return &ss;
}

glm::vec3 Player::get_front_position()
{
	return ss.get_front_pos(0, false);
}

void Player::init(GLuint program_id,
	std::string player_model_filename, glm::vec3 player_position, Color player_color,
	std::string bullet_model_filename, Color bullet_color)
{
	ss.init(player_model_filename, program_id);
	ss.add_new_spaceship(player_position, player_color);

	bullets.init(bullet_model_filename, program_id);
	this->bullet_color = bullet_color;
	this->bullet_speed = 0.05f;

	this->bullets_shot = 0;
	for (unsigned int i = 0; i < total_ammo; ++i)
	{
		bullets.add(ss.get_front_pos(0, false), bullet_color);
		bullets.change_scale(i, 0.025f);
	}

	/* Sound Initialization */
	if (!shoot_bullet_sound_buffer.loadFromFile("data\\wav\\short_laser.wav")) {
		std::cout << "Error: Could not load file: data\\wav\\short_laser.wav" << std::endl;
	}
	else {
		shoot_bullet_sound.setBuffer(shoot_bullet_sound_buffer);
	}
	if (!enemy_hit_sound_buffer.loadFromFile("data\\wav\\enemy_hit.wav")) {
		std::cout << "Error: Could not load file: data\\wav\\enemy_hit.wav" << std::endl;
	}
	else {
		enemy_hit_sound.setBuffer(enemy_hit_sound_buffer);
	}
	/* Sound Initialization */
}

void Spaceships::show() 
{
	spaceship_im.draw(GL_TRIANGLES);
}

void Enemies::reset(std::vector<glm::vec3> new_enemy_positions)
{
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		enemy_destroyed[i] = false;
		ss.move_position_to(i, new_enemy_positions[i]);
		bullets_shot[i] = 0;
		particle_effects[i].reset(new_enemy_positions[i]);
		
		for (unsigned int j = 0; j < total_ammo; ++j) {
			bullets[i].move_position_to(j, ss.get_front_pos(i, true));
		}
	}
}

bool Enemies::are_all_enemies_destroyed()
{
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i) {
		if (enemy_destroyed[i] == false) return false;
	}

	return true;
}

void Enemies::init(GLuint program_id, std::string enemy_model_filename,
	std::string bullet_model_filename, Color bullet_color,
	std::string particle_effect_model_filename, GLuint particle_effect_program_id)
{
	ss.init(enemy_model_filename, program_id);
	bullets = (InstancedModel*)malloc(sizeof(InstancedModel) * MAX_INSTANCED_MODELS);
	particle_effects = (ParticleEffect*)malloc(sizeof(ParticleEffect) * MAX_INSTANCED_MODELS);
	this->bullets_shot = (unsigned int*)malloc(sizeof(unsigned int) * MAX_INSTANCED_MODELS);
	this->enemy_destroyed = (bool*)malloc(sizeof(bool) * MAX_INSTANCED_MODELS);
	this->bullet_color = bullet_color;
	this->bullet_speed = -0.005f;
	this->particle_effect_model_filename = particle_effect_model_filename;
	this->particle_effect_program_id = particle_effect_program_id;

	if (bullets)
		for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
			bullets[i].init(bullet_model_filename, program_id);
	else
		std::cout << "ERROR: Unable to allocate memory for bullets!!!" << std::endl;

	if (!particle_effects)
		std::cout << "ERROR: Unable to allocate memory for particle_effects!!!" << std::endl;

	if (enemy_destroyed) {
		for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
			enemy_destroyed[i] = false;
	}
	else
		std::cout << "ERROR: Unable to allocate memory for enemy_destroyed" << std::endl;
}

void Enemies::start_particle_effect_for(int index)
{
	particle_effects[index].start();
}

void Enemies::add(glm::vec3 position, Color enemy_color, Color particle_color)
{
	ss.add_new_spaceship(position, enemy_color);
	unsigned int index = ss.get_current_num_spaceships() - 1;
	ss.rotate(index, glm::vec3(0, 1, 0), 180);

	particle_effects[index].init(particle_effect_model_filename, particle_effect_program_id,
		position, particle_color);

	for (unsigned int i = 0; i < total_ammo; ++i)
	{
		bullets[index].add(ss.get_front_pos(index, true), bullet_color);
		bullets_shot[index] = 0;
	}
}

void Enemies::check_if_hit_player(Spaceships* player_ss)
{
	glm::vec3 player_position = player_ss->get_position(0);
	glm::vec3 player_dims = player_ss->get_dims(0);

	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		for (unsigned int j = 0; j < bullets_shot[i]; ++j)
		{
			glm::vec3 bullet_position = bullets[i].get_position(j);
			glm::vec3 bullet_dims = bullets[i].get_dims(j);
			glm::vec3 bullet_max_dims = bullet_position + (0.5f * bullet_dims);
			glm::vec3 bullet_min_dims = bullet_position - (0.5f * bullet_dims);

			glm::vec3 player_position = player_ss->get_position(0);
			glm::vec3 player_dims = player_ss->get_dims(0);
			glm::vec3 player_max_pos = player_position + (0.5f * player_dims);
			glm::vec3 player_min_pos = player_position - (0.5f * player_dims);

			if (BoxBoxIntersection(bullet_max_dims, bullet_min_dims, player_min_pos, player_max_pos))
			{
				std::cout << "Collision between bullet " << j << " and player from enemy " << i << std::endl;
				bullets[i].move_position_to(i, glm::vec3(i, 0.0f, 0.0f));
			}
		}
	}
}

void Enemies::shoot_bullets()
{
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		if ((rand() % 100 > 60) && bullets_shot[i] < total_ammo)
		{
			bullets_shot[i] += 1;
		}
	}
}

void Enemies::update(float current_global_tick, Spaceships *player_ss)
{
	static float local_tick = 0;
	if (current_global_tick - local_tick >= 2.0f) {
		local_tick = current_global_tick;
		shoot_bullets();
	}

	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		for (unsigned int j = 0; j < bullets_shot[i]; ++j)
			bullets[i].move_position_by(j, glm::vec3(0, 0, bullet_speed));
	}

	check_if_hit_player(player_ss);
}

void Enemies::move_position_of_all_by(glm::vec3 delta)
{
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		ss.move_position_by(i, delta);
		particle_effects[i].move_position_of_all_by(delta);
		for (unsigned int j = bullets_shot[i]; j < total_ammo; ++j)
		{
			bullets[i].move_position_to(j, ss.get_front_pos(i, true));
		}
	}
}

void Enemies::change_scale_of_all(float enemy_scale_factor, float bullet_scale_factor)
{
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		ss.change_scale(i, enemy_scale_factor);
		for (unsigned int j = 0; j < total_ammo; ++j)
		{
			bullets[i].change_scale(j, bullet_scale_factor);
		}
	}
}

void Enemies::show(float current_global_tick, Spaceships *player_ss)
{
	update(current_global_tick, player_ss);
	ss.show();
	
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i) {
		particle_effects[i].show(current_global_tick);
		bullets[i].draw(GL_TRIANGLES);
	}
}

void Enemies::move_position_by(int index, glm::vec3 delta)
{
	ss.move_position_by(index, delta);
}

bool BoxBoxIntersection(glm::vec3 objectMin, glm::vec3 objectMax, glm::vec3 colliderMin, glm::vec3 colliderMax)
{
	return ((objectMin.x <= colliderMax.x && objectMax.x >= colliderMin.x) &&
		(objectMin.y <= colliderMax.y && objectMax.y >= colliderMin.y) &&
		(objectMin.z <= colliderMax.z && objectMax.z >= colliderMin.z));
}