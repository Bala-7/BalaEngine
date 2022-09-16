#pragma once
#include "glm/glm.hpp"
#include "Component.h"

class Transform : public Component
{
public:
	glm::vec3 position, scale;

	Transform();
	Transform(glm::vec3 _position, glm::vec3 scale);

	void OnComponentAdded() override;

	
private:
	
};

