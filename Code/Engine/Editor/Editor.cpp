#include "Editor.h"

#include "Engine/Rendering/RenderEngine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_widgets.h"

void Editor::Initialize()
{
	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(RenderEngine::GetInstance()->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 450");
	// \IMGUI
}

void Editor::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Editor::DrawEditorWindows()
{
	// ImGui render
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("ImGui Editor Window");
	ImGui::Text("Edit object parameters");
	ImGui::Checkbox("Auto rotate", &autoRotate);
	if(autoRotate)
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.0f, 20.0f);
	if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::Separator();
		ImGui::Text("Environment Color");
		ImGui::Separator(); 
		ImGui::ColorEdit4("##Environment Color", color);

	}
	ImGui::Separator();
	ImGui::Separator();
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Cube"))
	{
		DrawInspector();
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// \ImGui render
}

void Editor::SetDisplayedGameObject(GameObject* go)
{
	_displayedGameObject = go;
}

GameObject* Editor::GetDisplayedGameObject()
{
	return _displayedGameObject;
}

void Editor::DrawInspector()
{
	for (auto i : _displayedGameObject->GetComponentsList())
	{
		if(i->GetType() == Component::ComponentType::TRANSFORM)
			DrawComponent(static_cast<Transform*>(i));
		else if (i->GetType() == Component::ComponentType::MESH_RENDERER)
			DrawComponent(static_cast<MeshRenderer*>(i));
	}

}

void Editor::DrawComponent(Transform* transform)
{
	ImGui::Separator();
	ImGui::Text("TRANSFORM");
	ImGui::Separator();
	ImGui::Text("Position");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Position", transform->position);
	ImGui::Separator();
	ImGui::Text("Rotation");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Rotation", transform->rotation);
	ImGui::Separator();
	ImGui::Text("Scale");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Scale", transform->scale);
}

void Editor::DrawComponent(MeshRenderer* meshRenderer)
{
	ImGui::Separator();
	ImGui::Text("MESH RENDERER");
	ImGui::Separator();
	ImGui::Text("Ambient");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Ambient", meshRenderer->GetMaterial()->ambient);
	ImGui::Separator();
	ImGui::Text("Diffuse");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Diffuse", meshRenderer->GetMaterial()->diffuse);
	ImGui::Separator();
	ImGui::Text("Specular");
	ImGui::Separator();
	nimgui::draw_vec3_widget("Specular", meshRenderer->GetMaterial()->specular);
	ImGui::Separator();
	ImGui::Text("Shininess");
	ImGui::Separator();
	ImGui::DragFloat("##Shininess", &meshRenderer->GetMaterial()->shininess, 0.1f, 2.0f, 256.0f, "%.2f");
	
}
