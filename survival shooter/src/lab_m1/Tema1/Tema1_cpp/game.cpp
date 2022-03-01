#include "lab_m1/Tema1/Tema1_h/game.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/Tema1_h/transform2D.h"
#include "lab_m1/Tema1/Tema1_h/drawings.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Game::Game()
{
}


Game::~Game()
{
    delete player;
    delete score;
}


void Game::Init()
{
    window->HidePointer();  // ascunde cursorul mouse-ului

    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    player = new Player(glm::vec2(resolution.x / 2, resolution.y / 2));

    Mesh* playerBody = drawings::CreateCircle("playerBody", glm::vec3(0, 0, 0));
    AddMeshToList(playerBody);

    Mesh* enemyBody = drawings::CreateSquare("enemyBody", glm::vec3(0), 1, glm::vec3(0, 0, 0), true);
    AddMeshToList(enemyBody);

    Mesh* square = drawings::CreateSquare("square", glm::vec3(0), 1, glm::vec3(0, 1, 0), true);
    AddMeshToList(square);

    Mesh* squareFrame = drawings::CreateSquare("squareFrame", glm::vec3(0), 1, glm::vec3(0, 0, 0), false);
    AddMeshToList(squareFrame);

    level = new Level();
    score = new int(0);
    gameStarted = false;
    std::cout << "Press any key to start the game:\n";
}


void Game::FrameStart()
{
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Game::Update(float deltaTimeSeconds)
{
    if (!gameStarted) 
        return;

    player->UpdateBullets(deltaTimeSeconds);

    level->MoveEnemies(deltaTimeSeconds, player->pos);
    level->RotateEnemiesTowards(player->pos);

    DetectTriggerCollisions();

    RenderLifes();
    RenderPlayer();
    RenderCrosshair();
    RenderMap();
    RenderEnemies();
    RenderBullets();
}


void Game::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Game::OnInputUpdate(float deltaTime, int mods)
{
    if (!gameStarted) return;

    // efectuarea miscarii in jurul hartii
    {
        glm::vec2 movement = glm::vec2(0);
        if (window->KeyHold(GLFW_KEY_W)) 
        {
            movement = glm::vec2(0, -player->moveSpeed * deltaTime);
        }
        else if (window->KeyHold(GLFW_KEY_S))
        {
            movement = glm::vec2(0, player->moveSpeed * deltaTime);
        }
        if (window->KeyHold(GLFW_KEY_A)) 
        {
            movement += glm::vec2(player->moveSpeed * deltaTime, 0);
        }
        else if (window->KeyHold(GLFW_KEY_D))
        {
            movement += glm::vec2(-player->moveSpeed * deltaTime, 0);
        }

        // prezicerea coliziunilor aici pentru a preveni tremuratul (mutand jucatorul in interiorul peretului, apoi fortand-ul sa iasa)
        bool noFutureCollision = true;
        for each (Wall *w in level->walls) 
        {
            if (CollisionAABBCircle(w->pos + movement, w->size, player->pos, player->size.x)) 
            {
                noFutureCollision = false;
            }
        }

        bool hasMoved = movement != glm::vec2(0);
        if (noFutureCollision && hasMoved)
        {
            level->MoveObjects(movement);
            player->MoveBullets(movement);
        }
    }
}


void Game::OnKeyPress(int key, int mods)
{
    if (!gameStarted) {
        gameStarted = true;
        std::cout << "Game started! Current score: " << *score << '\n';
    }
}


void Game::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 res = window->GetResolution();

    player->lookAt = glm::vec2(mouseX, (res.y - 1) - mouseY);

    glm::vec2 adjacent{ glm::vec2(player->initialLookAt.x - player->pos.x, player->initialLookAt.y - player->pos.y) };
    glm::vec2 hypotenuse{ glm::vec2(player->lookAt.x - player->pos.x, player->lookAt.y - player->pos.y) };

    player->lookAngle = glm::acos(glm::dot(glm::normalize(adjacent), glm::normalize(hypotenuse)));  // radiani

    //pentru gama completa de miscare:
    if (mouseY > res.y / 2)
    {
        player->lookAngle *= -1;
    }
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (!gameStarted) 
    {
        gameStarted = true;
        return;
    }

    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) 
    {
        if (player->cooldownTimer == 0) 
        {
            bool foundBullet = false;
            for each (Bullet *b in player->bullets)
            {
                if (b->available)
                {
                    b->pos = player->pos;
                    b->direction = glm::normalize(player->lookAt - player->pos);
                    b->angle = player->lookAngle;
                    b->available = false;
                    foundBullet = true;
                    break;
                }
            }
            if (!foundBullet)
            {  
                // in cazul in care "firing rate" a crescut
                Bullet *b = new Bullet(player->pos, glm::normalize(player->lookAt - player->pos), player->lookAngle);
                b->available = false;
                player->bullets.push_back(b);
            }
            player->cooldownTimer = player->cooldown;
        }
    }
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Game::OnWindowResize(int width, int height)
{
}

