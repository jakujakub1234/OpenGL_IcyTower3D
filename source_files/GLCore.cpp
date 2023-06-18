#include "../header_files/GLCore.h" // TODO better includes from another directories
#include "../header_files/Model.h"
#include "../header_files/ShaderManager.h"
#include "../header_files/CollisionManager.h"
#include "../header_files/GUIManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "../header_files/stb_image.h"

Player GLCore::player = Player();
CameraManager GLCore::cameraManager = CameraManager(GLCore::player, glm::vec3(0.0f, 2.0f, 5.0f));

float GLCore::lastX = GLCore::SCR_WIDTH / 2.0;
float GLCore::lastY = GLCore::SCR_HEIGHT / 2.0;
bool GLCore::first_mouse = true;

GLCore::GLCore()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    stbi_set_flip_vertically_on_load(true);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    this->window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Icy Tower 3D", NULL, NULL);
    if (this->window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return;
    }
    glfwMakeContextCurrent(this->window);
    glfwSetFramebufferSizeCallback(this->window, this->FramebufferSizeCallback);
    glfwSetCursorPosCallback(this->window, this->MouseCallback);
    glfwSetScrollCallback(this->window, this->ScrollCallback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->platformShaderManager = ShaderManager();
    this->platformShaderManager.BuildShaders((char*)"shaders/PlatformVertexShader.vs", (char*)"shaders/PlatformFragmentShader.fs");

    this->skyboxShaderManager = ShaderManager();
    this->skyboxShaderManager.BuildShaders((char*)"shaders/SkyboxVertexShader.vs", (char*)"shaders/SkyboxFragmentShader.fs");

    this->modelShaderManager = ShaderManager();
    this->modelShaderManager.BuildShaders((char*)"shaders/ModelVertexShader.vs", (char*)"shaders/ModelFragmentShader.fs");

    this->lavaShaderManager = ShaderManager();
    this->lavaShaderManager.BuildShaders((char*)"shaders/PlatformVertexShader.vs", (char*)"shaders/LavaFragmentShader.fs");

    this->BindObjects();
    this->player_model = Model((char*)"models/character/character.obj");
    this->wall_model = Model((char*)"models/wall/wall.obj");
    
    this->textureManager = TextureManager();

    this->platform_texture = this->textureManager.LoadTexture((char*)"textures/platform.jpg", false);
    this->lava_texture = this->textureManager.LoadTexture((char*)"textures/lava.jpg", false);

    std::vector<char*> faces
    {
        (char*)"textures/right.jpg",
        (char*)"textures/left.jpg",
        (char*)"textures/top.jpg",
        (char*)"textures/bottom.jpg",
        (char*)"textures/front.jpg",
        (char*)"textures/back.jpg"
    };

    this->skybox_texture = this->textureManager.LoadCubemap(faces);

    this->platformShaderManager.Use();
    this->platformShaderManager.SetInt((char*)"texture1", 0);

    this->lavaShaderManager.Use();
    this->lavaShaderManager.SetInt((char*)"texture1", 0);

    GUIManager::Init();
}

