#include "lab_m1/Tema1/Tema1_h/enemy.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

Enemy::Enemy(glm::vec2 mapBottomLeftPixel, glm::vec2 mapTopRightPixel, glm::vec2 size, glm::vec3 color, float moveSpeed, int points)
{
	this->mapBottomLeftPixel = mapBottomLeftPixel;
	this->mapTopRightPixel = mapTopRightPixel;
	this->pos = glm::vec2(0, 0);
	this->size = size;
	this->color = color;
	this->moveSpeed = moveSpeed;
	this->points = points;
	initialLookAt = pos + glm::vec2(pos.x + 1000, 0);
	lookAt = glm::vec2(0);
	lookAngle = 0;

	isAlive = false;
	Spawn();
}

Enemy::~Enemy()
{
}

void Enemy::Spawn() {
	int randomNum = rand() % 3;
	int mapSize = mapTopRightPixel.x - mapBottomLeftPixel.x;
	int limit = 200;

	if (randomNum == 0) 
	{
		// genereaza orice x < mapBottomLeftPixel.x, dar nu prea departe
		pos.x = mapBottomLeftPixel.x - rand() % limit;
		// genereaza orice y, dar nu prea departe
		pos.y = (mapBottomLeftPixel.y - limit) + rand() % (mapSize + 2 * limit);
	} 

	else if (randomNum == 1) 
	{
		// genereaza x in interiorul granitelor hartii
		pos.x = mapBottomLeftPixel.x + rand() % mapSize;

		// genereaza y in afara granitelor hartii
		int whichBorderSide = rand() % 2;
		if (whichBorderSide == 0) 
		{
			pos.y = mapBottomLeftPixel.y - rand() % limit;
		} 
		else 
		{
			pos.y = mapTopRightPixel.y + rand() % limit;
		}
	} 

	else if (randomNum == 2) 
	{
		// genereaza orice x > mapTopRightPixel.x, dar nu prea departe
		pos.x = mapTopRightPixel.x + rand() % limit;
		// genereaza orice y, dar nu prea departe
		pos.y = (mapBottomLeftPixel.y - limit) + rand() % (mapSize + 2 * limit);
	}

	isAlive = true;
}
