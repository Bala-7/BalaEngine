#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	componentType = ComponentType::TRANSFORM;
}

Transform::Transform(glm::vec3 _position, glm::vec3 _scale)
{
	position = _position;
	scale = _scale;
}

void Transform::OnComponentAdded()
{
	
}