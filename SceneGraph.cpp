#include "Code\Engine\Core\SceneGraph.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include "imgui_widgets.h"

#include "Code/Engine/Editor/Editor.h"
#include "Engine/Debug/Debug.h"

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

void SceneGraph::DrawShadows()
{
	rootNode->DrawShadows();
}

SceneNode* SceneGraph::GetRootNode()
{
	return rootNode;
}

