#pragma once
#include <d3d11.h>

enum TextureType
{
	Texture2D,
	NormalMap,
	CubeMap
};

class Texture
{
public:
	Texture();
	Texture(ID3D11ShaderResourceView* texture, TextureType texType);
	~Texture();

	ID3D11ShaderResourceView* getTexture();
	TextureType getTextureType();

private:
	ID3D11ShaderResourceView* texture;
	TextureType texType;
};

