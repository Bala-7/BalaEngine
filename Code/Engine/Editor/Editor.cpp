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
	
	DrawSceneViewWindow();
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

void Editor::DrawSceneViewWindow()
{
	int subViewportWidth = 854;
	int subViewportHeight = 480;
	int subViewportX = 100;
	int subViewportY = 100;

	ImGui::SetNextWindowPos(ImVec2(426, 240));
	
	
	ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	// Render the sub-viewport content within the ImGui frame
	ImGui::BeginChild("SubViewport", ImVec2(subViewportWidth, subViewportHeight), true);
	{
		// Get the draw list for the sub-viewport
		/*ImDrawList* drawList = ImGui::GetWindowDrawList();

		// Clear the sub-viewport with a background color
		drawList->AddRectFilled(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y), IM_COL32(0, 0, 1.0f, 255));

		// Perform your OpenGL rendering commands for the sub-viewport content
		glViewport(subViewportX, subViewportY, subViewportWidth, subViewportHeight);
		glScissor(subViewportX, subViewportY, subViewportWidth, subViewportHeight);
		glEnable(GL_SCISSOR_TEST);

		// Clear the color and depth buffers for the sub-viewport content
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		// Render your OpenGL code for the sub-viewport here
		//scene->Draw();

		glDisable(GL_SCISSOR_TEST);*/

		// we access the ImGui window size
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;

		// we get the screen position of the window
		ImVec2 pos = ImGui::GetCursorScreenPos();

		// we rescale the framebuffer to the actual window size here and reset the glViewport 
		RenderEngine::GetInstance()->RescaleFramebuffer(window_width, window_height);
		glViewport(pos.x, 720-pos.y-window_height, window_width, window_height);

		// and here we can add our created texture as image to ImGui
		// unfortunately we need to use the cast to void* or I didn't find another way tbh
		ImGui::GetWindowDrawList()->AddImage(
			(void*)RenderEngine::GetInstance()->GetFrameBufferTexture(),
			ImVec2(pos.x, pos.y),
			ImVec2(pos.x + window_width, pos.y + window_height),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
	}
	
	

	ImGui::EndChild();

	ImGui::End();
	


}
