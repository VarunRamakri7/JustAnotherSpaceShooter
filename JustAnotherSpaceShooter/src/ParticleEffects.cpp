#include "ParticleEffects.h"
#include <cmath>

#include <iostream> // @TMP

void ParticleEffect::init(std::string model_filename, GLuint program_id,
	glm::vec3 position, Color color)
{
	particles.init(model_filename, program_id);
	this->initial_position = position;
	this->velocities = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
	this->blast_upward_count = 0;
	this->start_effect = false;
	this->blast_upwards = false;
	this->local_tick = 0;

	for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
		particles.add(position, color);

	float speed = 0.003f;
	float phi = 3.14f * (3 - sqrt(5));
	float into_rad = 3.14f / 180.0f;
	for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
	{
		glm::vec3 vel;
		vel.y = 1 - (i / (float)(MAX_INSTANCED_MODELS - 1)) * 2;
		float radius = sqrt(1 - vel.y * vel.y);
		float theta = phi * i;
		vel.x = cos(theta) * radius;
		vel.z = sin(theta) * radius;
		velocities[i] = vel * speed;
	}
}

void ParticleEffect::start()
{
	start_effect = true;
}

void ParticleEffect::update(float current_global_tick)
{
	if (blast_upward_count <= 100 && start_effect == true) {
		if (current_global_tick - local_tick >= 0.01f) {
			local_tick = current_global_tick;

			for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
				particles.move_position_by(i, velocities[i]);

			if (blast_upwards == true)
				blast_upward_count += 1;

			if (glm::distance(particles.get_position(0), initial_position) >= 0.05f
				&& blast_upwards == false) {
				blast_upwards = true;
				for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
					velocities[i] *= -2;
			}
			else if (glm::distance(particles.get_position(0), initial_position) <= 0.01f
				&& blast_upwards == true) {
				for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i) {
					double val_x = ((double)rand() / (double)RAND_MAX) * 2 - 1;
					double val_y = ((double)rand() / (double)RAND_MAX) * 2 - 1;
					velocities[i] = glm::vec3(val_x * 0.1f, val_y * 0.05f, 0);
				}
			}
		}
	}
}

void ParticleEffect::move_position_of_all_by(glm::vec3 delta)
{
	for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
		particles.move_position_by(i, delta);
	
	initial_position += delta;
}

void ParticleEffect::set_initial_position(glm::vec3 initial_position)
{
	this->initial_position = initial_position;
}

void ParticleEffect::show(float current_global_tick)
{
	if (blast_upward_count <= 100 && start_effect == true) {
		update(current_global_tick);
		
		glPointSize(5.0f);
		particles.draw(GL_POINTS);
		glPointSize(1.0f);
	}
}