#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

#define MAX_INSTANCED_MODELS 200

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
    glm::vec3 extreme_min;
    glm::vec3 extreme_max;
    bool canCollide;

    bool* should_hide; /* @Akash: Do not use this for now. */

    glm::mat4 *models;
    glm::vec3 *positions;
    glm::vec3 *scales;
    glm::vec3 *rotation_axes;
    float* angles;
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
        void move_position_by(int index, glm::vec3 delta);
        void move_position_to(int index, glm::vec3 new_position);
        glm::vec3 get_position(int index);
        glm::vec3 get_front_pos(int index, bool flip_on_y_180);
        void change_scale(int index, float factor);
        void change_scale(int index, glm::vec3 factor);
        void rotate(int index, glm::vec3 axes, float degree);
        float get_scale_factor();
        void draw(GLenum mode);
        void add(glm::vec3 position, Color color);
        glm::vec3 get_dims(int index);
        unsigned int get_current_length();
        void SetCollisionStatus(bool status);
        bool GetCollisionStatus();
};