bool Game::CollisionAABBAABB(glm::vec2 pos1, glm::vec2 size1, glm::vec2 pos2, glm::vec2 size2) 
{
    // ciocnire axa X
    bool collisionX = pos1.x + size1.x >= pos2.x && pos2.x + size2.x >= pos1.x;
    // ciocnire axa Y
    bool collisionY = pos1.y + size1.y >= pos2.y && pos2.y + size2.y >= pos1.y;
    // ciocnire doar pe ambele axe
    return collisionX && collisionY;
}

bool Game::CollisionAABBCircle(glm::vec2 pos1, glm::vec2 size1, glm::vec2 pos2, float radius2) 
{
    // obtinem intai cercul punctului din centru 
    glm::vec2 center(pos2);
    // calculam informatile AABB (center, half-extents)
    glm::vec2 aabb_half_extents(size1.x / 2.0f, size1.y / 2.0f);
    glm::vec2 aabb_center(pos1.x + aabb_half_extents.x, pos1.y + aabb_half_extents.y);
    // calculam diferenta intre ambele centre
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // adaugam valoarea obtinuta mai sus la aabb_center si obtinem valoarea figurii cea mai apropiata de cerc
    glm::vec2 closest = aabb_center + clamped;
    // recuperam vectorul dintre cercul central cel mai apropiat punct AABB si verificam daca length <= radius
    difference = closest - center;
    return glm::length(difference) < radius2;
}

void Game::DetectTriggerCollisions() 
{
    for each (Wall *w in level->walls)
    {
        for each (Bullet *b in player->bullets)
        {
            if (b->available) continue;  // daca glontul este disponibil sa fie eliberat, inseamna ca nu este generat

            if (CollisionAABBAABB(b->pos, b->size, w->pos, w->size))
            {
                // proiectilul a lovit peretele
                // resetam proiectilul
                b->Reset();
            }
        }
    }

    for each (Enemy *e in level->enemies)
    {
        for each (Bullet *b in player->bullets)
        {
            if (b->available) continue;

            if (CollisionAABBAABB(b->pos, b->size, e->pos, e->size)) 
            {
                // proiectilul a lovit inamicul
                // reapar inamici
                e->Spawn();

                // resetam proiectilul
                b->Reset();

                // marim scorul jucatorului
                *score += e->points;
                std::cout << "Score increased by " << e->points << " => score = " << *score << '\n';
            }
        }

        if (CollisionAABBCircle(e->pos, e->size, player->pos, player->size.x))
        {
            // inamicul loveste jucatorul
            // reapar inamici
            e->Spawn();

            // scadem vietile jucatorului
            player->lifes -= 1;
            cout << "Lifes = " << player->lifes << '\n';

            if (player->lifes == 0) 
            {
                //daca vietile jucatorului s-au epuizat
                std::cout << "Game Over! Out of lifes! Final Score: " << *score << '\n';
                //se reseteaza jocul
                RestartGame();
                std::cout << "Press any key to start the game.\n";
            }
        }
    }
}

