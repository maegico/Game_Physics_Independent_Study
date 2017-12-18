#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Collider.h"

class EntityInterface
{
public:
	EntityInterface(Mesh* mesh, Material* mat, ColliderType colType, ColliderMesh cmesh)
		: mesh(mesh), mat(mat), collider(colType, cmesh, transform)
	{
		transform = { DirectX::XMFLOAT3(0,0,1), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0),
			DirectX::XMFLOAT3(1,1,1), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0), DirectX::XMFLOAT3(0,0,0), 1 };
	}

	~EntityInterface() { delete mesh; }

	virtual void Init() = 0;

	virtual void Update(float deltaTime)
	{
		DirectX::XMFLOAT3 massDivisor = DirectX::XMFLOAT3(transform.mass, transform.mass, transform.mass);
		DirectX::XMFLOAT3 dtMultiplier = DirectX::XMFLOAT3(deltaTime, deltaTime, deltaTime);
		DirectX::XMFLOAT3 frictionMultiplier = DirectX::XMFLOAT3(0.99f, 0.99f, 0.99f);

		DirectX::XMVECTOR force = DirectX::XMLoadFloat3(&transform.force);
		DirectX::XMVECTOR acceleration = DirectX::XMLoadFloat3(&transform.acceleration);
		DirectX::XMVECTOR velocity = DirectX::XMLoadFloat3(&transform.velocity);
		DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&transform.position);
		DirectX::XMVECTOR mass = DirectX::XMLoadFloat3(&massDivisor);
		DirectX::XMVECTOR dt = DirectX::XMLoadFloat3(&dtMultiplier);
		DirectX::XMVECTOR friction = DirectX::XMLoadFloat3(&frictionMultiplier);

		float speed = 0;

		//acceleration = force/mass
		//velocity += acceleration * dt;
		//check if 
		//position += velocity * dt;
		acceleration = DirectX::XMVectorDivide(force, mass);
		velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVectorMultiply(acceleration, dt));
		velocity = DirectX::XMVectorMultiply(velocity, friction);
		DirectX::XMStoreFloat(&speed, DirectX::XMVector3Length(velocity));
		if (speed > 0.001)
		{
			position = DirectX::XMVectorAdd(position, DirectX::XMVectorMultiply(velocity, dt));
			DirectX::XMStoreFloat3(&transform.position, position);
			DirectX::XMStoreFloat3(&transform.velocity, velocity);
			DirectX::XMStoreFloat3(&transform.acceleration, acceleration);
		}
		else
		{
			transform.velocity = DirectX::XMFLOAT3(0, 0, 0);
			transform.acceleration = DirectX::XMFLOAT3(0, 0, 0);
		}
		transform.prevForce = transform.force;
		transform.force = DirectX::XMFLOAT3(0, 0, 0);
	}

	virtual void Draw() = 0;

	Mesh* GetMesh() { return mesh; }
	Material* GetMaterial() { return mat; }
	Transform transform;
	Collider collider;
private:
	Mesh* mesh;
	Material* mat;
};

