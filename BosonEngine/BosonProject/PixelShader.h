#pragma once
#include <d3d11.h>

class PixelShader
{
public:
	PixelShader();
	PixelShader(ID3D11PixelShader* shader);
	~PixelShader();

	ID3D11PixelShader* GetPShader();

private:
	ID3D11PixelShader* pshader;
};

