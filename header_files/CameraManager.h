/*
The code template was taken from https://learnopengl.com/
*/

#pragma once

#include "Player.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

const float YAW = 0.0f;
const float PITCH = -20.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum Camera_Movement {
    CAM_FORWARD,
    CAM_BACKWARD,
    CAM_LEFT,
    CAM_RIGHT,
    CAM_UP,
    CAM_DOWN
};

class CameraManager
{
public:
    glm::vec3 position;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    glm::vec3 world_up;

    float yaw;
    float pitch;

    float zoom;

    float movement_speed;
    float mouse_sensitivity;
    float distance_from_player;

    CameraManager(
        Player& player,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH
    )
    {
        distance_from_player = 5.0f;
        zoom = ZOOM;
        mouse_sensitivity = SENSITIVITY;
        movement_speed = SPEED;
        front = glm::vec3(0.0f, 0.0f, -1.0f);

        this->position = position;
        world_up = up;
        this->yaw = yaw;
        this->pitch = pitch;

        this->player = &player;

        UpdateCameraVectors();
    }

    CameraManager(
        Player& player,
        float posX,
        float posY,
        float posZ,
        float upX,
        float upY,
        float upZ,
        float yaw,
        float pitch
    )
    {
        distance_from_player = 5.0f;
        zoom = ZOOM;
        mouse_sensitivity = SENSITIVITY;
        movement_speed = SPEED;
        front = glm::vec3(0.0f, 0.0f, -1.0f);

        position = glm::vec3(posX, posY, posZ);
        world_up = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;

        this->player = &player;

        UpdateCameraVectors();
    }

    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void ProcessMouseScroll(float yoffset)
    {
        distance_from_player -= (float)yoffset / 10.0f;
        if (distance_from_player < 1.0f)
            distance_from_player = 1.0f;
        if (distance_from_player > 5.0f)
            distance_from_player = 5.0f;
    }

    void ProcessMouseMovement(float yoffset)
    {
        yoffset *= mouse_sensitivity;
        pitch += yoffset;

        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        UpdateCameraVectors();
    }

    void UpdateCameraVectors()
    {
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(-pitch));
        front.y = sin(glm::radians(-pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(-pitch));
        front = glm::normalize(-front);
        right = glm::normalize(glm::cross(front, world_up));
        up = glm::normalize(glm::cross(right, front));
        
        float horizontal_distance = distance_from_player * cos(glm::radians(pitch));
        float vertical_distance = distance_from_player * sin(glm::radians(pitch));

        position = player->position;

        float offset_x = horizontal_distance * sin(glm::radians(-(player->yaw - 90.0f)));
        float offset_z = horizontal_distance * cos(glm::radians(-(player->yaw - 90.0f)));

        position.x = player->position.x - offset_x;
        position.y = player->position.y - vertical_distance;
        position.z = player->position.z - offset_z;

        this->yaw = - (180.0f - player->yaw);
    }

private:
    Player* player;

    float angle_around_player = 0.0f;
};

