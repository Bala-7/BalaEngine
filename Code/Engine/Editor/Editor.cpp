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
	DrawSceneGraphWindow();
	
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	//ImGui::SetNextWindowSize(ImVec2(400, 400));
	ImGui::Begin("GameObject Inspector");
	ImGui::Text("Edit object parameters");
	ImGui::Checkbox("Auto rotate", &autoRotate);
	if(autoRotate)
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.0f, 20.0f);
	
	/*if (ImGui::CollapsingHeader("Light"))
	{
		ImGui::Separator();
		ImGui::Text("Environment Color");
		ImGui::Separator(); 
		ImGui::ColorEdit4("##Environment Color", color);

	}*/


	
	ImGui::Separator();
	ImGui::Separator();
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Selected GameObject"))
	{
		DrawInspector();
	}
	ImGui::End();

	DrawLightingWindow();
	

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
		i->DrawInspector();
	}

}

void Editor::DrawSceneGraphWindow()
{
	ImGui::SetNextWindowPos(ImVec2(200, 0));
	//ImGui::SetNextWindowSize(ImVec2(400, 400));
	sceneGraph->DrawEditorWindow();
}


void Editor::DrawLightingWindow()
{
	ImGui::SetNextWindowPos(ImVec2(400, 0));
	ImGui::Begin("Lighting");
	ImGui::Text("Environment Color");
	ImGui::Separator();
	ImGui::ColorEdit4("##Environment Color", color);
	ImGui::End();
}
