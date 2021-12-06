#include "ParticleEffects.h"

#include <iostream> // @TMP
#include <cmath>

void ParticleEffect::init(std::string model_filename, GLuint program_id,
	glm::vec3 position, Color color)
{
	particles.init(model_filename, program_id);

	for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
	{
		particles.add(position, color);
	}

	float speed = 0.001f;
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
		velocities.push_back(vel * speed);
	}
}

void ParticleEffect::update(float current_global_tick)
{
	static float local_tick = current_global_tick;
	if (current_global_tick - local_tick >= 0.05f) {
		local_tick = current_global_tick;

		for (unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i) {
			particles.move_position_by(i, velocities[i]);
		}

	}
}

void ParticleEffect::show(float current_global_tick)
{
	update(current_global_tick);
	particles.draw();
}