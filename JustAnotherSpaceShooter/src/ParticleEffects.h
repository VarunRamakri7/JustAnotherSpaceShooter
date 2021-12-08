#pragma once
#include "InstancedModel.h"

class ParticleEffect
{
	InstancedModel particles;
	glm::vec3 *velocities;
	glm::vec3 initial_position;
	bool blast_upwards;
	unsigned int blast_upward_count = 0;
	bool start_effect;
	float local_tick;

	void update(float current_global_tick);
public:
	void init(std::string model_filename, GLuint program_id,
		glm::vec3 position, Color color);
	void show(float current_global_tick);
	void start();
	void set_initial_position(glm::vec3 initial_position);
	void move_position_of_all_by(glm::vec3 delta);
	void reset(glm::vec3 new_initial_position);
};