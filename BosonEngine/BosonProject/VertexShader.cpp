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

void VertexShader::SetShader(ID3D11DeviceContext * context)
{
	context->IASetInputLayout(inputLayout);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(vshader, 0, 0);
}
