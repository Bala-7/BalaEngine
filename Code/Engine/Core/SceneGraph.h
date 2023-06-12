#pragma once
#include "SceneNode.h"

class SceneGraph {
public:
	SceneGraph();
	~SceneGraph();
	void Update(float deltaTime);
	void Draw();
	SceneNode* GetRootNode();

	void DrawEditorWindow();
private:
	SceneNode* rootNode;
};

