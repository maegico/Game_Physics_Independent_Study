#include "Material.h"

Material::Material()
{
}

Material::Material(VertexShader * vshader, PixelShader * pshader, ID3D11SamplerState * sampler, Texture * textures, unsigned int numTextures )
	: vshader(vshader), pshader(pshader), sampler(sampler)
{
	if (numTextures > 1)
	{
		this->textures = (Texture*)malloc(numTextures * sizeof(Texture));
		memcpy(this->textures, textures, numTextures * sizeof(Texture));
	}
}

Material::~Material()
{
	delete textures;
}

VertexShader * Material::getVShader()
{
	return vshader;
}

PixelShader * Material::getPShader()
{
	return pshader;
}

ID3D11SamplerState * Material::getSamplerState()
{
	return sampler;
}
