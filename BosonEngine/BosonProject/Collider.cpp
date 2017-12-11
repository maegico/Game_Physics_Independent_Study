#include "Collider.h"

using namespace DirectX;

Collider::Collider(ColliderType colliderType, MeshType meshType,
	Transform& transform, void(*onCollisionFunc)(), Vertex* vertArray)
	: inUse(false), colType(colliderType), mType(meshType), transform(transform),
	  onCollision(onCollisionFunc)
{
}


Collider::~Collider()
{
}

ColliderType Collider::getColliderType()
{
	return colType;
}

MeshType Collider::getMeshType()
{
	return mType;
}

void Collider::setID(int id)
{
	this->id = id;
}

int Collider::getID()
{
	return id;
}

ColliderMesh Collider::getMesh()
{
	return mesh;
}

bool Collider::computeColliderMesh(MeshType meshType, std::vector<Vertex>& vertArray)
{
	ColliderMesh cmesh = {};
	/*AABBMesh abox = {};
	OBBMesh obox = {};
	SphereMesh sphere = {};*/
	DirectX::XMFLOAT3 temp;
	int index1 = 0;
	int index2 = 0.0f;
	float mag = -1.0f;
	bool notFitted = true;

	switch (meshType)
	{
	case MeshType::AABB:
		
		for (int i = 0; i < vertArray.size(); i++)
		{
			DirectX::XMVECTOR altered = DirectX::XMLoadFloat3(&vertArray[i].Position);
			//perform the necessary calculations, rotate, scale, then calculate the box with that
			XMStoreFloat3(&temp, altered);
			if (temp.x < cmesh.vec1.x)
				cmesh.vec1.x = temp.x;
			else if (temp.x > cmesh.vec2.x)
				cmesh.vec2.x = temp.x;

			if (temp.y < cmesh.vec1.y)
				cmesh.vec1.y = temp.y;
			else if (temp.y > cmesh.vec2.y)
				cmesh.vec2.y = temp.y;

			if (temp.y < cmesh.vec1.y)
				cmesh.vec1.y = temp.y;
			else if (temp.y > cmesh.vec2.y)
				cmesh.vec2.y = temp.y;
		}
		break;
	case MeshType::OBB:
		
		for (int i = 0; i < vertArray.size(); i++)
		{
			DirectX::XMFLOAT3 pos = vertArray[i].Position;
			if (pos.x < cmesh.vec1.x)
				cmesh.vec1.x = pos.x;
			else if (pos.x > cmesh.vec2.x)
				cmesh.vec2.x = pos.x;

			if (pos.y < cmesh.vec1.y)
				cmesh.vec1.y = pos.y;
			else if (pos.y > cmesh.vec2.y)
				cmesh.vec2.y = pos.y;

			if (pos.y < cmesh.vec1.z)
				cmesh.vec1.z = pos.z;
			else if (pos.z > cmesh.vec2.z)
				cmesh.vec2.z = pos.z;
		}
		break;
	case MeshType::Sphere:
		for (int i = 0; i < vertArray.size(); i++)
		{
			//XMVector3Length
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&vertArray[0].Position);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&vertArray[i].Position);

			DirectX::XMVECTOR length = DirectX::XMVector3Length(v1 - v2);
			float temp;
			DirectX::XMStoreFloat(&temp, length);
			if (mag < temp)
			{
				index1 = i;
				mag = temp;
			}

			float dist;
		}

		for (int i = 0; i < vertArray.size(); i++)
		{
			//XMVector3Length
			DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&vertArray[index1].Position);
			DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&vertArray[i].Position);
			DirectX::XMVECTOR length = DirectX::XMVector3Length(v1 - v2);
			float temp;
			DirectX::XMStoreFloat(&temp, length);
			if (mag < temp)
			{
				index2 = i;
				mag = temp;
			}
		}

		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&vertArray[index1].Position);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&vertArray[index2].Position);
		DirectX::XMVECTOR length = DirectX::XMVector3Length(v1 - v2);

		DirectX::XMStoreFloat(&cmesh.vec2.x, length);
		cmesh.vec2.x /= 2;

		DirectX::XMVECTOR centerTemp = v1 + DirectX::XMVector3Normalize(v2-v1) * cmesh.vec2.x;
		
		while (notFitted)
		{
			DirectX::XMVECTOR tempCenter = DirectX::XMLoadFloat3(&cmesh.vec1);
			for (int i = 0; i < vertArray.size(); i++)
			{
				float tempDistance;
				
				DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&vertArray[i].Position);
				DirectX::XMVECTOR tempLength = DirectX::XMVector3Length(tempCenter - vec);
				DirectX::XMStoreFloat(&tempDistance, tempLength);
				if (tempDistance > cmesh.vec2.x)
				{
					XMFLOAT3 tempAdd;
					float change = (tempDistance - cmesh.vec2.x) / 2.0f;
					cmesh.vec2.x += change;

					tempCenter += DirectX::XMVector3Normalize(vec - tempCenter) * change;
					break;
				}
				XMStoreFloat3(&cmesh.vec1, tempCenter);

				if (i + 1 == vertArray.size())
					notFitted = false;
			}
		}
		break;
	default:
		break;
	}
	return false;
}
