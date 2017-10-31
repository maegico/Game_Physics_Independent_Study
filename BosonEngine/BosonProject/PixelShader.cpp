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

void PixelShader::SetPixelShader(ID3D11DeviceContext * context)
{
	context->PSSetShader(pshader, 0, 0);
}

ID3D11PixelShader * PixelShader::GetPShader()
{
	return pshader;
}
