#pragma once
#include <DirectXMath.h>

struct Transform
{
	//forward in world space
	DirectX::XMFLOAT3 forward;
	//world position
	DirectX::XMFLOAT3 position;
	//euler angle rotation
	DirectX::XMFLOAT3 rotation;
	//scale in the x, y, z
	DirectX::XMFLOAT3 scale;
	//force acting on this object
	DirectX::XMFLOAT3 force;
	//last force that acted on this object
	DirectX::XMFLOAT3 prevForce;
	//acceleration of this object
	DirectX::XMFLOAT3 acceleration;
	//velocity of this object
	DirectX::XMFLOAT3 velocity;
	float mass;
};