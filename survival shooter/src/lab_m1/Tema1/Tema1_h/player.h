#pragma once

#include "components/simple_scene.h"
#include "bullet.h"

namespace m1
{
    class Player
    {
    public:
        Player(glm::vec2 pos,
            glm::vec2 size = glm::vec2(30.0f, 30.0f),
            glm::vec3 color = glm::vec3(1, 0.85f, 0.35f),
            float moveSpeed = 190.0f,
            int ammo = 10,
            int lifes = 3);
        ~Player();
        void MoveBullets(glm::vec2 movement);  // cand "se misca" jucatorul
        void UpdateBullets(float deltaTimeSeconds);

    public:
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec3 color;
        glm::vec2 initialLookAt;  // jucatorul e desenat sa priveasca la initialLookAt
        glm::vec2 lookAt;
        float lookAngle;
        float moveSpeed;

        int ammo;
        const float cooldown = 0.5f;  // firing rate in secunde
        float cooldownTimer;
        std::vector<Bullet*> bullets;

        int lifes;
    };
}
