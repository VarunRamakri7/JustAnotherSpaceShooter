#include "Spaceships.h"
#include <iostream>

void Spaceships::init(std::string model_filename, GLuint program_id) 
{
	spaceship_im.init(model_filename, program_id);
}

void Spaceships::add_new_spaceship(glm::vec3 position, Color color) 
{
	spaceship_im.add(position, color);
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

void Player::update()
{
	for (unsigned int i = 0; i < bullets_shot; ++i)
	{
		bullets.move_position_by(i, glm::vec3(0, 0, bullet_speed));
	}
}

void Player::shoot_bullet()
{
	if (bullets_shot < total_ammo)
		bullets_shot += 1;
}

void Player::move_position_by(glm::vec3 delta)
{
	ss.move_position_by(0, delta);
	for (unsigned int i = bullets_shot; i < total_ammo; ++i) {
		bullets.move_position_to(i, ss.get_front_pos(0, false));
	}
}

void Player::show()
{
	ss.show();
	update();
	bullets.draw();
}

void Player::change_scale(float factor)
{
	ss.change_scale(0, factor);
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
}

void Spaceships::show() 
{
	spaceship_im.draw();
}

void Enemies::init(GLuint program_id, std::string enemy_model_filename,
	std::string bullet_model_filename, Color bullet_color)
{
	ss.init(enemy_model_filename, program_id);
	bullets = (InstancedModel*)malloc(sizeof(InstancedModel) * MAX_INSTANCED_MODELS);
	this->bullets_shot = (unsigned int*)malloc(sizeof(unsigned int) * MAX_INSTANCED_MODELS);
	this->bullet_color = bullet_color;
	this->bullet_speed = 0.025f;

	if (bullets)
	{
		for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
		{
			bullets[i].init(bullet_model_filename, program_id);
		}
	}
	else
	{
		std::cout << "ERROR: Unable to allocate memory for bullets!!!" << std::endl;
	}
}

void Enemies::add(glm::vec3 position, Color color)
{
	ss.add_new_spaceship(position, color);
	unsigned int index = ss.get_current_num_spaceships() - 1;
	ss.rotate(index, glm::vec3(0, 1, 0), 180);

	for (unsigned int i = 0; i < total_ammo; ++i)
	{
		bullets[index].add(ss.get_front_pos(index, true), bullet_color);
		bullets_shot[index] = 0;
	}
}

void Enemies::shoot_bullets()
{
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		if ((rand() % 100 > 98) && bullets_shot[i] < total_ammo)
		{
			bullets_shot[i] += 1;
		}
	}
}

void Enemies::update()
{
	shoot_bullets();

	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		for (unsigned int j = 0; j < bullets_shot[i]; ++j)
		{
			bullets[i].move_position_by(j, glm::vec3(0, 0, bullet_speed));
		}
	}
}

void Enemies::move_position_of_all_by(glm::vec3 delta)
{
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		ss.move_position_by(i, delta);
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

void Enemies::show()
{
	update();
	ss.show();
	
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
		bullets[i].draw();
}

void Enemies::move_position_by(int index, glm::vec3 delta)
{
	ss.move_position_by(index, delta);
}