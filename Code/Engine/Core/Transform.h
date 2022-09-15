#pragma once
#include "glm/glm.hpp"
#include "Component.h"

class Transform : public Component
{
public:
	Transform();
	Transform(glm::vec3 _position, glm::vec3 scale);

private:
	glm::vec3 position, scale;
};