void Game::RestartGame() 
{
    gameStarted = false;
    player->lifes = 3;
    level->Restart();
    *score = 0;

    for each (Bullet *b in player->bullets)
    {
        b->Reset();
    }
}

void Game::RenderPlayer() 
{
    glm::mat3 modelMatrix;

    // desenam mainile jucatorului din cercuri
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(player->pos.x, player->pos.y);
        modelMatrix *= transform2D::Rotate(player->lookAngle);
        modelMatrix *= transform2D::Translate(player->size.x, player->size.y / 2);
        modelMatrix *= transform2D::Scale(player->size.x / 4, player->size.y / 4);

        RenderMesh2D(meshes["playerBody"], modelMatrix, player->color);

        // pentru conturul negru
        modelMatrix *= transform2D::Scale(1.25f, 1.25f);
        RenderMesh2D(meshes["playerBody"], shaders["VertexColor"], modelMatrix);


        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(player->pos.x, player->pos.y);
        modelMatrix *= transform2D::Rotate(player->lookAngle);
        modelMatrix *= transform2D::Translate(player->size.x, -player->size.y / 2);
        modelMatrix *= transform2D::Scale(player->size.x / 4, player->size.y / 4);

        RenderMesh2D(meshes["playerBody"], modelMatrix, player->color);

        // pentru conturul negru
        modelMatrix *= transform2D::Scale(1.25f, 1.25f);
        RenderMesh2D(meshes["playerBody"], shaders["VertexColor"], modelMatrix);
    }

    // desenam corpul jucatorului in forma de cerc
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(player->pos.x, player->pos.y);
        modelMatrix *= transform2D::Scale(player->size.x, player->size.y);
        RenderMesh2D(meshes["playerBody"], modelMatrix, player->color);

        // pentru conturul negru
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(player->pos.x, player->pos.y);
        modelMatrix *= transform2D::Scale(player->size.x, player->size.y);
        modelMatrix *= transform2D::Scale(1.075f, 1.075f);
        RenderMesh2D(meshes["playerBody"], shaders["VertexColor"], modelMatrix);
    }
}

void Game::RenderCrosshair()
{
    // desenam patratul negru care tine loc de cursorul mouse-ului
    glm::mat3 modelMatrix{ glm::mat3(1) };
    modelMatrix *= transform2D::Translate(player->lookAt.x, player->lookAt.y);
    modelMatrix *= transform2D::Scale(4.0f, 4.0f);

    RenderMesh2D(meshes["playerBody"], shaders["VertexColor"], modelMatrix);
}

void Game::RenderMap()
{
    glm::mat3 modelMatrix;

    // desenam peretii
    for each (Wall *w in level->walls)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(w->pos.x, w->pos.y);
        modelMatrix *= transform2D::Scale(w->size.x, w->size.y);
        RenderMesh2D(meshes["square"], modelMatrix, w->color);
    }
}

