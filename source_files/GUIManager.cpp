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

float GUIManager::score_board[16] = {
    -0.9f, 0.9f, 0.0f, 1.0f,
    -0.4f, 0.9f, 1.0f, 1.0f,
    -0.9f, 0.5f, 0.0f, 0.0f,
    -0.4f, 0.5f, 1.0f, 0.0f
};

ShaderManager GUIManager::guiShaderManager = ShaderManager();
TextureManager GUIManager::textureManager = TextureManager();

unsigned int GUIManager::HUD_text_VAO = 0;
unsigned int GUIManager::HUD_text_VBO = 0;

unsigned int GUIManager::plate_texture = 0;
unsigned int GUIManager::numbers_texture = 0;
unsigned int GUIManager::game_over_texture = 0;

unsigned int GUIManager::score = 0;
bool GUIManager::game_over = false;

void GUIManager::Init()
{
    GUIManager::guiShaderManager.BuildShaders((char*)"shaders/GUIVertexShader.vs", (char*)"shaders/GUIFragmentShader.fs");

    GUIManager::BindObject();

    GUIManager::plate_texture = GUIManager::textureManager.LoadTexture((char*)"textures/score_plate.png", true);
    GUIManager::numbers_texture = GUIManager::textureManager.LoadTexture((char*)"textures/numbers.png", true);
    GUIManager::game_over_texture = GUIManager::textureManager.LoadTexture((char*)"textures/game_over.png", true);

    GUIManager::guiShaderManager.Use();
    GUIManager::guiShaderManager.SetInt((char*)"texture1", 0);

    GUIManager::score = 0;
    GUIManager::game_over = false;
}

void GUIManager::BindObject()
{
    glGenVertexArrays(1, &GUIManager::HUD_text_VAO);
    glGenBuffers(1, &GUIManager::HUD_text_VBO);
    glBindVertexArray(GUIManager::HUD_text_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, GUIManager::HUD_text_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GUIManager::score_board), GUIManager::score_board, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GUIManager::DrawText()
{
    int score = GUIManager::score;

    glm::mat4 model = glm::mat4(1.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GUIManager::numbers_texture);

    GUIManager::guiShaderManager.Use();
    glBindVertexArray(GUIManager::HUD_text_VAO);

    int offset = score > 0 ? (int) log10 ((double) score) + 1 : 1;
    offset--;

    do {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.54 + 0.04 * offset, 0.5275, 0.0));
        model = glm::scale(model, glm::vec3(0.125f, 0.25f, 0.25f));
        GUIManager::guiShaderManager.SetFloat((char*)"score_digit_by_10", (score % 10) / 10.0);
        GUIManager::guiShaderManager.SetMat4((char*)"model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        score /= 10;
        offset--;
    } while (score > 0);

    glBindTexture(GL_TEXTURE_2D, GUIManager::plate_texture);

    model = glm::mat4(1.0f);
    GUIManager::guiShaderManager.SetFloat((char*)"score_digit_by_10", -1.0);
    GUIManager::guiShaderManager.SetMat4((char*)"model", model);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


    if (GUIManager::game_over) {
        glBindTexture(GL_TEXTURE_2D, GUIManager::game_over_texture);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.3, -1.3, 0.0));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 0.25f));
        GUIManager::guiShaderManager.SetFloat((char*)"score_digit_by_10", -1.0);
        GUIManager::guiShaderManager.SetMat4((char*)"model", model);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    glBindVertexArray(0);
}