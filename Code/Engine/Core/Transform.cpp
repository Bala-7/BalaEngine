#include "Transform.h"

Transform::Transform()
{
	position = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);
	componentType = ComponentType::TRANSFORM;
}

Transform::Transform(glm::vec3 _position, glm::vec3 _scale, glm::vec3 _rotation)
{
	position = _position;
	scale = _scale;
	rotation = _rotation;
}

void Transform::OnComponentAdded()
{
	
}