void Game::RenderEnemies()
{
    glm::mat3 modelMatrix;

    for each (Enemy *e in level->enemies)
    {
        if (!e->isAlive) continue;

        // desenam partea din fata a inamicilor prin patrate
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(e->pos.x, e->pos.y);
            modelMatrix *= transform2D::Translate(e->size.x / 2, e->size.y / 2);  // rotim folosind mijlocul patratului
            modelMatrix *= transform2D::Rotate(e->lookAngle);
            modelMatrix *= transform2D::Translate(-e->size.x / 2, -e->size.y / 2);  // inapoi la coltul din stanga jos 
            modelMatrix *= transform2D::Translate(e->size.x, e->size.y * 0.7f);
            modelMatrix *= transform2D::Scale(e->size.x / 5, e->size.y / 5);

            RenderMesh2D(meshes["enemyBody"], modelMatrix, e->color);

            // pentru conturul negru
            modelMatrix *= transform2D::Translate(-0.25, -0.25);
            modelMatrix *= transform2D::Scale(1.5f, 1.5f);
            RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);


            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(e->pos.x, e->pos.y);
            modelMatrix *= transform2D::Translate(e->size.x / 2, e->size.y / 2);  // rotim folosind mijlocul patratului
            modelMatrix *= transform2D::Rotate(e->lookAngle);
            modelMatrix *= transform2D::Translate(-e->size.x / 2, -e->size.y / 2);  // inapoi la coltul din stanga jos
            modelMatrix *= transform2D::Translate(e->size.x, e->size.y * 0.1f);
            modelMatrix *= transform2D::Scale(e->size.x / 5, e->size.y / 5);

            RenderMesh2D(meshes["enemyBody"], modelMatrix, e->color);

            // pentru conturul negru
            modelMatrix *= transform2D::Translate(-0.25, -0.25);
            modelMatrix *= transform2D::Scale(1.5f, 1.5f);
            RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);
        }

        // desenam corpul inamicilor
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(e->pos.x, e->pos.y);
            modelMatrix *= transform2D::Translate(e->size.x / 2, e->size.y / 2);  // rotim folosind mijlocul patratului
            modelMatrix *= transform2D::Rotate(e->lookAngle);
            modelMatrix *= transform2D::Translate(-e->size.x / 2, -e->size.y / 2);  // inapoi la coltul din stanga jos
            modelMatrix *= transform2D::Scale(e->size.x, e->size.y);
            RenderMesh2D(meshes["enemyBody"], modelMatrix, e->color);

            // pentru conturul negru
            modelMatrix *= transform2D::Scale(1.1f, 1.1f);
            modelMatrix *= transform2D::Translate(-0.05f, -0.05f);
            RenderMesh2D(meshes["enemyBody"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Game::RenderBullets()
{
    //desenam proiectilele
    glm::mat3 modelMatrix;

    for each (Bullet *b in player->bullets) {
        if (!b->available) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(b->pos.x, b->pos.y);
            modelMatrix *= transform2D::Translate(b->size.x / 2, b->size.y / 2);  // rotim folosind mijlocul patratului
            modelMatrix *= transform2D::Rotate(b->angle);
            modelMatrix *= transform2D::Translate(-b->size.x / 2, -b->size.y / 2);  // inapoi la coltul din stanga jos
            modelMatrix *= transform2D::Scale(b->size.x, b->size.y);
            RenderMesh2D(meshes["square"], modelMatrix, b->color);
        }
    }
}

void Game::RenderLifes() 
{
    //desenam vietiile jucatorului 
    glm::ivec2 resolution = window->GetResolution();

    glm::mat3 modelMatrix;
    glm::vec3 color{ glm::vec3(1, 0, 0) };
    glm::vec2 size{ glm::vec2(40, 40) };

    // desenam fiecare patrat rosu individual ce reprezinta o viata a jucatorului
    for (int i = 0; i < player->lifes; ++i) 
    {
        modelMatrix = glm::mat3(1);
        glm::vec2 pos = glm::vec2(i * size.x, 0);
        modelMatrix *= transform2D::Translate(resolution.x - 3 * size.x - size.x / 2, size.y / 2);
        modelMatrix *= transform2D::Translate(pos.x, pos.y);
        modelMatrix *= transform2D::Scale(size.x, size.y);
        RenderMesh2D(meshes["square"], modelMatrix, color);

    }

    // desenam cadranul negru pentru bara de viata
    glm::vec2 frameSize = size;
    frameSize.x *= 3;
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(resolution.x - 3 * size.x - size.x / 2, size.y / 2);
    modelMatrix *= transform2D::Translate(-0.8f, 0);
    modelMatrix *= transform2D::Scale(frameSize.x, frameSize.y);
    modelMatrix *= transform2D::Scale(1.01f, 1.01f);
    RenderMesh2D(meshes["squareFrame"], shaders["VertexColor"], modelMatrix);
}
