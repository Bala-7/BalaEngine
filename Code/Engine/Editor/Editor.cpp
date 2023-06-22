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

	std::vector<std::string> options = { "ModelRenderer", "Light" };
	if (ImGui::CollapsingHeader("Selected GameObject"))
	{
		DrawInspector();
		
		if(showAddComponentModal)
		{
			ImGui::OpenPopup("Add Component");
			if (ImGui::BeginPopupModal("Add Component", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				for (const auto& option : options)
				{
					if (ImGui::Button(option.c_str()))
					{
						//gameObject->AddComponent(option.second);
						Debug::Log("Adding component '" + option + "'.");
						showAddComponentModal = false;
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}
		if (ImGui::Button("Add Component"))
		{
			showAddComponentModal = true;
		}

	}
	ImGui::End();

	DrawLightingWindow();
	
	DrawSceneViewWindow();
	DrawShadowMapWindow();
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
	ImGui::Begin("Scene Graph");
	std::vector<std::pair<SceneNode*, int>> itemsVector;
	sceneGraph->GetRootNode()->GetChildNamesForEditor(0, itemsVector);
	
	static int selectedItemIndex = -1;
	int previousDepth = 0;
	for (size_t i = 0; i < itemsVector.size(); ++i)
	{
		const std::pair<SceneNode*, int>& pair = itemsVector[i];
		if (pair.first->GetGameObject())
		{
			int difference = pair.second - previousDepth;
			if (difference > 0)
				ImGui::Indent(12 * difference);
			else if (difference < 0)
				ImGui::Unindent(12 * (-difference));

			previousDepth = pair.second;
			if (ImGui::Selectable(pair.first->GetGameObject()->name.c_str(), selectedItemIndex == i))
			{
				Editor::Instance()->SetDisplayedGameObject(pair.first->GetGameObject());
				selectedItemIndex = i;
			}
		}
	}

	ImGui::Separator();


	// -- Adding a new node--
	if (ImGui::Button("Add Node"))
	{
		showNewNodeTextField = true;
		newNodeName = "";
		Debug::Log("Adding node");
	}

	if (showNewNodeTextField)
	{
		char buffer[256];  // Buffer for the input text

		strncpy(buffer, newNodeName.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';  // Ensure null-termination

		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_CallbackCharFilter, HandleInputText, nullptr))
		{
			newNodeName = buffer;
		}

		if (ImGui::Button("Confirm"))
		{
			sceneGraph->GetRootNode()->AddChild(new SceneNode(new GameObject(newNodeName)));
			// Do something with the entered name
			showNewNodeTextField = false;
		}
	}
	ImGui::Separator();

	// -- Adding a child node --
	if (ImGui::Button("Add Child Node"))
	{
		showNewChildTextField = true;
		newNodeName = "";
		Debug::Log("Adding child node");
	}

	if (showNewChildTextField)
	{
		char buffer[256];  // Buffer for the input text

		strncpy(buffer, newNodeName.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';  // Ensure null-termination

		if (ImGui::InputText("Name", buffer, sizeof(buffer), ImGuiInputTextFlags_CallbackCharFilter, HandleInputText, nullptr))
		{
			newNodeName = buffer;
		}

		if (ImGui::Button("Confirm"))
		{
			SceneNode* nodeToAddChildTo = itemsVector[selectedItemIndex].first;
			nodeToAddChildTo->AddChild(new SceneNode(new GameObject(newNodeName)));
			Debug::Log("Added child node '" + newNodeName + "' to '" + nodeToAddChildTo->GetGameObject()->name + "'.");
			// Do something with the entered name
			showNewChildTextField = false;
		}
	}

	ImGui::Separator();
	// -- Deleting node --
	if (ImGui::Button("Delete"))
	{
		SceneNode* nodeToDelete = itemsVector[selectedItemIndex].first;
		nodeToDelete->Destroy(); // TODO: Access to the SceneNode, not the GameObject
		Debug::Log("Deleted node '" + nodeToDelete->GetGameObject()->name + "'.");
	}
	ImGui::End();
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
		//RenderEngine::GetInstance()->RescaleFramebuffer(window_width, window_height);
		int fullWindowWidth, fullWindowHeight;
		glfwGetWindowSize(RenderEngine::GetInstance()->GetWindow(), &fullWindowWidth, &fullWindowHeight);
		//glViewport(pos.x, fullWindowHeight -pos.y-window_height, window_width, window_height);
		glViewport(pos.x - 300, pos.y - 100, window_width, window_height);

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

void Editor::DrawShadowMapWindow()
{
	int scale = 2 / 3;
	int subViewportWidth = 1024 * scale;
	int subViewportHeight = 1024 * scale;
	int subViewportX = 100;
	int subViewportY = 100;


	ImGui::Begin("Shadowmap View", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	const ImVec2 windowPos = ImGui::GetWindowPos();
	const ImVec2 windowSize = ImGui::GetWindowSize();

	mouseOverSceneView = IsMouseOverWindow(windowPos, windowSize);

	// Render the sub-viewport content within the ImGui frame
	ImGui::BeginChild("ShadowSubViewport", ImVec2(subViewportWidth, subViewportHeight), true);
	{
		// we access the ImGui window size
		const float window_width = ImGui::GetContentRegionAvail().x;
		const float window_height = ImGui::GetContentRegionAvail().y;

		ImTextureID texId = reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(RenderEngine::GetInstance()->GetDepthMapTexture()));
		ImGui::Image(texId, windowSize,
			ImVec2(0, 1),
			ImVec2(1, 0));

		// we get the screen position of the window
		ImVec2 pos = ImGui::GetCursorScreenPos();

		// we rescale the framebuffer to the actual window size here and reset the glViewport 
		//RenderEngine::GetInstance()->RescaleDepthFramebuffer(window_width, window_height);
		int fullWindowWidth, fullWindowHeight;
		glfwGetWindowSize(RenderEngine::GetInstance()->GetWindow(), &fullWindowWidth, &fullWindowHeight);
		//glViewport(pos.x, fullWindowHeight -pos.y-window_height, window_width, window_height);
		glViewport(pos.x - 300, pos.y - 100, window_width, window_height);

		// and here we can add our created texture as image to ImGui
		// unfortunately we need to use the cast to void* or I didn't find another way tbh
		ImGui::GetWindowDrawList()->AddImage(
			(void*)RenderEngine::GetInstance()->GetDepthMapTexture(),
			ImVec2(pos.x, pos.y),
			ImVec2(pos.x + window_width, pos.y + window_height),
			ImVec2(0, 1),
			ImVec2(1, 0)
		);
	}
	ImGui::EndChild();

	ImGui::End();
}

int Editor::HandleInputText(ImGuiInputTextCallbackData* data)
{
	if (data->EventChar == '\b' && data->CursorPos > 0)
	{
		// Delete the character to the left of the cursor
		std::string& text = *static_cast<std::string*>(data->UserData);
		text.erase(data->CursorPos - 1, 1);
		data->CursorPos--;
		data->BufTextLen--;
		return true;
	}
	/*std::string log = "Introduced character ";
	log = log.append(ImWcharToString(data->EventChar));
	Debug::Log(log);*/

	return false;
}

std::string Editor::ImWcharToString(ImWchar imWchar)
{
	// Convert wide character to narrow character using std::wcrtomb
	char narrowChar[124];
	std::mbstate_t state = std::mbstate_t(); // Initialize conversion state

	std::size_t result = std::wcrtomb(narrowChar, imWchar, &state);

	if (result == static_cast<std::size_t>(-1))
	{
		// Conversion error occurred
		return "";
	}

	// Create std::string from narrow character
	std::string str(narrowChar, result);

	return str;
}