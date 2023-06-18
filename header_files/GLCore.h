/*
The code template was taken from https://learnopengl.com/
*/

#pragma once

#include "ShaderManager.h"
#include "Player.h"
#include "CameraManager.h"
#include "TextureManager.h"
#include "Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

class GLCore
{
public:
	static CameraManager cameraManager;
    static Player player;

	static float lastX;
	static float lastY;
	static bool first_mouse;

	static const unsigned int SCR_WIDTH = 1400;
	static const unsigned int SCR_HEIGHT = 900;

	GLCore();
	void RenderLoop();

private:
    
    // TODO too many class members, maybe keep shaders as ints and change ShadeerManager to static class

	ShaderManager platformShaderManager, skyboxShaderManager, modelShaderManager, lavaShaderManager;
    TextureManager textureManager;
    
    Model player_model;
    Model wall_model;
	
	float delta_time = 0.0f;
	float last_frame = 0.0f;
    float wall_scale = 1.0f; //TODO

    unsigned int platform_texture, lava_texture, skybox_texture;

    bool game_over;

    float platform_vertices[5*6*6] = {
        -0.5f, -0.0f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.0f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.0f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.0f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.0f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.0f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.0f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.0f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.0f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.0f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.0f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.0f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.0f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.0f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.0f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.0f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.0f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.0f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    float skybox_vertices[5*6*6] = { // TODO cannot attach to EBO because cubes dont like UV
        -100.0f,  100.0f, -100.0f,
        -100.0f, -100.0f, -100.0f,
        100.0f, -100.0f, -100.0f,
        100.0f, -100.0f, -100.0f,
        100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f, -100.0f,

        -100.0f, -100.0f,  100.0f,
        -100.0f, -100.0f, -100.0f,
        -100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f, -100.0f,
        -100.0f,  100.0f,  100.0f,
        -100.0f, -100.0f,  100.0f,

        100.0f, -100.0f, -100.0f,
        100.0f, -100.0f,  100.0f,
        100.0f,  100.0f,  100.0f,
        100.0f,  100.0f,  100.0f,
        100.0f,  100.0f, -100.0f,
        100.0f, -100.0f, -100.0f,

        -100.0f, -100.0f,  100.0f,
        -100.0f,  100.0f,  100.0f,
        100.0f,  100.0f,  100.0f,
        100.0f,  100.0f,  100.0f,
        100.0f, -100.0f,  100.0f,
        -100.0f, -100.0f,  100.0f,

        -100.0f,  100.0f, -100.0f,
        100.0f,  100.0f, -100.0f,
        100.0f,  100.0f,  100.0f,
        100.0f,  100.0f,  100.0f,
        -100.0f,  100.0f,  100.0f,
        -100.0f,  100.0f, -100.0f,

        -100.0f, -100.0f, -100.0f,
        -100.0f, -100.0f,  100.0f,
        100.0f, -100.0f, -100.0f,
        100.0f, -100.0f, -100.0f,
        -100.0f, -100.0f,  100.0f,
        100.0f, -100.0f,  100.0f
    };

    float lava_vertices[5*6] = {
        -15.0f, -0.5f, 15.0f, 0.0f, 10.0f,
        15.0f, -0.5f, 15.0f, 10.0f, 10.0f,
        -15.0f, -0.5f, -15.0f, 0.0f, 0.0f,

        15.0f, -0.5f, 15.0f, 10.0f, 10.0f,
        15.0f, -0.5f, -15.0f, 10.0f, 0.0f,
        -15.0f, -0.5f, -15.0f, 0.0f, 0.0f
    };
   
	
	GLFWwindow* window;
	unsigned int platform_VAO, platform_VBO, lava_VAO, lava_VBO, skybox_VAO, skybox_VBO;

	void ProcessInput();

	void BindObjects();

	void static FramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void static MouseCallback(GLFWwindow* window, double xposIn, double yposIn);
    void static ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    void DrawPlayer();
    void DrawWalls();
    void DrawPlatforms();
    void DrawLava();
    void DrawSkybox();
};

