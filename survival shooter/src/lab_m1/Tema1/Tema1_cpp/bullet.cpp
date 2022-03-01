#include "lab_m1/Tema1/Tema1_h/bullet.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

Bullet::Bullet(glm::vec2 pos, glm::vec2 direction, float angle, glm::vec2 size, glm::vec3 color, float speed)
{
	this->pos = pos;
	this->direction = direction;
	this->size = size;
	this->color = color;
	this->speed = speed;
	this->angle = angle;
	mileage = 0;
	available = true;
}

Bullet::~Bullet()
{
}

void Bullet::Reset() {
	available = true;
	mileage = 0;
}
