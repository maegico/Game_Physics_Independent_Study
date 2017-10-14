#pragma once
#include "EntityInterface.h"
class BasicEntity:EntityInterface
{
public:
	BasicEntity(Mesh* m);
	~BasicEntity();
	
	void Init();
	void Update();
	void Draw();
};

