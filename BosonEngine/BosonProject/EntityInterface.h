#pragma once
#include "Mesh.h"
class EntityInterface
{
public:
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	
	Mesh* mesh;
	//Material* mat;
};

