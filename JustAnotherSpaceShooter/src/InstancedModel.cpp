#include "InstancedModel.h"
#include <stdint.h>
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

float* get_model_data_from_file(const char* filename, glm::vec3 *extreme_mins, glm::vec3 *extreme_maxs, 
                                int *num_floats) {
	FILE *file = NULL;
	file = fopen(filename, "r");
	if(file == NULL) {
		printf("ERROR get_model_data_from_file(): Could not open file %s\n", filename);
        return nullptr;
	}

	char num_sub_models_str[25];
	fscanf(file, "%s", num_sub_models_str);
	uint16_t num_sub_models;
	fscanf(file, "%hu", &num_sub_models);

	char num_vertex_floats_str[25];
	fscanf(file, "%s", num_vertex_floats_str);
	fscanf(file, "%d", num_floats);

	char extremes_str[25];
	fscanf(file, "%s", extremes_str);
	fscanf(file, "%f", &extreme_mins->x);
	fscanf(file, "%f", &extreme_maxs->x);
	fscanf(file, "%f", &extreme_mins->y);
	fscanf(file, "%f", &extreme_maxs->y);
	fscanf(file, "%f", &extreme_mins->z);
	fscanf(file, "%f", &extreme_maxs->z);

    float *vertices = (float*)malloc(sizeof(float) * (*num_floats));
	for(int i = 0; i < *num_floats; ++i)
		fscanf(file, "%f", &vertices[i]);
    
    return vertices;
}

void InstancedModel::init(std::string filename, GLuint program_id)
{
    glm::vec3 extreme_mins, extreme_maxes;
    int num_floats;
    float *floats = get_model_data_from_file(filename.c_str(), &extreme_mins, &extreme_maxes, &num_floats);

    this->program_id = program_id;
    this->current_length = 0;
    this->num_vertices = (int) (num_floats / 6.0f); // @Note: 3 for position, 3 for normals
    this->models = (glm::mat4*)malloc(sizeof(glm::mat4) * MAX_INSTANCED_MODELS);
    this->positions = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
    this->Ka = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
    this->Kd = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
    this->Ks = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
    this->La = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
    this->Ld = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
    this->Ls = (glm::vec3*)malloc(sizeof(glm::vec3) * MAX_INSTANCED_MODELS);
    this->shininess = (float*)malloc(sizeof(float) * MAX_INSTANCED_MODELS);

    glm::vec3 diff = extreme_maxes - extreme_mins;
    float w = std::max(diff.x, std::max(diff.y, diff.z));
    this->scale_factor = 1.0f / w;

    for(unsigned int i = 0; i < MAX_INSTANCED_MODELS; ++i)
    {
        models[i] = glm::mat4(1.0f);
        positions[i] = glm::vec3(0.0f);
        La[i] = glm::vec3(0.1745f, 0.01175f, 0.01175f);
        Ld[i] = glm::vec3(0.614f, 0.041f, 0.041f);
        Ls[i] = glm::vec3(0.727f, 0.626f, 0.626f);
        Ka[i] = glm::vec3(1.0f);
        Kd[i] = glm::vec3(1.0f);
        Ks[i] = glm::vec3(1.0f);
        shininess[i] = 32.0f;
    }

    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_floats, floats, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(floats), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(floats), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    {
        glGenBuffers(1, &instanced_model_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_model_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * MAX_INSTANCED_MODELS, models, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &instanced_Ka_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_Ka_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_INSTANCED_MODELS, Ka, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &instanced_Kd_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_Kd_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_INSTANCED_MODELS, Kd, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &instanced_Ks_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_Ks_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_INSTANCED_MODELS, Ks, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &instanced_La_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_La_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_INSTANCED_MODELS, La, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &instanced_Ld_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_Ld_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_INSTANCED_MODELS, Ld, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &instanced_Ls_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_Ls_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_INSTANCED_MODELS, Ls, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &instanced_shininess_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, instanced_shininess_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MAX_INSTANCED_MODELS, shininess, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    {
        glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_model_vbo);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(2, 1);

		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_model_vbo);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(4 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(3, 1);

		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_model_vbo);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(4, 1);

		glEnableVertexAttribArray(5);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_model_vbo);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(5, 1);

		glEnableVertexAttribArray(6);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_Ka_vbo);
		glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(6, 1);

		glEnableVertexAttribArray(7);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_Kd_vbo);
		glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(7, 1);

		glEnableVertexAttribArray(8);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_Ks_vbo);
		glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(8, 1);

		glEnableVertexAttribArray(9);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_La_vbo);
		glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(9, 1);

		glEnableVertexAttribArray(10);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_Ld_vbo);
		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(10, 1);

		glEnableVertexAttribArray(11);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_Ls_vbo);
		glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(11, 1);

		glEnableVertexAttribArray(12);
		glBindBuffer(GL_ARRAY_BUFFER, instanced_shininess_vbo);
		glVertexAttribPointer(12, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(12, 1);
    }

    free(floats);
    std::cout << "Loaded model " << filename << " successfully " << std::endl;
}

void InstancedModel::add(glm::vec3 position, Color color)
{
    positions[current_length] = position;
    Ka[current_length] = color.Ka;
    Kd[current_length] = color.Kd;
    Ks[current_length] = color.Ks;
    La[current_length] = color.La;
    Ld[current_length] = color.Ld;
    Ls[current_length] = color.Ls;
    shininess[current_length] = color.shininess;

    current_length += 1;
}

void InstancedModel::draw()
{
    glUseProgram(program_id);

    glm::vec3 tmp_scale = glm::vec3(1, 1, 1);
    glm::vec3 tmp_rotation_axis = glm::vec3(0, 0, 1);
    for (int i = 0; i < current_length; ++i)
    {
        models[i] = glm::mat4(1.0f);
        models[i] = glm::translate(models[i], positions[i]);
        models[i] = glm::scale(models[i], tmp_scale * scale_factor);
        models[i] = glm::rotate(models[i], glm::radians(0.0f), tmp_rotation_axis);
    }

    /* Updating ModelVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_model_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4) * current_length, models);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating ModelVBO */

    /* Updating KaVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_Ka_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * current_length, Ka);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating KaVBO */

    /* Updating KdVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_Kd_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * current_length, Kd);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating KdVBO */

    /* Updating KsVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_Ks_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * current_length, Ks);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating KsVBO */

    /* Updating LaVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_La_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * current_length, La);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating LaVBO */

    /* Updating LdVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_Ld_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * current_length, Ld);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating LdVBO */

    /* Updating LsVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_Ls_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * current_length, Ls);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating LsVBO */

    /* Updating shininessVBO */
    glBindBuffer(GL_ARRAY_BUFFER, instanced_shininess_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * current_length, shininess);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /* Updating shininessVBO */

    glBindVertexArray(vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, num_vertices, current_length);
}