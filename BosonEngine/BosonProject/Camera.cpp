#include "Camera.h"

using namespace DirectX;

DirectX::XMFLOAT4X4 Camera::getViewMatrix()
{
	return viewMatrix;
}

DirectX::XMFLOAT4X4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::addRotX(float rotX)
{
	this->rotX += rotX/500;
}

void Camera::addRotY(float rotY)
{
	this->rotY += rotY/500;
}

Camera::Camera()
{
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 0, -5, 1);
	DirectX::XMVECTOR dir = DirectX::XMVectorSet(0, 0, 1, 1);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 1);
	DirectX::XMMATRIX V = DirectX::XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixTranspose(V)); // Transpose for HLSL!
}


Camera::~Camera()
{
}

void Camera::update(float deltaTime)
{
	DirectX::XMVECTOR uX = DirectX::XMLoadFloat3(&unitX);
	DirectX::XMVECTOR uY = DirectX::XMLoadFloat3(&unitY);
	DirectX::XMVECTOR uZ = DirectX::XMLoadFloat3(&unitZ);

	DirectX::XMVECTOR updatedPos = DirectX::XMLoadFloat3(&position);

	DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationRollPitchYaw(rotX, rotY, 0);

	DirectX::XMVECTOR updatedForward = DirectX::XMVector3Rotate(uZ, quat);
	DirectX::XMVECTOR updatedRight = DirectX::XMVector3Rotate(uX, quat);
	DirectX::XMVECTOR updatedUp = DirectX::XMVector3Rotate(uY, quat);
	
	if (GetAsyncKeyState('W') & 0x8000) { updatedPos += updatedForward * .25f * deltaTime; }
	if (GetAsyncKeyState('S') & 0x8000) { updatedPos -= updatedForward * .25f * deltaTime; }
	if (GetAsyncKeyState('A') & 0x8000) { updatedPos -= updatedRight * .25f * deltaTime; }
	if (GetAsyncKeyState('D') & 0x8000) { updatedPos += updatedRight * .25f * deltaTime; }

	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(updatedPos, updatedForward, uY);

	DirectX::XMStoreFloat3(&forward, updatedForward);
	DirectX::XMStoreFloat3(&right, updatedRight);
	DirectX::XMStoreFloat3(&up, updatedUp);
	DirectX::XMStoreFloat3(&position, updatedPos);
	DirectX::XMStoreFloat4x4(&viewMatrix, DirectX::XMMatrixTranspose(view));
}

void Camera::updateProjection(unsigned int width, unsigned int height)
{
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixTranspose(P)); // Transpose for HLSL!
}
