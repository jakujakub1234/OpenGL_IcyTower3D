#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class CollisionManager
{
public:
    static float platform_hitbox[6];
    static float global_floor;
    static float lava_level;
    static float platforms_speed;
    
    static std::vector<glm::vec3> platforms_trans;

    static void MovePlatforms(float delta_time);
    static void SpawnPlatform(int index);

    static bool CheckPlatformsCollision(glm::vec3 position);
    static bool CheckWallsCollision(glm::vec3 position);

private:
    static float collision_radius;
    static float walls_radius;

    static glm::vec2 walls_center;
};

