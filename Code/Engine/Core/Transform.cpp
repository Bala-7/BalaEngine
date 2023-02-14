#include "Transform.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Engine/Editor/imgui_widgets.h"

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

void Transform::DrawInspector()
{
	ImGui::Separator();
	ImGui::Text("TRANSFORM");
	ImGui::Separator();
	ImGui::Text("Position");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Position", position);
	ImGui::Separator();
	ImGui::Text("Rotation");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Rotation", rotation);
	ImGui::Separator();
	ImGui::Text("Scale");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Scale", scale);
}