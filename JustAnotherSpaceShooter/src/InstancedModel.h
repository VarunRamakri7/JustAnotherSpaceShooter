#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#define MAX_INSTANCED_MODELS 100

struct Color {
    glm::vec3 Ka, La;
    glm::vec3 Kd, Ld;
    glm::vec3 Ks, Ls;
    float shininess;
};

class InstancedModel
{
    GLuint program_id;
	//GLuint texture_id;
    GLuint vbo;
    GLuint vao;
    GLuint instanced_model_vbo;
    GLuint instanced_Ka_vbo;
    GLuint instanced_Kd_vbo;
    GLuint instanced_Ks_vbo;
    GLuint instanced_La_vbo;
    GLuint instanced_Ld_vbo;
    GLuint instanced_Ls_vbo;
    GLuint instanced_shininess_vbo;
    GLuint current_length;

    GLuint num_vertices;
    float scale_factor;

    glm::mat4 *models;
    glm::vec3 *positions;
    glm::vec3 *Ka;
    glm::vec3 *Kd;
    glm::vec3 *Ks;
    glm::vec3 *La;
    glm::vec3 *Ld;
    glm::vec3 *Ls;
    float *shininess;

    public:
        // void init(std::string filename, GLuint program_id, GLuint texture_id);
        void init(std::string filename, GLuint program_id);
        void move_position(int index, glm::vec3 delta);
        void draw();
        void add(glm::vec3 position, Color color);
};
