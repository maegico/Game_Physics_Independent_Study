#include "VertexShader.h"

VertexShader::VertexShader()
	: vshader(nullptr), inputLayout(nullptr)
{
}

VertexShader::VertexShader(ID3D11VertexShader * shader, ID3D11InputLayout * layout)
	: vshader(shader), inputLayout(layout)
{
}

VertexShader::~VertexShader()
{
}

ID3D11VertexShader * VertexShader::GetVShader()
{
	return vshader;
}

ID3D11InputLayout * VertexShader::GetInputLayout()
{
	return inputLayout;
}
