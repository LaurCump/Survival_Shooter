#include "lab_m1/Tema1/Tema1_h/drawings.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* drawings::CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) 
	{
        square->SetDrawMode(GL_LINE_LOOP);
    } 
	else 
	{
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* drawings::CreateCircle(std::string name, glm::vec3 color)
{
	glm::vec3 center{ glm::vec3(0, 0, 0) };
	float radius{ 1 };

	int i;
	float angle = 0;
	int points = 60;
	float angleOffset = 360.0f / points;

	std::vector<VertexFormat> vertices
	{
		VertexFormat(center, color)
	};

	for (i = 1; i <= points; ++i) 
	{
		float x = center.x + glm::cos(glm::radians(angle)) * radius;
		float y = center.y + glm::sin(glm::radians(angle)) * radius;
		float z = 0;

		VertexFormat v = VertexFormat(glm::vec3(x, y, z), color);
		vertices.push_back(v);
		angle += angleOffset;
	}

	Mesh* circle = new Mesh(name);

	std::vector<unsigned int> indices;

	for (i = 0; i <= points; ++i) 
	{
		indices.push_back(i);
	}
	indices.push_back(1);

	circle->SetDrawMode(GL_TRIANGLE_FAN);

	circle->InitFromData(vertices, indices);

	return circle;
}
