#pragma once
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"

class Material
{
public:
	Material();
	Material(VertexShader* vshader, PixelShader* pshader, ID3D11SamplerState* sampler, Texture* textures, unsigned int numTextures);
	~Material();

	VertexShader* getVShader();
	PixelShader* getPShader();
	ID3D11SamplerState* getSamplerState();

private:
	VertexShader* vshader;
	PixelShader* pshader;
	ID3D11SamplerState* sampler;
	Texture* textures;
};

