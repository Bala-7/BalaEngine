#include "SceneNode.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include "imgui_widgets.h"

SceneNode::SceneNode()
{
	gameObject = nullptr;
}

SceneNode::SceneNode(GameObject* go)
{
	gameObject = go;
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

void SceneNode::Draw()
{
	if (gameObject && gameObject->GetRenderComponent()) {
		gameObject->GetRenderComponent()->draw();
	}
	for (SceneNode* child : children) {
		child->Draw();
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
	for (SceneNode* child : children) {
		child->GetChildNamesForEditor(depth + 1, result);
	}
}

void SceneNode::SetParent(SceneNode* newParent)
{
	parent = newParent;
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
