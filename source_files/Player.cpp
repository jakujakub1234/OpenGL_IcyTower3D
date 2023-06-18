#include "../header_files/Player.h"
#include "../header_files/CollisionManager.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>
#include <iostream>

Player::Player()
{
    UpdateModelVectors();
}

glm::mat4 Player::GetModelViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Player::ProcessKeyboard(Player_Movement direction, float deltaTime)
{
    if (direction == JUMP) {
        ProcessJump(deltaTime);
        return;
    }

    float pos_y_save = position.y;
    float velocity = movement_speed * deltaTime;

    glm::vec3 next_position = position;

    switch (direction) {
        case FORWARD:
            next_position += front * velocity;
            break;

        case BACKWARD:
            next_position -= front * velocity;
            break;

        case LEFT:
            next_position -= right * velocity;
            break;

        case RIGHT:
            next_position += right * velocity;
            break;
    }

    next_position.y = pos_y_save;

    if (!CollisionManager::CheckWallsCollision(next_position)) {
        position = next_position;
    }
}

void Player::ProcessJump(float delta_time)
{
    if (in_air || falling_down) {
        return;
    }

    
    jump_limit = position.y + 1.5f;
    in_air = true;
}

void Player::ProcessGravity(float delta_time) // TODO more natural speed
{
    if (in_air) {
        position.y += jump_speed * delta_time;
        if (position.y > jump_limit) {
            falling_down = true;
            in_air = false;
        }
    }
    else {
        falling_down = !CollisionManager::CheckPlatformsCollision(this->position);
    }

    if (falling_down) {
        position.y -= jump_speed * delta_time;
    }
}

void Player::ProcessMouseMovement(float xoffset)
{
    xoffset *= mouse_sensitivity;
    yaw += xoffset;

    UpdateModelVectors();
}

void Player::UpdateModelVectors()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}