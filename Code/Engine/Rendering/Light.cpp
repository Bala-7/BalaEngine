#include "Light.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Editor/imgui_widgets.h"
#include "Engine/Core/GameObject.h"
#include "Engine/Core/SceneNode.h"
#include "Engine/Core/SceneGraph.h"



Light::Light()
{
	componentType = ComponentType::LIGHT;
}

Light::Light(LightType lightType, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff, float nearPlane, float farPlane)
{
	_lightTtype = lightType;
	_direction = direction;
	
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;

	_constant = constant;
	_linear = linear;
	_quadratic = quadratic;

	_innerCutOff = innerCutOff;
	_outerCutOff = outerCutOff;

	_shadowNearPlane = nearPlane;
	_shadowFarPlane = farPlane;
}

Light::~Light()
{
}

void Light::OnComponentAdded()
{
	gameObject->GetNode()->GetScene()->AddToSceneLights(this);
}

void Light::DrawInspector()
{
	const char* types[] = { "Directional", "Point", "Spot" };
	ImGui::Separator();
	ImGui::Separator();
	ImGui::Text("LIGHT SETTINGS");
	ImGui::Separator();
	if (ImGui::Combo("Type", reinterpret_cast<int*>(&_lightTtype), types, IM_ARRAYSIZE(types)))
	{
		// Handle the selection change here if needed
		// The selectedOption variable will hold the new selected value
	}
	ImGui::Separator();
	ImGui::Text("Direction");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Direction", _direction);
	ImGui::Separator();
	ImGui::Text("Ambient");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Ambient", _ambient);
	ImGui::Separator();
	ImGui::Text("Diffuse");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Diffuse", _diffuse);
	ImGui::Separator();
	ImGui::Text("Specular");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Specular", _specular);

	ImGui::Separator();
	ImGui::Text("Constant");
	ImGui::Separator();
	ImGui::DragFloat("##Constant", &_constant, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::Separator();

	ImGui::Text("Linear");
	ImGui::Separator();
	ImGui::DragFloat("##Linear", &_linear, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::Separator();
	
	ImGui::Text("Quadratic");
	ImGui::Separator();
	ImGui::DragFloat("##Quadratic", &_quadratic, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::Separator();

	ImGui::Text("Near plane");
	ImGui::Separator();
	ImGui::DragFloat("##NearPlane", &_shadowNearPlane, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::Separator();

	ImGui::Text("Far plane");
	ImGui::Separator();
	ImGui::DragFloat("##FarPlane", &_shadowFarPlane, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::Separator();
}
