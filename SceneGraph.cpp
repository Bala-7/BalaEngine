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
	rootNode->SetSceneGraph(this);
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

void SceneGraph::AddToSceneLights(Light* newLight)
{
	sceneLights.push_back(newLight);
}

int SceneGraph::CountDirectionalLights()
{
	int directionalLightCount = 0;
	for (auto i = sceneLights.begin(); i != sceneLights.end(); ++i)
	{
		if ((*i)->GetType() == Light::LightType::Directional)
			directionalLightCount++;
	}

	return directionalLightCount;
}

