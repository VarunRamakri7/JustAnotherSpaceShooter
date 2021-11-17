#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

#include "InitShader.h"
#include "LoadMesh.h"
#include "LoadTexture.h"
#include "InstancedModel.h"

Color terrain_color;
Color spaceship_color;
glm::vec3 light_position_1;
glm::vec3 light_direction_1;

// Default window dimensions
#define WIDTH 1024
#define HEIGHT 768

// Mesh related data
//MeshData spaceship_mesh;
//MeshData terrainStart_mesh;
//GLuint spaceship_shader = -1;
//GLuint terrain_shader = -1;

GLuint instanced_model_shader = -1;
InstancedModel im_1;
InstancedModel im_2;

// Window related data
glm::vec2 window_dims = glm::vec2(WIDTH, HEIGHT);
float aspectRatio = (float)window_dims.x / window_dims.y; // Window aspect ratio

// Camera related data
glm::vec3 camera_pos   = glm::vec3(-2.0f, 2.0f, 0.1f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f, 0.0f);

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
glm::vec3 spaceship_pos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 spaceship_scale = glm::vec3(1.0f);
glm::vec3 spaceship_rot = glm::vec3(0.0f, 1.0f, 0.0f);

// Terrain related data
glm::vec3 terrainStart_pos = glm::vec3(0.0f, -0.5f, 0.0f);
glm::vec3 terrainStart_scale = glm::vec3(3.0f);
glm::vec3 terrainStart_rot = glm::vec3(0.0f, 1.0f, 0.0f);

void init_game()
{
    /* Shader initialization */
    {
        std::string shader_folder = "shaders\\";

        // Spaceship Shader
        /*
        std::string spaceship_vs_fname = shader_folder + "spaceship_vs.glsl";
        std::string spaceship_fs_fname = shader_folder + "spaceship_fs.glsl";
        spaceship_shader = InitShader(spaceship_vs_fname.c_str(), spaceship_fs_fname.c_str());
    
        // Terrain Shader
        std::string terrain_vs_fname = shader_folder + "terrain_vs.glsl";
        std::string terrain_fs_fname = shader_folder + "terrain_fs.glsl";
        terrain_shader = InitShader(terrain_vs_fname.c_str(), terrain_fs_fname.c_str());
        */

        // Model Shader (Testing)
        std::string model_vs_fname = shader_folder + "instanced_model_vs.glsl";
        std::string model_fs_fname = shader_folder + "instanced_model_fs.glsl";
        instanced_model_shader = InitShader(model_vs_fname.c_str(), model_fs_fname.c_str());
    }

    /* Colors initialization */
    {
        terrain_color.La = glm::vec3(0.2125f, 0.1275f, 0.054f);
        terrain_color.Ld = glm::vec3(0.714f, 0.428f, 0.181f);
        terrain_color.Ls = glm::vec3(0.393f, 0.271f, 0.166f);
        terrain_color.Ka = glm::vec3(1.0f);
        terrain_color.Kd = glm::vec3(1.0f);
        terrain_color.Ks = glm::vec3(1.0f);
        terrain_color.shininess = 32.0f;

        spaceship_color.La = glm::vec3(0.1745f, 0.01175f, 0.01175f);
        spaceship_color.Ld = glm::vec3(0.614f, 0.041f, 0.041f);
        spaceship_color.Ls = glm::vec3(0.727f, 0.626f, 0.626f);
        spaceship_color.Ka = glm::vec3(1.0f);
        spaceship_color.Kd = glm::vec3(1.0f);
        spaceship_color.Ks = glm::vec3(1.0f);
        spaceship_color.shininess = 32.0f;
    }

    /* Model initialization */
    {
        std::string model_folder = "data\\models\\";
        
        // Get spaceship
        /*
        std::string spaceship_fname = model_folder + "spaceship_main.obj";
        spaceship_mesh = LoadMesh(spaceship_fname);
        std::cout << "Spaceship Scale Factor: " << spaceship_mesh.mScaleFactor << std::endl;

        // Get Terrain
        std::string terrainStart_fname = model_folder + "terrain_start.obj";
        terrainStart_mesh = LoadMesh(terrainStart_fname);
        std::cout << "Terrain Scale Factor: " << terrainStart_mesh.mScaleFactor << std::endl;
        */

        // spaceship_model
        std::string tmp_fname_1 = model_folder + "spaceship_test.model";
        im_1.init(tmp_fname_1, instanced_model_shader);
        im_1.add(glm::vec3(0, 0, 0), spaceship_color);
        im_1.add(glm::vec3(2, 0, 0), terrain_color);
        im_1.add(glm::vec3(-2, 0, 0), spaceship_color);
        im_1.add(glm::vec3(-4, 0, 0), terrain_color);

        // terrain_model
        std::string tmp_fname_2 = model_folder + "terrain_start_test.model";
        im_2.init(tmp_fname_2, instanced_model_shader);
        im_2.add(glm::vec3(0, -2, 0), spaceship_color);
        im_2.add(glm::vec3(2, -2, 0), terrain_color);
        im_2.add(glm::vec3(-2, -2, 0), spaceship_color);
        im_2.add(glm::vec3(-4, -2, 0), terrain_color);
    }

    /* Texture initialization */
    {
    }

    /* Others */
    light_position_1 = glm::vec3(0.0f, 2.0f, 0.0f);
    light_direction_1 = glm::vec3(0.0f, -1.0f, 0.0f);
}

