#include "../header_files/CollisionManager.h"
#include "../header_files/GUIManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

// TODO ???
float CollisionManager::collision_radius = 0.5f;
float CollisionManager::global_floor = -0.2f;
float CollisionManager::lava_level = -0.5f;
float CollisionManager::platforms_speed = 0.0f;
float CollisionManager::walls_radius = 7.8f;

glm::vec2 CollisionManager::walls_center = glm::vec2(1.0f, 1.0f);

float CollisionManager::platform_hitbox[6] = {
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f, 0.5f
};

std::vector<glm::vec3> CollisionManager::platforms_trans = {
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 2.0f, 0.0f),
    glm::vec3(2.0f, 3.0f, 0.0f),
    glm::vec3(3.0f, 4.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, 0.0f),
    glm::vec3(1.0f, 6.0f, 0.0f),
    glm::vec3(3.0f, 7.0f, 0.0f),
    glm::vec3(3.0f, 8.0f, 2.0f),
    glm::vec3(2.0f, 9.0f, 1.0f),
    glm::vec3(0.0f, 10.0f, 2.0f),
    glm::vec3(1.0f, 11.0f, 3.0f),
    glm::vec3(2.0f, 12.0f, 2.0f),
};

void CollisionManager::MovePlatforms(float delta_time)
{
    float velocity = platforms_speed * delta_time;

    for (std::size_t i = 0; i < platforms_trans.size(); i++) {
        platforms_trans[i].y -= velocity;

        if (platforms_trans[i].y < lava_level - 1.0f) {
            SpawnPlatform(i);
        }
    }
}

void CollisionManager::SpawnPlatform(int index)
{
    platforms_trans[index].y = (float)platforms_trans.size() - 1.5f - ((lava_level - 1.0f) - platforms_trans[index].y); // TODO ugly hack

    platforms_trans[index].x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/ 5.0f));
    platforms_trans[index].z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/ 5.0f));

    GUIManager::score++;
}

bool CollisionManager::CheckPlatformsCollision(glm::vec3 position)
{
    float delta_y = 0.72f;
    float delta_x = -0.5f;
    float delta_z = -0.5f;

    if (position.y < global_floor) {
        return true;
    }

    for (std::size_t i = 0; i < platforms_trans.size(); i++) {
        if (
            position.x < delta_x + platforms_trans[i].x + 1.0f &&
            position.x > delta_x + platforms_trans[i].x &&
            position.y < delta_y + platforms_trans[i].y &&
            position.y > delta_y + platforms_trans[i].y - 0.8f &&
            position.z < delta_z + platforms_trans[i].z + 1.0f &&
            position.z > delta_z + platforms_trans[i].z
            )
        {
            return true;
        }
    }

    return false;
}

bool CollisionManager::CheckWallsCollision(glm::vec3 position)
{
    return glm::distance(walls_center, glm::vec2(position.x, position.z)) >= walls_radius;
}