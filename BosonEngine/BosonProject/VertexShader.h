#pragma once
#include <d3d11.h>

class VertexShader
{
public:
	VertexShader();
	VertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* layout);
	~VertexShader();

	ID3D11VertexShader* GetVShader();
	ID3D11InputLayout* GetInputLayout();
private:
	ID3D11VertexShader* vshader;
	ID3D11InputLayout* inputLayout;
};

