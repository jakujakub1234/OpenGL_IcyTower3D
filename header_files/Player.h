#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

enum Player_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP
};

class Player
{
public:
    float yaw = 0.0f;

    glm::vec3 position = glm::vec3(0.0f, 1.7f, 0.0f);

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    float movement_speed = 2.5f;
    float mouse_sensitivity = 0.1f;;

    Player();

    glm::mat4 GetModelViewMatrix();

    void ProcessKeyboard(Player_Movement direction, float deltaTime);
    void ProcessGravity(float deltaTime);
    void ProcessMouseMovement(float xoffset);

private:
    bool falling_down = false;
    bool in_air = false;

    float jump_limit = 1.0f;
    float jump_speed = 2.5f;
    const float pitch = -20.0f;

    void ProcessJump(float deltaTime);
    void UpdateModelVectors();
};