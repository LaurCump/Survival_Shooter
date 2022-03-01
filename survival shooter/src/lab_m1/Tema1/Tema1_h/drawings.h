#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace drawings
{
    //Creeaza patratul cu coltul din stanga jos, lungimea si culoarea date
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

    //Creeaza cercul cu culoarea data
    Mesh* CreateCircle(std::string name, glm::vec3 color);
}
