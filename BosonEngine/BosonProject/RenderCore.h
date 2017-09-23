#pragma once
#include <d3d11.h>

class RenderCore
{
public:
	RenderCore();
	~RenderCore();

private:
	D3D_FEATURE_LEVEL featureLvl;
};

