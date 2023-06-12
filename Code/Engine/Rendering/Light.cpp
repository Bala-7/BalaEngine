#include "Light.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Editor/imgui_widgets.h"

Light::Light()
{
	componentType = ComponentType::LIGHT;
}

Light::Light(LightType lightType, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic, float innerCutOff, float outerCutOff)
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
}

Light::~Light()
{
}

void Light::OnComponentAdded()
{
}

void Light::DrawInspector()
{
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
}
