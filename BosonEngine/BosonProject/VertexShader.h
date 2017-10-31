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
	void SetShader(ID3D11DeviceContext* context);
private:
	ID3D11VertexShader* vshader;
	ID3D11InputLayout* inputLayout;
};

