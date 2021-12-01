#include "Spaceships.h"

void Spaceships::init(std::string model_filename, GLuint program_id) {
	im.init(model_filename, program_id);
}

void Spaceships::add_new_spaceship(glm::vec3 position, Color color) {
	im.add(position, color);
}

void Spaceships::move(int id, glm::vec3 delta) {
	im.move_position(id, delta);
}

void Spaceships::show() {
	im.draw();
}