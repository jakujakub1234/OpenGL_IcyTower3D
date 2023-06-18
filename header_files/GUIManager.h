#pragma once

#include "TextureManager.h"
#include "ShaderManager.h"

class GUIManager
{
public:
    static void DrawText();    
    static void Init();

    static unsigned int score;
    static bool game_over;

private:
    static ShaderManager guiShaderManager;
    static TextureManager textureManager;
    
    static float score_board[16];
    static unsigned int HUD_text_VAO, HUD_text_VBO, score_int_VAO, score_int_VBO;
    static unsigned int plate_texture, numbers_texture, game_over_texture;

    static void BindObject();
};