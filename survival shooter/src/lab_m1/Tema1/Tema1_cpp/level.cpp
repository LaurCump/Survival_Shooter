#include "lab_m1/Tema1/Tema1_h/level.h"
#include <iostream>
#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

Level::Level()
{
    mapSize = 2600;
    enemyCount = 3;
    CreateMap();
    CreateEnemies();  // harta trebuie sa existe ca aceasta functie sa mearga corect
}

Level::~Level()
{
    for each (Wall *w in walls)
    {
        delete w;
    }
    walls.clear();

    for each (Enemy *e in enemies)
    {
        delete e;
    }
    enemies.clear();
}

void Level::CreateMap()
{
    glm::vec3 color = glm::vec3(0, 1, 0);

    /* pot fi modificate valorile de mai jos pentru a schimba pozitia jucatorului pe harta la inceputul jocului
     * Nota: Urmatoarea afirmatie trebuie sa fie adevarata  
     * mapTopRightPixel - mapBottomLeftPixel = glm::vec2(mapSize, mapSize)
     */
    mapBottomLeftPixel = glm::vec2(0, 0);
    mapTopRightPixel = glm::vec2(mapSize, mapSize);

    // crearea granitelor hartii
    {
        float wallThickness = 30.0f; //grosimea hartii
        glm::vec2 tmp_pos;
        glm::vec2 tmp_size;

        // marginea din stanga
        tmp_pos = glm::vec2(mapBottomLeftPixel.x - wallThickness, mapBottomLeftPixel.y - wallThickness);
        tmp_size = glm::vec2(wallThickness, mapSize + 2 * wallThickness);
        walls.push_back(new Wall(tmp_pos, tmp_size, color));

        // marginea din dreapta
        tmp_pos = glm::vec2(mapTopRightPixel.x, mapBottomLeftPixel.y - wallThickness);
        tmp_size = glm::vec2(wallThickness, mapSize + 2 * wallThickness);
        walls.push_back(new Wall(tmp_pos, tmp_size, color));

        // marginea de sus
        tmp_pos = glm::vec2(mapBottomLeftPixel.x, mapTopRightPixel.y);
        tmp_size = glm::vec2(mapSize, wallThickness);
        walls.push_back(new Wall(tmp_pos, tmp_size, color));

        // marginea de jos
        tmp_pos = glm::vec2(mapBottomLeftPixel.x, mapBottomLeftPixel.y - wallThickness);
        tmp_size = glm::vec2(mapSize, wallThickness);
        walls.push_back(new Wall(tmp_pos, tmp_size, color));
    }

    // crearea altor pereti de harta
    {
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x +  470, mapBottomLeftPixel.y + 2350), glm::vec2( 70,  70), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x +  470, mapBottomLeftPixel.y + 2300), glm::vec2(400,  50), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x +  700, mapBottomLeftPixel.y + 1500), glm::vec2( 70, 600), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x +  800, mapBottomLeftPixel.y +  800), glm::vec2(400,  70), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x +  870, mapBottomLeftPixel.y +  230), glm::vec2( 70,  70), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x +  870, mapBottomLeftPixel.y +  300), glm::vec2(400,  50), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 1400, mapBottomLeftPixel.y +  800), glm::vec2( 40, 600), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 1650, mapBottomLeftPixel.y + 2300), glm::vec2(800,  40), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 1900, mapBottomLeftPixel.y +  200), glm::vec2(400,  50), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 1800, mapBottomLeftPixel.y + 1300), glm::vec2(800,  40), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 1900, mapBottomLeftPixel.y +  800), glm::vec2(300,  50), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 2050, mapBottomLeftPixel.y + 1500), glm::vec2( 70, 600), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 2150, mapBottomLeftPixel.y +  730), glm::vec2( 50, 170), color));
        walls.push_back(new Wall(glm::vec2(mapBottomLeftPixel.x + 2450, mapBottomLeftPixel.y +  230), glm::vec2( 30, 700), color));
    }

}

void Level::CreateEnemies() 
{
    for (int i = 0; i < enemyCount; ++i) 
    {
        enemies.push_back(new Enemy(mapBottomLeftPixel, mapTopRightPixel));
    }
}

void Level::MoveObjects(glm::vec2 movement) {
    for each (Wall *w in walls)
    {
        w->pos.x += movement.x;
        w->pos.y += movement.y;
    }

    for each (Enemy *e in enemies)
    {
        if (e->isAlive) 
        {
            e->pos.x += movement.x;
            e->pos.y += movement.y;
        }
    }
}

