#pragma once

#include "InstancedModel.h"
#include <vector>

class Spaceships {
private:
	InstancedModel spaceship_im;
	std::vector<float> health;
	std::vector<float> speed;
public:
	void show();
	void init(std::string model_filename, GLuint program_id);
	void add_new_spaceship(glm::vec3 position, Color color);
	void change_scale(int index, float factor);
	void move_position(int index, glm::vec3 delta);
	glm::vec3 get_front_pos(int index);
	void move(int id, glm::vec3 delta);
};

class Player {
private:
	Spaceships ss;
	InstancedModel bullets;
	std::vector<unsigned int> ammo;
};

class Enemies {
private:
	Spaceships ss;
};