#pragma once

#include "components/simple_scene.h"
#include "player.h"
#include "level.h"


namespace m1
{
    class Game : public gfxc::SimpleScene
    {
     public:
        Game();
        ~Game();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        bool CollisionAABBAABB(glm::vec2 pos1, glm::vec2 size1, glm::vec2 pos2, glm::vec2 size2);
        bool CollisionAABBCircle(glm::vec2 pos1, glm::vec2 size1, glm::vec2 pos2, float radius2);
        void DetectTriggerCollisions();
        void RestartGame();

        void RenderPlayer();
        void RenderCrosshair();
        void RenderMap();
        void RenderEnemies();
        void RenderBullets();
        void RenderLifes();

     protected:
        glm::mat3 modelMatrix;
        Player *player;

        const glm::vec3 backgroundColor{ glm::vec3(0.5f, 0.5f, 0.5f) };

        const float mapSize{ 2600 };  // pixeli

        Level *level;
        int* score;

        bool gameStarted;
    };
}
