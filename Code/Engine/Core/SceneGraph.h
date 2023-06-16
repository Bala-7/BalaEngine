#pragma once
#include "SceneNode.h"

class SceneGraph {
public:
	SceneGraph();
	~SceneGraph();
	void Update(float deltaTime);
	void Draw();
	SceneNode* GetRootNode();

private:
	SceneNode* rootNode;
};

