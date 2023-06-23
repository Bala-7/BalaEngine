#pragma once
#include "SceneNode.h"
#include "Engine/Rendering/Light.h"
#include <vector>

class SceneGraph {
public:
	SceneGraph();
	~SceneGraph();
	void Update(float deltaTime);
	void Draw();
	void DrawShadows();
	SceneNode* GetRootNode();

	void AddToSceneLights(Light* newLight);
	std::vector<Light*> GetSceneLights() { return sceneLights; }
private:
	SceneNode* rootNode;
	std::vector<Light*> sceneLights;


	int CountDirectionalLights();
};

