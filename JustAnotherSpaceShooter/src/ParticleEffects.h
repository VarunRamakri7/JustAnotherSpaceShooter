#pragma once
#include "InstancedModel.h"


class ParticleEffect
{
	InstancedModel particles;
	std::vector<glm::vec3> velocities;

public:
	void init(std::string model_filename, GLuint program_id,
		glm::vec3 position, Color color);
	void show(float current_global_tick);
	void update(float current_global_tick);
};