#include "Material.h"

Material::Material()
{
}

Material::Material(VertexShader * vshader, PixelShader * pshader, ID3D11SamplerState * sampler, Texture * textures, unsigned int numTextures )
	: vshader(vshader), pshader(pshader), sampler(sampler)
{
	this->textures = (Texture*)malloc(numTextures * sizeof(Texture));
	memcpy(this->textures, textures, numTextures * sizeof(Texture));
}

Material::~Material()
{
	delete textures;
}