void Level::MoveEnemies(float deltaTimeSeconds, glm::vec2 target)
{
    for each (Enemy * e in enemies)
    {
        if (e->isAlive)
        {
            glm::vec2 ePos = glm::vec2(e->pos.x + e->size.x / 2, e->pos.y + e->size.y / 2);
            glm::vec2 moveDirection = glm::normalize(target - ePos);

            e->pos.x += moveDirection.x * deltaTimeSeconds * e->moveSpeed;
            e->pos.y += moveDirection.y * deltaTimeSeconds * e->moveSpeed;
        }
    }
}

void Level::RotateEnemiesTowards(glm::vec2 target)
{
    for each (Enemy *e in enemies) 
    {
        if (e->isAlive) 
        {
            glm::vec2 ePos = glm::vec2(e->pos.x + e->size.x / 2, e->pos.y + e->size.y / 2);
            e->lookAt = target;
            e->initialLookAt = ePos + glm::vec2(ePos.x + 1000, 0);

            glm::vec2 adjacent{ glm::vec2(e->initialLookAt.x - ePos.x, e->initialLookAt.y - ePos.y) };
            glm::vec2 hypotenuse{ glm::vec2(e->lookAt.x - ePos.x, e->lookAt.y - ePos.y) };

            e->lookAngle = glm::acos(glm::dot(glm::normalize(adjacent), glm::normalize(hypotenuse)));  // radiani

            if (ePos.y > e->lookAt.y) 
            {
                e->lookAngle *= -1;
            }
        }
    }
}

void Level::Restart() 
{
    //se pot modifica valorile de mai jos pentru a modifica pozitia jucatorului la restartul jocului
    mapBottomLeftPixel = glm::vec2(0, 0);
    mapTopRightPixel = glm::vec2(mapSize, mapSize);

    float wallThickness = 30.0f;
    walls.at(0)->pos = glm::vec2(mapBottomLeftPixel.x - wallThickness, mapBottomLeftPixel.y - wallThickness);
    walls.at(1)->pos = glm::vec2(mapTopRightPixel.x, mapBottomLeftPixel.y - wallThickness);
    walls.at(2)->pos = glm::vec2(mapBottomLeftPixel.x, mapTopRightPixel.y);
    walls.at(3)->pos = glm::vec2(mapBottomLeftPixel.x, mapBottomLeftPixel.y - wallThickness);

    walls.at(4)->pos = glm::vec2(mapBottomLeftPixel.x + 470, mapBottomLeftPixel.y + 2350);
    walls.at(5)->pos = glm::vec2(mapBottomLeftPixel.x + 470, mapBottomLeftPixel.y + 2300);
    walls.at(6)->pos = glm::vec2(mapBottomLeftPixel.x + 700, mapBottomLeftPixel.y + 1500);
    walls.at(7)->pos = glm::vec2(mapBottomLeftPixel.x + 800, mapBottomLeftPixel.y + 800);
    walls.at(8)->pos = glm::vec2(mapBottomLeftPixel.x + 870, mapBottomLeftPixel.y + 230);
    walls.at(9)->pos = glm::vec2(mapBottomLeftPixel.x + 870, mapBottomLeftPixel.y + 300);
    walls.at(10)->pos = glm::vec2(mapBottomLeftPixel.x + 1400, mapBottomLeftPixel.y + 800);
    walls.at(11)->pos = glm::vec2(mapBottomLeftPixel.x + 1650, mapBottomLeftPixel.y + 2300);
    walls.at(12)->pos = glm::vec2(mapBottomLeftPixel.x + 1900, mapBottomLeftPixel.y + 200);
    walls.at(13)->pos = glm::vec2(mapBottomLeftPixel.x + 1800, mapBottomLeftPixel.y + 1300);
    walls.at(14)->pos = glm::vec2(mapBottomLeftPixel.x + 1900, mapBottomLeftPixel.y + 800);
    walls.at(15)->pos = glm::vec2(mapBottomLeftPixel.x + 2050, mapBottomLeftPixel.y + 1500);
    walls.at(16)->pos = glm::vec2(mapBottomLeftPixel.x + 2150, mapBottomLeftPixel.y + 730);
    walls.at(17)->pos = glm::vec2(mapBottomLeftPixel.x + 2450, mapBottomLeftPixel.y + 230);

    for each (Enemy *e in enemies)
    {
        e->Spawn();
    }
}
