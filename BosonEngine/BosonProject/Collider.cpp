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
	AABBMesh abox = {};
	OBBMesh obox = {};
	SphereMesh sphere = {};
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
			if (temp.x < obox.min.x)
				obox.min.x = temp.x;
			else if (temp.x > obox.max.x)
				obox.max.x = temp.x;

			if (temp.y < obox.min.y)
				obox.min.y = temp.y;
			else if (temp.y > obox.max.y)
				obox.max.y = temp.y;

			if (temp.y < obox.min.y)
				obox.min.y = temp.y;
			else if (temp.y > obox.max.y)
				obox.max.y = temp.y;
		}
		break;
	case MeshType::OBB:
		
		for (int i = 0; i < vertArray.size(); i++)
		{
			DirectX::XMFLOAT3 pos = vertArray[i].Position;
			if (pos.x < obox.min.x)
				obox.min.x = pos.x;
			else if (pos.x > obox.max.x)
				obox.max.x = pos.x;

			if (pos.y < obox.min.y)
				obox.min.y = pos.y;
			else if (pos.y > obox.max.y)
				obox.max.y = pos.y;

			if (pos.y < obox.min.y)
				obox.min.y = pos.y;
			else if (pos.y > obox.max.y)
				obox.max.y = pos.y;
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

		DirectX::XMStoreFloat(&sphere.radius, length);
		sphere.radius /= 2;

		DirectX::XMVECTOR centerTemp = v1 + DirectX::XMVector3Normalize(v2-v1) * sphere.radius;
		
		while (notFitted)
		{
			DirectX::XMVECTOR tempCenter = DirectX::XMLoadFloat3(&sphere.center);
			for (int i = 0; i < vertArray.size(); i++)
			{
				float tempDistance;
				
				DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&vertArray[i].Position);
				DirectX::XMVECTOR tempLength = DirectX::XMVector3Length(tempCenter - vec);
				DirectX::XMStoreFloat(&tempDistance, tempLength);
				if (tempDistance > sphere.radius)
				{
					XMFLOAT3 tempAdd;
					float change = (tempDistance - sphere.radius) / 2.0f;
					sphere.radius += change;

					tempCenter += DirectX::XMVector3Normalize(vec - tempCenter) * change;
					break;
				}
				XMStoreFloat3(&sphere.center, tempCenter);

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
