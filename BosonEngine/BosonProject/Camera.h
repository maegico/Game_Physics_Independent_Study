#pragma once
#include <DirectXMath.h>
#include <Windows.h>

class Camera
{
public:
	DirectX::XMFLOAT4X4 getViewMatrix();
	DirectX::XMFLOAT4X4 getProjectionMatrix();
	void addRotX(float rotX);
	void addRotY(float rotY);

	Camera();
	~Camera();

	void update(float deltaTime);
	void updateProjection(unsigned int width, unsigned int height);

private:
	float rotX = 0.0f;
	float rotY = 0.0f;
	
	DirectX::XMFLOAT3 position = DirectX::XMFLOAT3(0, 0, -5);
	DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0, 0, 1);
	DirectX::XMFLOAT3 up = DirectX::XMFLOAT3(0, 1, 0);
	DirectX::XMFLOAT3 right = DirectX::XMFLOAT3(1, 0, 0);

	DirectX::XMFLOAT3 unitX = DirectX::XMFLOAT3(1, 0, 0);
	DirectX::XMFLOAT3 unitY = DirectX::XMFLOAT3(0, 1, 0);
	DirectX::XMFLOAT3 unitZ = DirectX::XMFLOAT3(0, 0, 1);

	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

