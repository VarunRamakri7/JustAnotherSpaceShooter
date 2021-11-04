#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"

// Default window dimensions
#define WIDTH 1024
#define HEIGHT 768

// Mesh related data
MeshData spaceship_mesh;
MeshData terrainStart_mesh;
GLuint spaceship_shader = -1;
GLuint terrain_shader = -1;

// Window related data
glm::vec2 window_dims = glm::vec2(WIDTH, HEIGHT);
float aspectRatio = (float)window_dims.x / window_dims.y; // Window aspect ratio

// Camera related data
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 0.1f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
bool firstMouse = true;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  window_dims.x / 2.0;
float lastY =  window_dims.y / 2.0;
float fov   =  45.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// World Related data
const glm::vec3 origin = glm::vec3(0.0f);
float moveFactor = 50.0f;
float scaleFactor = 1.0f;

// Spaceship related
glm::vec3 spaceship_pos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 spaceship_scale = glm::vec3(1.0f);
glm::vec3 spaceship_rot = glm::vec3(0.0f, 1.0f, 0.0f);

// Terrain related data
glm::vec3 terrainStart_pos = glm::vec3(0.0f, -0.3f, 5.0f);
glm::vec3 terrainStart_scale = glm::vec3(1.0f);
glm::vec3 terrainStart_rot = glm::vec3(0.0f, 1.0f, 0.0f);

void init_game()
{
    /* Shader initialization */
    {
        std::string shader_folder = "shaders\\";

        // Spaceship Shader
        std::string spaceship_vs_fname = shader_folder + "spaceship_vs.glsl";
        std::string spaceship_fs_fname = shader_folder + "spaceship_fs.glsl";
        spaceship_shader = InitShader(spaceship_vs_fname.c_str(), spaceship_fs_fname.c_str());
    
        // Terrain Shader
        std::string terrain_vs_fname = shader_folder + "terrain_vs.glsl";
        std::string terrain_fs_fname = shader_folder + "terrain_fs.glsl";
        terrain_shader = InitShader(terrain_vs_fname.c_str(), terrain_fs_fname.c_str());
    }

    /* Model initialization */
    {
        std::string model_folder = "data\\models\\";
        
        // Get spaceship
        std::string spaceship_fname = model_folder + "spaceship_main.obj";
        spaceship_mesh = LoadMesh(spaceship_fname);
        std::cout << "Spaceship Scale Factor: " << spaceship_mesh.mScaleFactor << std::endl;

        // Get Terrain
        std::string terrainStart_fname = model_folder + "terrain_start.obj";
        terrainStart_mesh = LoadMesh(terrainStart_fname);
        std::cout << "Terrain Scale Factor: " << terrainStart_mesh.mScaleFactor << std::endl;
    }

    /* Texture initialization */
    {
    }
}

void display(GLFWwindow* window)
{
    /// TODO: Fix mesh scales

    // Set draw mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Setup Orthographic view
    glm::mat4 view = glm::lookAt(cameraPos, spaceship_pos, cameraUp);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);

    // Spaceship matrix
    glm::mat4 spaceship_matrix = glm::mat4(1.0f);
    spaceship_matrix = glm::translate(spaceship_matrix, spaceship_pos);
    spaceship_matrix = glm::scale(spaceship_matrix, spaceship_scale);
    spaceship_matrix = glm::rotate(spaceship_matrix, glm::radians(0.0f), spaceship_rot);

    // Terrain Matrices
    glm::mat4 terrainStart_matrix = glm::mat4(1.0f);
    terrainStart_matrix = glm::translate(terrainStart_matrix, terrainStart_pos);
    terrainStart_matrix = glm::scale(terrainStart_matrix, terrainStart_scale);
    terrainStart_matrix = glm::rotate(terrainStart_matrix, glm::radians(0.0f), terrainStart_rot);

    glUseProgram(spaceship_shader);
    glUseProgram(terrain_shader);

    int projection_loc = glGetUniformLocation(spaceship_shader, "projection");
    glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));
    
    int view_loc = glGetUniformLocation(spaceship_shader, "view");
    glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
    
    int spaceship_loc = glGetUniformLocation(spaceship_shader, "spaceship");
    glUniformMatrix4fv(spaceship_loc, 1, false, glm::value_ptr(spaceship_matrix));

    int terrainStart_loc = glGetUniformLocation(terrain_shader, "terrainStart");
    glUniformMatrix4fv(terrainStart_loc, 1, false, glm::value_ptr(terrainStart_matrix));

    glBindVertexArray(spaceship_mesh.mVao);
    glDrawElements(GL_TRIANGLES, spaceship_mesh.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(terrainStart_mesh.mVao);
    glDrawElements(GL_TRIANGLES, terrainStart_mesh.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
}

void idle()
{

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    window_dims = glm::vec2(width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 1.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    std::cout << "key : " << key << ", " << char(key) << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << std::endl;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
            // Move front
            case 'w':
            case 'W':
                spaceship_pos.z += (moveFactor * deltaTime);
                break;

            // Move back
            case 's':
            case 'S':
                spaceship_pos.z -= (moveFactor * deltaTime);
                break;

            // Move left
            case 'a':
            case 'A':
                spaceship_pos.x -= (moveFactor * deltaTime);
                break;

            // Move right
            case 'd':
            case 'D':
                spaceship_pos.x += (moveFactor * deltaTime);
                break;

            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}

void mouse_cursor(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void mouse_button(GLFWwindow* window, int button, int action, int mods)
{
}

void mouse_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    switch ((int)yoffset)
    {
        case 1: // Scroll Up
            scaleFactor++; // Increase scale
            break;

        case -1: // Scroll Down
            scaleFactor *= 0.5f; // Decrease scale
            break;
    }

    // Update scales
    spaceship_scale *= scaleFactor;
    terrainStart_scale *= scaleFactor;

    // Print info
    std::printf("New scale factor: %0.3f\n", scaleFactor);
    std::printf("New Spaceship Scale: (%.9f, %.9f, %.9f)\n", spaceship_scale.x, spaceship_scale.y, spaceship_scale.z);
    std::printf("New Terrain Scale: (%.2f, %.2f, %.2f)\n", terrainStart_scale.x, terrainStart_scale.y, terrainStart_scale.z);
}

void resize(GLFWwindow* window, int width, int height)
{
    std::printf("New window dimensions: (%.2u, %.2u)\n", width, height);

    // Prevent dividing by zero
    if (height == 0)
    {
        height = 1;
    }

    // Preserve aspect ratio and update viewport for resizing
    aspectRatio = (float)width / height; // Update aspect ratio

    // Update window dimensions
    window_dims = glm::vec2(width, height);

    // Fit viewport to window
    glViewport(0, 0, width, height);
}

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);

    const char* window_name = "Just Another Space Shooter";
    GLFWwindow* window = glfwCreateWindow(window_dims.x, window_dims.y, window_name, NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetKeyCallback(window, keyboard);
    //glfwSetCursorPosCallback(window, mouse_cursor);
    glfwSetMouseButtonCallback(window, mouse_button);
    glfwSetScrollCallback(window, mouse_scroll);
    glfwSetWindowSizeCallback(window, resize);
    glEnable(GL_DEPTH_TEST);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    init_game();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

        //processInput(window);

        display(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}