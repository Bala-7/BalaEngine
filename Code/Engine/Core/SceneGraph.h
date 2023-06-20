#pragma once
#include "SceneNode.h"

class SceneGraph {
public:
	SceneGraph();
	~SceneGraph();
	void Update(float deltaTime);
	void Draw();
	void DrawShadows();
	SceneNode* GetRootNode();

private:
	SceneNode* rootNode;
};

