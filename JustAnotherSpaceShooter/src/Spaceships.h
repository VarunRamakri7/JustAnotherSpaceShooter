#pragma once

#include "InstancedModel.h"
#include <vector>

class Spaceships {
private:
	InstancedModel im;
	std::vector<float> health;
	std::vector<float> speed;
	std::vector<unsigned int> ammo;
public:
	void show();
	void init(std::string model_filename, GLuint program_id);
	void add_new_spaceship(glm::vec3 position, Color color);
	void move(int id, glm::vec3 delta);
};