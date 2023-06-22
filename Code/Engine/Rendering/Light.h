#pragma once
#include "Engine/Core/Component.h"

#include "glm/glm.hpp"

class Light : public Component
{
public:
	enum class LightType { Directional, Point, Spot };

	Light();
	Light(LightType lightType, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
		float constant, float linear, float quadratic,
		float innerCutOff, float outerCutOff);

	~Light();

	void OnComponentAdded() override;
	void DrawInspector() override;

	glm::vec3 GetDirection() { return _direction; }
	glm::vec3 GetAmbient() { return _ambient; }
	glm::vec3 GetDiffuse() { return _diffuse; }
	glm::vec3 GetSpecular() { return _specular; }

	float GetConstant() { return _constant; }
	float GetLinear() { return _linear; }
	float GetQuadratic() { return _quadratic; }

	float GetInnerCutOff() { return _innerCutOff; }
	float GetOuterCutOff() { return _outerCutOff; }

	LightType GetType() { return _lightTtype; }

private:

	glm::vec3 _direction;

	glm::vec3 _ambient;
	glm::vec3 _diffuse;
	glm::vec3 _specular;

	float _constant;
	float _linear;
	float _quadratic;

	float _innerCutOff;
	float _outerCutOff;

	LightType _lightTtype;
};

