#include "lab_m1/Tema1/Tema1_h/wall.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace m1;

Wall::Wall(glm::vec2 pos, glm::vec2 size, glm::vec3 color)
{
	this->pos = pos;
	this->size = size;
	this->color = color;
}

Wall::~Wall()
{
}
