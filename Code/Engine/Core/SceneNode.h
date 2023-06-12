#pragma once
#include "GameObject.h"
#include <vector>

class SceneNode
{
public:
	SceneNode();
	SceneNode(GameObject* go);
	~SceneNode();

	void AddChild(SceneNode* child);
	void RemoveChild(SceneNode* child);
	void Update(float deltaTime);
	void Draw();
	void DrawEditorWindow(int level);

	void GetChildNamesForEditor(int depth, std::vector<std::pair<GameObject*, int>>& result);

private:
	std::vector<SceneNode*> children;
	GameObject* gameObject;

	

};

