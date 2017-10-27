#include "PixelShader.h"



PixelShader::PixelShader()
	: pshader(nullptr)
{
}

PixelShader::PixelShader(ID3D11PixelShader* shader)
	: pshader(shader)
{
}


PixelShader::~PixelShader()
{
}

ID3D11PixelShader * PixelShader::GetPShader()
{
	return pshader;
}
