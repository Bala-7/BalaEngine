#include "Code\Engine\Core\SceneGraph.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include "imgui_widgets.h"

#include "Code/Engine/Editor/Editor.h"

SceneGraph::SceneGraph()
{
	rootNode = new SceneNode();
}

SceneGraph::~SceneGraph()
{
	delete rootNode;
}

void SceneGraph::Update(float deltaTime)
{
	rootNode->Update(deltaTime);
}

void SceneGraph::Draw()
{
	rootNode->Draw();
}

SceneNode* SceneGraph::GetRootNode()
{
	return rootNode;
}

void SceneGraph::DrawEditorWindow()
{
	ImGui::Begin("Scene Graph");
	
	std::vector<std::pair<GameObject*, int>> itemsVector;
	rootNode->GetChildNamesForEditor(0, itemsVector);
	static int selectedItemIndex = -1;
	int previousDepth = 0;
	for (size_t i = 0; i < itemsVector.size(); ++i)
	{
		const std::pair<GameObject*, int>& pair = itemsVector[i];
		if (pair.first) 
		{
			if (pair.second > previousDepth)
				ImGui::Indent(12);
			else if (pair.second < previousDepth)
				ImGui::Unindent(12);

			previousDepth = pair.second;
			if (ImGui::Selectable(pair.first->name.c_str(), selectedItemIndex == i))
			{
				Editor::Instance()->SetDisplayedGameObject(pair.first);
				selectedItemIndex = i;
			}
		}
	}

	ImGui::Separator();
	//rootNode->DrawEditorWindow(0);
	ImGui::End();
}
