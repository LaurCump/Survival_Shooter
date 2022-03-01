#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Bullet
    {
    public:
        Bullet(glm::vec2 pos = glm::vec2(0, 0),
            glm::vec2 direction = glm::vec2(0, 0),
            float angle = 0.0f,
            glm::vec2 size = glm::vec2(10, 10),
            glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f),
            float speed = 550.0f);
        ~Bullet();
        void Reset();

    public:
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec3 color;
        glm::vec2 direction;
        float angle;
        float speed;
        const float maxRange = 600;  // distanta maxima pe care o poate parcurge proiectilul
        float mileage;  // distanta curenta parcursa
        bool available;
    };
}