void GLCore::BindObjects()
{
    glGenVertexArrays(1, &this->platform_VAO);
    glGenBuffers(1, &this->platform_VBO);
    glBindVertexArray(this->platform_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->platform_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->platform_vertices), this->platform_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &this->lava_VAO);
    glGenBuffers(1, &this->lava_VBO);
    glBindVertexArray(this->lava_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->lava_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->lava_vertices), this->lava_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &this->skybox_VAO);
    glGenBuffers(1, &this->skybox_VBO);
    glBindVertexArray(this->skybox_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->skybox_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->skybox_vertices), &this->skybox_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void GLCore::DrawPlayer()
{
    glm::mat4 projection = glm::perspective(glm::radians(this->cameraManager.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = this->cameraManager.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    this->modelShaderManager.Use();
    this->modelShaderManager.SetMat4((char*)"projection", projection);
    this->modelShaderManager.SetMat4((char*)"view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, this->player.position);
    model = glm::rotate(model, glm::radians(-this->player.yaw+90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // TODO wtf is this yaw hack
    //model = glm::rotate(model, glm::radians(this->cameraManager.yaw), glm::vec3(0.0f, 1.0f, 0.0f)); // TODO wtf is this yaw hack
    model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
    this->modelShaderManager.SetMat4((char*)"model", model);
    this->modelShaderManager.SetFloat((char*)"jump", 0.0);
    this->player_model.Draw(this->modelShaderManager);
}

void GLCore::DrawWalls()
{
    glm::mat4 projection = glm::perspective(glm::radians(this->cameraManager.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = this->cameraManager.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    this->modelShaderManager.Use();
    this->modelShaderManager.SetMat4((char*)"projection", projection);
    this->modelShaderManager.SetMat4((char*)"view", view);
    
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(1.0f, 8.0f * j, 1.0f));
            model = glm::rotate(model, glm::radians(90.0f * i), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
            this->modelShaderManager.SetMat4((char*)"model", model);
            this->modelShaderManager.SetFloat((char*)"jump", CollisionManager::platforms_speed * static_cast<float>(glfwGetTime()));
            this->wall_model.Draw(this->modelShaderManager);
        }
    }        
}

void GLCore::DrawPlatforms()
{
    glm::mat4 projection = glm::perspective(glm::radians(this->cameraManager.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = this->cameraManager.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    std::map<float, glm::vec3> sorted;
    for (std::size_t i = 0; i < CollisionManager::platforms_trans.size(); i++)
    {
        float distance = glm::length(cameraManager.position - CollisionManager::platforms_trans[i]);

        sorted[distance] = CollisionManager::platforms_trans[i];
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->platform_texture);

    this->platformShaderManager.Use();
    this->platformShaderManager.SetMat4((char*)"projection", projection);
    this->platformShaderManager.SetMat4((char*)"view", view);

    glBindVertexArray(platform_VAO);

    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        platformShaderManager.SetMat4((char*)"model", model);

        if (cameraManager.position.y < 0.5f + it->second[1])
            this->platformShaderManager.SetFloat((char*)"transparent", it->first / 10.0f);
        else
            this->platformShaderManager.SetFloat((char*)"transparent", 1.0f);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
}

void GLCore::DrawLava()
{
    glm::mat4 projection = glm::perspective(glm::radians(this->cameraManager.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = this->cameraManager.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->lava_texture);

    this->lavaShaderManager.Use();
    this->lavaShaderManager.SetMat4((char*)"projection", projection);
    this->lavaShaderManager.SetMat4((char*)"view", view);
    this->lavaShaderManager.SetFloat((char*)"time", static_cast<float>(glfwGetTime()));

    glBindVertexArray(lava_VAO);

    model = glm::mat4(1.0f);
    platformShaderManager.SetMat4((char*)"model", model);

    if (cameraManager.position.y < CollisionManager::lava_level)
        this->lavaShaderManager.SetFloat((char*)"transparent", 0.3f);
    else
        this->lavaShaderManager.SetFloat((char*)"transparent", 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
}

void GLCore::DrawSkybox()
{
    glm::mat4 projection = glm::perspective(glm::radians(this->cameraManager.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = this->cameraManager.GetViewMatrix();

    glDepthFunc(GL_LEQUAL);
    this->skyboxShaderManager.Use();
    view = glm::mat4(glm::mat3(this->cameraManager.GetViewMatrix()));
    this->skyboxShaderManager.SetMat4((char*)"view", view);
    this->skyboxShaderManager.SetMat4((char*)"projection", projection);
    glBindVertexArray(skybox_VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skybox_texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void GLCore::RenderLoop()
{
    int prev_sec = 0;
    int counter = 0;

    game_over = false;

    while (!glfwWindowShouldClose(this->window)) {
        float current_frame = static_cast<float>(glfwGetTime());
        this->delta_time = current_frame - this->last_frame;
        this->last_frame = current_frame;

        if (current_frame > prev_sec) {
            prev_sec += 1;
            //std::cout << "FPS: " << counter << std::endl;
            counter = 0;
        }
        counter++;

        this->ProcessInput();

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->DrawPlayer();
        this->DrawWalls();
              
        this->player.ProcessGravity(delta_time);
        CollisionManager::MovePlatforms(delta_time);

        if (this->player.position.y <= CollisionManager::global_floor) {
            game_over = true;

            GUIManager::game_over = true;
            CollisionManager::platforms_speed = 0.0f;
        }

        this->DrawPlatforms();
        this->DrawLava();

        GUIManager::DrawText();

        this->DrawSkybox();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &this->platform_VAO);
    glDeleteBuffers(1, &this->platform_VBO);
    glDeleteVertexArrays(1, &this->lava_VAO);
    glDeleteBuffers(1, &this->lava_VBO);
    glDeleteVertexArrays(1, &this->skybox_VAO);
    glDeleteBuffers(1, &this->skybox_VBO);

    this->modelShaderManager.DeleteShader();
    this->platformShaderManager.DeleteShader();
    this->skyboxShaderManager.DeleteShader();

    glfwTerminate();
}

void GLCore::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (game_over) {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        this->player.ProcessKeyboard(FORWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        this->player.ProcessKeyboard(BACKWARD, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        this->player.ProcessKeyboard(LEFT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        this->player.ProcessKeyboard(RIGHT, delta_time);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        this->player.ProcessKeyboard(JUMP, delta_time);

        CollisionManager::platforms_speed = 0.75f;
    }

    this->cameraManager.UpdateCameraVectors();
}

void GLCore::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLCore::MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (GLCore::first_mouse)
    {
        GLCore::lastX = xpos;
        GLCore::lastY = ypos;
        GLCore::first_mouse = false;
    }

    float xoffset = xpos - GLCore::lastX;
    float yoffset = GLCore::lastY - ypos;

    GLCore::lastX = xpos;
    GLCore::lastY = ypos;

    player.ProcessMouseMovement(xoffset);
    cameraManager.ProcessMouseMovement(yoffset);
}

void GLCore::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    cameraManager.ProcessMouseScroll(static_cast<float>(yoffset));
}