#include "Spaceships.h"

void Spaceships::init(std::string model_filename, GLuint program_id) {
	spaceship_im.init(model_filename, program_id);
}

void Spaceships::add_new_spaceship(glm::vec3 position, Color color) {
	spaceship_im.add(position, color);
}

void Spaceships::change_scale(int index, float factor)
{
	spaceship_im.change_scale(index, factor);
}

glm::vec3 Spaceships::get_front_pos(int index)
{
	return spaceship_im.get_front_pos(index);
}

void Spaceships::move_position(int index, glm::vec3 delta)
{
	spaceship_im.move_position(index, delta);
}

void Spaceships::move(int id, glm::vec3 delta) {
	spaceship_im.move_position(id, delta);
}

void Spaceships::show() {
	spaceship_im.draw();
}