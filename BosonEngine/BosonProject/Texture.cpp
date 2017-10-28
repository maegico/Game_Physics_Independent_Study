#include "Texture.h"



Texture::Texture()
	: texture(nullptr), texType(TextureType::Texture2D)
{
}

Texture::Texture(ID3D11ShaderResourceView * texture, TextureType texType)
	: texture(texture), texType(texType)
{
}

Texture::~Texture()
{
}

ID3D11ShaderResourceView * Texture::getTexture()
{
	return texture;
}

TextureType Texture::getTextureType()
{
	return texType;
}