void draw_gui(GLFWwindow *window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /*
    ImGui::Begin("Debug Game");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetWindowSize(ImVec2(700, 350));
    {
    }
    ImGui::End();
    */

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void display(GLFWwindow *window)
{
    // Set draw mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Setup Orthographic view
    glm::vec3 camLookAt = glm::vec3(0.0f, 0.0f, spaceship_pos.z);
    glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    // glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    glm::mat4 projection = glm::perspective(fov, window_dims.x / window_dims.y, 0.1f, 100.0f);

    // Spaceship matrix
    /*
    glm::mat4 spaceship_matrix = glm::mat4(1.0f);
    spaceship_matrix = glm::translate(spaceship_matrix, spaceship_pos);
    spaceship_matrix = glm::scale(spaceship_matrix, spaceship_scale * spaceship_mesh.mScaleFactor
    spaceship_matrix = glm::rotate(spaceship_matrix, glm::radians(0.0f), spaceship_rot);

    // Terrain Matrices
    glm::mat4 terrainStart_matrix = glm::mat4(1.0f);
    terrainStart_matrix = glm::translate(terrainStart_matrix, terrainStart_pos);
    terrainStart_matrix = glm::scale(terrainStart_matrix, terrainStart_scale * terrainStart_mesh.mScaleFactor * scaleFactor);
    terrainStart_matrix = glm::rotate(terrainStart_matrix, glm::radians(0.0f), terrainStart_rot);
    */

    // TMP Matrix
    // glm::mat4 model_matrix = glm::mat4(1.0f);
    // glm::vec3 tmp_pos = glm::vec3(0, 0, 5.0f);
    // model_matrix = glm::translate(model_matrix, tmp_pos);
    // model_matrix = glm::scale(model_matrix, spaceship_scale * spaceship_mesh.mScaleFactor /** scaleFactor*/);
    // model_matrix = glm::rotate(model_matrix, glm::radians(0.0f), spaceship_rot);

    {
        //glUseProgram(spaceship_shader);
        //int projection_loc = glGetUniformLocation(spaceship_shader, "s_projection");
        //int view_loc = glGetUniformLocation(spaceship_shader, "s_view");
        //int model_loc = glGetUniformLocation(spaceship_shader, "model");
        //int lightDirection_loc = glGetUniformLocation(spaceship_shader, "light_direction");
        //int viewPosition_loc = glGetUniformLocation(spaceship_shader, "view_position");
        //int Ka_loc = glGetUniformLocation(spaceship_shader, "Ka");
        //int Kd_loc = glGetUniformLocation(spaceship_shader, "Kd");
        //int Ks_loc = glGetUniformLocation(spaceship_shader, "Ks");
        //int La_loc = glGetUniformLocation(spaceship_shader, "La");
        //int Ld_loc = glGetUniformLocation(spaceship_shader, "Ld");
        //int Ls_loc = glGetUniformLocation(spaceship_shader, "Ls");
        //int shininess_loc = glGetUniformLocation(spaceship_shader, "shininess");
        //glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));
        //glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
        //glUniformMatrix4fv(model_loc, 1, false, glm::value_ptr(spaceship_matrix));
        //glUniform3fv(lightDirection_loc, 1, glm::value_ptr(light_direction_1));
        //glUniform3fv(viewPosition_loc, 1, glm::value_ptr(camera_pos));
        //glUniform3fv(Ka_loc, 1, glm::value_ptr(spaceship_color.Ka));
        //glUniform3fv(Kd_loc, 1, glm::value_ptr(spaceship_color.Kd));
        //glUniform3fv(Ks_loc, 1, glm::value_ptr(spaceship_color.Ks));
        //glUniform3fv(La_loc, 1, glm::value_ptr(spaceship_color.La));
        //glUniform3fv(Ld_loc, 1, glm::value_ptr(spaceship_color.Ld));
        //glUniform3fv(Ls_loc, 1, glm::value_ptr(spaceship_color.Ls));
        //glUniform1f(shininess_loc, spaceship_color.shininess);
        //glBindVertexArray(spaceship_mesh.mVao);
        //glDrawElements(GL_TRIANGLES, spaceship_mesh.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
    }

    {
        /*
        glUseProgram(terrain_shader);
        int t_projection_loc = glGetUniformLocation(terrain_shader, "t_projection");
        int t_view_loc = glGetUniformLocation(terrain_shader, "t_view");
        int terrainStart_loc = glGetUniformLocation(terrain_shader, "terrainStart");
        int lightDirection_loc = glGetUniformLocation(terrain_shader, "light_direction");
        int viewPosition_loc = glGetUniformLocation(terrain_shader, "view_position");
        int Ka_loc = glGetUniformLocation(terrain_shader, "Ka");
        int Kd_loc = glGetUniformLocation(terrain_shader, "Kd");
        int Ks_loc = glGetUniformLocation(terrain_shader, "Ks");
        int La_loc = glGetUniformLocation(terrain_shader, "La");
        int Ld_loc = glGetUniformLocation(terrain_shader, "Ld");
        int Ls_loc = glGetUniformLocation(terrain_shader, "Ls");
        int shininess_loc = glGetUniformLocation(terrain_shader, "shininess");
        glUniformMatrix4fv(t_projection_loc, 1, false, glm::value_ptr(projection));
        glUniformMatrix4fv(t_view_loc, 1, false, glm::value_ptr(view));
        glUniformMatrix4fv(terrainStart_loc, 1, false, glm::value_ptr(terrainStart_matrix));
        glUniform3fv(lightDirection_loc, 1, glm::value_ptr(light_direction_1));
        glUniform3fv(viewPosition_loc, 1, glm::value_ptr(camera_pos));
        glUniform3fv(Ka_loc, 1, glm::value_ptr(terrain_color.Ka));
        glUniform3fv(Kd_loc, 1, glm::value_ptr(terrain_color.Kd));
        glUniform3fv(Ks_loc, 1, glm::value_ptr(terrain_color.Ks));
        glUniform3fv(La_loc, 1, glm::value_ptr(terrain_color.La));
        glUniform3fv(Ld_loc, 1, glm::value_ptr(terrain_color.Ld));
        glUniform3fv(Ls_loc, 1, glm::value_ptr(terrain_color.Ls));
        glUniform1f(shininess_loc, terrain_color.shininess);
        glBindVertexArray(terrainStart_mesh.mVao);
        glDrawElements(GL_TRIANGLES, terrainStart_mesh.mSubmesh[0].mNumIndices, GL_UNSIGNED_INT, 0);
        */
    }

    {
        glUseProgram(instanced_model_shader);
        int projection_loc = glGetUniformLocation(instanced_model_shader, "projection");
        int view_loc = glGetUniformLocation(instanced_model_shader, "view");
        int lightDirection_loc = glGetUniformLocation(instanced_model_shader, "light_direction");
        int viewPosition_loc = glGetUniformLocation(instanced_model_shader, "view_position");
        glUniformMatrix4fv(projection_loc, 1, false, glm::value_ptr(projection));
        glUniformMatrix4fv(view_loc, 1, false, glm::value_ptr(view));
        glUniform3fv(lightDirection_loc, 1, glm::value_ptr(light_direction_1));
        glUniform3fv(viewPosition_loc, 1, glm::value_ptr(camera_pos));
        im_1.draw();
        im_2.draw();
    }

    draw_gui(window);
}

void idle()
{
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    window_dims = glm::vec2(width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 1.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos += cameraSpeed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos -= cameraSpeed * camera_front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * cameraSpeed;
}

/*
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    //std::cout << "key : " << key << ", " << char(key) << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << std::endl;

    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        // Move terrain up
        case 'w':
        case 'W':
            //spaceship_pos.z += (moveFactor * deltaTime);
            ///terrainStart_pos.y += (moveFactor * deltaTime);
            break;

        // Move terrain back
        case 's':
        case 'S':
            //spaceship_pos.z -= (moveFactor * deltaTime);
            //terrainStart_pos.y -= (moveFactor * deltaTime);
            break;

        // Move left
        case 'a':
        case 'A':
            //spaceship_pos.x += (moveFactor * deltaTime);
            break;

        // Move right
        case 'd':
        case 'D':
            //spaceship_pos.x -= (moveFactor * deltaTime);
            break;

        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }
    }
}
*/

void mouse_cursor(GLFWwindow *window, double xpos, double ypos)
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
    camera_front = glm::normalize(direction);
}

void mouse_button(GLFWwindow *window, int button, int action, int mods)
{
}

/*
void mouse_scroll(GLFWwindow *window, double xoffset, double yoffset)
{
    switch ((int)yoffset)
    {
    case 1:                  // Scroll Up
        scaleFactor += 0.5f; // Increase scale
        break;

    case -1:                 // Scroll Down
        scaleFactor *= 0.5f; // Decrease scale
        break;
    }

    // Update scales
    //spaceship_scale *= scaleFactor;
    terrainStart_scale *= scaleFactor;

    // Print info
    std::printf("New scale factor: %0.3f\n", scaleFactor);
    //std::printf("New Spaceship Scale: (%.9f, %.9f, %.9f)\n", spaceship_scale.x, spaceship_scale.y, spaceship_scale.z);
    std::printf("New Terrain Scale: (%.2f, %.2f, %.2f)\n", terrainStart_scale.x, terrainStart_scale.y, terrainStart_scale.z);
}
*/

void resize(GLFWwindow *window, int width, int height)
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

    const char *window_name = "Just Another Space Shooter";
    // GLFWwindow *window = glfwCreateWindow(window_dims.x, window_dims.y, window_name, NULL, NULL);
    GLFWmonitor *primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primary_monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    GLFWwindow *window = glfwCreateWindow(mode->width, mode->height, "My Title", primary_monitor, NULL);
    window_dims.x = mode->width;
    window_dims.y = mode->height;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    //glfwSetKeyCallback(window, keyboard);
    glfwSetCursorPosCallback(window, mouse_cursor);
    glfwSetMouseButtonCallback(window, mouse_button);
    //glfwSetScrollCallback(window, mouse_scroll);
    glfwSetWindowSizeCallback(window, resize);
    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMaximizeWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    init_game();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0, 0, 0, 1.0f);

        processInput(window);

        display(window);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
