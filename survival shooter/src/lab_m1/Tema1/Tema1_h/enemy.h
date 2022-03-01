#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Enemy
    {
    public:
        Enemy(glm::vec2 mapBottomLeftPixel,
            glm::vec2 mapTopRightPixel,
            glm::vec2 size = glm::vec2(60.0f, 60.0f),
            glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f),
            float moveSpeed = 200,
            int points = 12);
        ~Enemy();
        void Spawn();

    public:
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec3 color;
        glm::vec2 initialLookAt;  // Inamicul e desenat sa priveasca la initialLookAt
        glm::vec2 lookAt;
        float lookAngle;
        float moveSpeed;
        bool isAlive;
        int points;

        glm::vec2 mapBottomLeftPixel;  // valoarea este in interiorul granitelor hartii si reprezinta coltul din stanga jos a hartii
        glm::vec2 mapTopRightPixel;    // valoarea este in interiorul granitelor hartii si reprezinta coltul din dreapta sus a hartii
    };
}
