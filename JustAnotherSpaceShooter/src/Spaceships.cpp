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

// Check if player's bullets collide with the enemies
void Player::check_for_collion_with_enemies(Spaceships* ss)
{
	// Iterate through all the bullets shot by the player
	for (unsigned int i = 0; i < bullets_shot; ++i)
	{
		// Check if bullet has collision active
		if (bullets.GetCollisionStatus())
		{
			// Get position and dimensions of a bullet
			glm::vec3 bullet_position = bullets.get_position(i);
			glm::vec3 bullet_dims = bullets.get_dims(i);

			// Get extremeities of bullet
			glm::vec3 bullet_max_dims = bullet_position + (0.5f * bullet_dims);
			glm::vec3 bullet_min_dims = bullet_position - (0.5f * bullet_dims);

			// Iterate through all the enemies
			for (unsigned int j = 0; j < ss->get_current_num_spaceships(); ++j)
			{
				// Check if enemy has collision active
				if (ss->GetCollisionStatus())
				{
					// Get position and dimensions of an enemy
					glm::vec3 enemy_position = ss->get_position(j);
					glm::vec3 enemy_dims = ss->get_dims(j);

					// Get extremeities of enemy
					glm::vec3 enemy_max_dims = enemy_position + (0.5f * enemy_dims);
					glm::vec3 enemy_min_dims = enemy_position - (0.5f * enemy_dims);

					// Check if bullet and enemy collide (using box-box intersection)
					if (BoxBoxIntersection(bullet_max_dims, bullet_min_dims, enemy_min_dims, enemy_max_dims))
					{
						// Collision occurs
						std::cout << "Collision between bullet " << i << " and enemy " << j << std::endl;

						// Disable collision for bullet and enemy
						bullets.SetCollisionStatus(false);
						ss->SetCollisionStatus(false);

						// Move bullet and enemy behind the camera
						bullets.move_position_to(i, glm::vec3(i, 0.0f, 0.0f)); // Move bullet
						ss->move_position_to(j, glm::vec3(j * 2.0f, 0.0f, 50.0f)); // Move enemy
					}
				}
			}
		}
	}
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

void Enemies::check_if_hit_player(Spaceships* player_ss)
{
	glm::vec3 player_position = player_ss->get_position(0);
	glm::vec3 player_dims = player_ss->get_dims(0);

	// Iterate through all the enemies
	for (unsigned int i = 0; i < ss.get_current_num_spaceships(); ++i)
	{
		// Iterate through all the bullets shot by the enemy
		for (unsigned int j = 0; j < bullets_shot[i]; ++j)
		{
			// Get position and dimensions of a bullet
			glm::vec3 bullet_position = bullets[i].get_position(i);
			glm::vec3 bullet_dims = bullets[i].get_dims(i);

			// Get extremeities of bullet
			glm::vec3 bullet_max_dims = bullet_position + (0.5f * bullet_dims);
			glm::vec3 bullet_min_dims = bullet_position - (0.5f * bullet_dims);

			// Iterate through all the enemies
			for (unsigned int j = 0; j < player_ss->get_current_num_spaceships(); ++j)
			{
				// Check if enemy has collision active
				if (player_ss->GetCollisionStatus())
				{
					// Get position and dimensions of an enemy
					glm::vec3 enemy_position = player_ss->get_position(j);
					glm::vec3 enemy_dims = player_ss->get_dims(j);

					// Get extremeities of enemy
					glm::vec3 enemy_max_dims = enemy_position + (0.5f * enemy_dims);
					glm::vec3 enemy_min_dims = enemy_position - (0.5f * enemy_dims);

					// Check if bullet and enemy collide (using box-box intersection)
					if (BoxBoxIntersection(bullet_max_dims, bullet_min_dims, enemy_min_dims, enemy_max_dims))
					{
						// Collision occurs
						std::cout << "Collision between bullet " << i << " and player " << j << std::endl;

						// Disable collision for bullet
						bullets[i].SetCollisionStatus(false);

						// Move bullet and enemy behind the camera
						bullets[i].move_position_to(i, glm::vec3(i, 0.0f, 0.0f)); // Move bullet
						// TODO: Reduce player health
					}
				}
			}
		}
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

bool BoxBoxIntersection(glm::vec3 objectMin, glm::vec3 objectMax, glm::vec3 colliderMin, glm::vec3 colliderMax)
{
	return ((objectMin.x <= colliderMax.x && objectMax.x >= colliderMin.x) &&
		(objectMin.y <= colliderMax.y && objectMax.y >= colliderMin.y) &&
		(objectMin.z <= colliderMax.z && objectMax.z >= colliderMin.z));
}