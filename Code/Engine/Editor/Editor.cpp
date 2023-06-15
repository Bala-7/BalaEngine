#include "Editor.h"

#include "Engine/Rendering/RenderEngine.h"
#include "Engine/Debug/Debug.h"






void Editor::Initialize()
{
	// IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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

	// Set up ImGui windows and layout
	ImGui::DockSpaceOverViewport();

	DrawSceneGraphWindow();
	// Begin the main menu bar
	bool showSceneGraphWindow;
	bool showSceneViewWindow;
	bool showInspectorWindow;
	bool showLightingWindow;

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Window")) {
			ImGui::MenuItem("Scene Graph", nullptr, &showSceneGraphWindow);
			ImGui::MenuItem("Scene View", nullptr, &showSceneViewWindow);
			ImGui::MenuItem("Inspector", nullptr, &showInspectorWindow);
			ImGui::MenuItem("Lighting", nullptr, &showLightingWindow);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("GameObject Inspector");
	ImGui::Text("Edit object parameters");
	ImGui::Checkbox("Auto rotate", &autoRotate);
	if(autoRotate)
		ImGui::SliderFloat("Rotation Speed", &rotationSpeed, 0.0f, 20.0f);
	
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

	DrawConsoleWindow();
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

bool Editor::IsMouseOverSceneView()
{
	return mouseOverSceneView;
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
	sceneGraph->DrawEditorWindow();
}


void Editor::DrawLightingWindow()
{
	ImGui::Begin("Lighting");
	ImGui::Text("Environment Color");
	ImGui::Separator();
	ImGui::ColorEdit4("##Environment Color", color);
	ImGui::End();
}

void Editor::DrawConsoleWindow()
{
	ImGui::Begin("Console");
	if (ImGui::BeginTabBar("Console TabBar")) {
		if (ImGui::BeginTabItem("Log")) {
			ImGui::TextUnformatted(Debug::GetLogBuffer());
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Warnings")) {
			ImGui::Text("This tab will show the warnings");
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Errors")) {
			ImGui::Text("This tab will show the errors");
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	
	ImGui::End();
}

bool Editor::IsMouseOverWindow(const ImVec2& windowPos, const ImVec2& windowSize) 
{
	const ImVec2 mousePos = ImGui::GetIO().MousePos;
	return mousePos.x >= windowPos.x && mousePos.x <= (windowPos.x + windowSize.x) &&
		mousePos.y >= windowPos.y && mousePos.y <= (windowPos.y + windowSize.y);
}

void Editor::DrawSceneViewWindow()
{
	int subViewportWidth = 854;
	int subViewportHeight = 480;
	int subViewportX = 100;
	int subViewportY = 100;
	

	ImGui::Begin("Scene View", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	const ImVec2 windowPos = ImGui::GetWindowPos();
	const ImVec2 windowSize = ImGui::GetWindowSize();

	mouseOverSceneView = IsMouseOverWindow(windowPos, windowSize);

	// Render the sub-viewport content within the ImGui frame
	ImGui::BeginChild("SubViewport", ImVec2(subViewportWidth, subViewportHeight), true);
	{
		// we access the ImGui window size
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;

		// we get the screen position of the window
		ImVec2 pos = ImGui::GetCursorScreenPos();

		// we rescale the framebuffer to the actual window size here and reset the glViewport 
		RenderEngine::GetInstance()->RescaleFramebuffer(window_width, window_height);
		int fullWindowWidth, fullWindowHeight;
		glfwGetWindowSize(RenderEngine::GetInstance()->GetWindow(), &fullWindowWidth, &fullWindowHeight);
		glViewport(pos.x, fullWindowHeight -pos.y-window_height, window_width, window_height);

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
