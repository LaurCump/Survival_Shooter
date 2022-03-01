#pragma once

#include "components/simple_scene.h"
#include "enemy.h"
#include "wall.h"

namespace m1
{
    class Level
    {
    public:
        Level();
        ~Level();
        void CreateMap();
        void CreateEnemies();  // harta trebuie sa existe pentru ca aceasta sa ruleze corect

        // functiile de mai jos sunt folosite pentru miscarea obiectelor, a inamiciilor, deoarece cand jucatorul "se misca", in realitate nu el se misca, ci totul din jurul lui
        void MoveObjects(glm::vec2 movement);
        void MoveEnemies(float deltaTimeSeconds, glm::vec2 target);
        void RotateEnemiesTowards(glm::vec2 target);
        void Restart();   

    public:
        int mapSize;
        glm::vec2 mapBottomLeftPixel;  // valoarea este in interiorul granitelor hartii
        glm::vec2 mapTopRightPixel;    // valoarea este in interiorul granitelor hartii
        std::vector<Wall*> walls;

        int enemyCount;
        std::vector<Enemy*> enemies;
    };
}
