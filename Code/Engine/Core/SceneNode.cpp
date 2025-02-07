#include "SceneNode.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <Engine/Rendering/RenderEngine.h>
//#include "imgui_widgets.h"

SceneNode::SceneNode()
{
	gameObject = nullptr;
}

SceneNode::SceneNode(GameObject* go)
{
	gameObject = go;
	gameObject->SetSceneNode(this);
}

SceneNode::~SceneNode()
{
	for (SceneNode* child : children) {
		delete child;
	}
	children.clear();
}

void SceneNode::AddChild(SceneNode* child)
{
	children.push_back(child);
	child->SetParent(this);
	child->SetSceneGraph(scene);
}

void SceneNode::RemoveChild(SceneNode* child)
{
	auto it = std::find(children.begin(), children.end(), child);
	if (it != children.end()) 
	{
		(*it)->RemoveParent();
		children.erase(it);
	}
}

void SceneNode::Update(float deltaTime)
{
	if (gameObject) {
		gameObject->Update();
	}
	for (SceneNode* child : children) {
		child->Update(deltaTime);
	}
}

void SceneNode::Draw(Camera* camera)
{
	if (gameObject && gameObject->GetRenderComponent()) 
	{
		gameObject->GetRenderComponent()->draw(camera);
	}
	for (SceneNode* child : children) {
		child->Draw(camera);
	}
}

void SceneNode::DrawOutline(Camera* camera)
{
	if (gameObject && gameObject->GetRenderComponent())
	{
		gameObject->GetRenderComponent()->drawOutline(camera);
	}
	for (SceneNode* child : children) {
		child->DrawOutline(camera);
	}
}

void SceneNode::DrawShadows()
{
	if (gameObject && gameObject->GetRenderComponent())
	{
		gameObject->GetRenderComponent()->DrawShadows();
	}
	for (SceneNode* child : children) {
		child->DrawShadows();
	}
}

void SceneNode::DrawCubemapShadows()
{
	if (gameObject && gameObject->GetRenderComponent())
	{
		gameObject->GetRenderComponent()->DrawCubemapShadows();
	}
	for (SceneNode* child : children) {
		child->DrawCubemapShadows();
	}
}

void SceneNode::DrawPicking()
{
	if (gameObject && gameObject->GetRenderComponent())
	{
		RenderEngine::GetInstance()->AddToRenderedItems(gameObject);
		gameObject->GetRenderComponent()->DrawPickingColor(RenderEngine::GetInstance()->GetCurrentObjectPickingIndex());
		RenderEngine::GetInstance()->IncrementObjectPickingIndex();
	}
	for (SceneNode* child : children) {
		child->DrawPicking();
	}
}

void SceneNode::DrawEditorWindow(int level)
{
	if(level == 0)
		ImGui::Text("Scene");
	else 
	{
		ImGui::Indent(10.0f);
		ImGui::Text(gameObject->name.c_str());
	}
	
	int nextLevel = level+1;
	for (SceneNode* child : children) {
		child->DrawEditorWindow(nextLevel);
		ImGui::Unindent(10.0f);
	}
}

void SceneNode::GetChildNamesForEditor(int depth, std::vector<std::pair<SceneNode*, int>>& result)
{
	// Add current node to the result list
	result.emplace_back(this, depth);
	
	// Recursively iterate over children
	for (SceneNode* child : children) 
	{
		child->GetChildNamesForEditor(depth + 1, result);
	}
}

void SceneNode::SetParent(SceneNode* newParent)
{
	parent = newParent;
}

void SceneNode::SetSceneGraph(SceneGraph* newScene)
{
	scene = newScene;
}

void SceneNode::RemoveParent()
{
	parent = nullptr;
}

void SceneNode::Destroy()
{
	if(children.size() > 0)
	{	
		for (SceneNode* child : children) 
		{
			child->Destroy();
		}
	}
	children.clear();
	parent->RemoveChild(this);
	RemoveParent();
	delete this;
}

GameObject* SceneNode::GetGameObject()
{
	return gameObject;
}
