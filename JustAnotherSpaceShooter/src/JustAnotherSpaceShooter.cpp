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


GLuint model_shader = -1;
MeshData space_shooter_1;
glm::vec2 window_dims = glm::vec2(WIDTH, HEIGHT);
float aspectRatio = (float)window_dims.x / window_dims.y; // Window aspect ratio

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
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

void init_game()
{
    /* Shader initialization */
    {
        std::string shader_folder = "shaders\\";
        std::string model_shader_vertex_fname = shader_folder + "model_shader_vs.glsl";
        std::string model_shader_fragment_fname = shader_folder + "model_shader_fs.glsl";
        model_shader = InitShader(model_shader_vertex_fname.c_str(), model_shader_fragment_fname.c_str());
    }
    /* Shader initialization */

    /* Model initialization */
    {
        std::string model_folder = "data\\models\\";
        std::string space_shooter_1_fname = model_folder + "spaceship_main.obj";
        space_shooter_1 = LoadMesh(space_shooter_1_fname);
    }
    /* Model initialization */

    /* Texture initialization */
    {
    }
    /* Texture initialization */
}

void display(GLFWwindow* window)
{
    // Set draw mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Model transformations
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 rotation_axes = glm::vec3(0, 1, 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model - glm::scale(model, scale * space_shooter_1.mScaleFactor);
    model = glm::rotate(model, glm::radians(0.0f), rotation_axes);

    // glm::mat4 view = glm::lookAt(camera_position, camera_position + glm::vec3(0.0f, 0.0f, -1.0f),
    //                          glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    // glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    glm::mat4 projection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 0.1f, 1000.0f);

    glUseProgram(model_shader);

    int projection_loc = glGetUniformLocation(model_shader, "projection");
    glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));
    
    int view_loc = glGetUniformLocation(model_shader, "view");
    glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
    
    int model_loc = glGetUniformLocation(model_shader, "model");
    glUniformMatrix4fv(model_loc, 1, false, glm::value_ptr(model));
    
    glBindVertexArray(space_shooter_1.mVao);
    glDrawElements(GL_TRIANGLES, space_shooter_1.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
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

    float cameraSpeed = 3.5f * deltaTime;
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

        processInput(window);

        display(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
