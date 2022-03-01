#pragma once

#include "components/simple_scene.h"

namespace m1
{
    class Wall
    {
    public:
        Wall(glm::vec2 pos = glm::vec2(0),
            glm::vec2 size = glm::vec2(10.0f, 10.0f),
            glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f));
        ~Wall();

    public:
        glm::vec2 pos;
        glm::vec2 size;
        glm::vec3 color;
    };
}
