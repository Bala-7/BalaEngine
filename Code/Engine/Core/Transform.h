#pragma once
#include "glm/glm.hpp"
#include "Component.h"

class Transform : public Component
{
public:
	glm::vec3 position, scale, rotation;

	Transform();
	Transform(glm::vec3 _position, glm::vec3 scale, glm::vec3 _rotation);

	void OnComponentAdded() override;
	void DrawInspector() override;

	
private:
	
};

