#include "lab_m1/Tema1/Tema1_h/player.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

Player::Player(glm::vec2 pos, glm::vec2 size, glm::vec3 color, float moveSpeed, int ammo, int lifes)
{
	this->pos = pos;
	this->size = size;
	this->color = color;
	this->moveSpeed = moveSpeed;
	this->ammo = ammo;
	this->lifes = lifes;
	initialLookAt = pos + glm::vec2(pos.x + 1000, 0);
	lookAt = glm::vec2(0);
	lookAngle = 0;
	cooldownTimer = 0;

	for (int i = 0; i < ammo; ++i) 
	{
		bullets.push_back(new Bullet());
	}
}

Player::~Player()
{
	for each (Bullet *b in bullets)
	{
		delete b;
	}
	bullets.clear();
}

void Player::MoveBullets(glm::vec2 movement) 
{
	for each (Bullet *b in bullets) 
	{
		if (!b->available) 
		{
			b->pos += movement;
		}
	}
}

void Player::UpdateBullets(float deltaTimeSeconds) 
{
	for each (Bullet *b in bullets) 
	{
		// actualizare raza de tragere a gloantelor
		if (!b->available)
		{
			if (b->mileage >= b->maxRange) 
			{
				b->Reset();
				continue;
			}

			// actualizare pozitia glontului
			glm::vec2 distance{b->direction.x * deltaTimeSeconds * b->speed, b->direction.y * deltaTimeSeconds * b->speed};

			b->pos.x += distance.x;
			b->pos.y += distance.y;

			b->mileage += glm::length(distance);
		}
	}
	cooldownTimer = glm::max(cooldownTimer - deltaTimeSeconds, 0.0f);
}
