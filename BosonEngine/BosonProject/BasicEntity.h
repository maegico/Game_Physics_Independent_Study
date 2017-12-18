#pragma once
#include "EntityInterface.h"

class BasicEntity:EntityInterface
{
public:
	BasicEntity(Mesh* m, ColliderMesh cmesh, Material* mat);
	~BasicEntity();
	
	void Init();
	void Update(float deltaTime);
	void Draw();
};

