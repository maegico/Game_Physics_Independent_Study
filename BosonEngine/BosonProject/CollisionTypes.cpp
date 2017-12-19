#include "CollisionTypes.h"

DirectX::XMFLOAT3 ColliderMeshFunctions::halfpoint(DirectX::XMFLOAT3 vec1, DirectX::XMFLOAT3 vec2)
{
	//see if you can load into xmvectors, subtract them, and then do a scalar multiplication to 0.5
	return DirectX::XMFLOAT3((vec2.x - vec1.x) / 2, (vec2.y - vec1.y) / 2, (vec2.z - vec1.z) / 2);
}

ColliderMesh ColliderMeshFunctions::computeColliderMesh(MeshType meshType, std::vector<Vertex>& vertArray)
{
	if (meshType < 0 || meshType > 3) return ColliderMesh();

	DirectX::XMFLOAT3 temp;
	int index1 = 0;
	int index2 = 0;
	float mag = -1.0f;
	bool notFitted = true;
	int count = 0;
	DirectX::XMFLOAT3 min = vertArray[0].Position;
	DirectX::XMFLOAT3 max = vertArray[0].Position;
	ColliderMesh cmesh = ColliderMesh();
	cmesh.mType = meshType;
	cmesh.vec1 = min;
	cmesh.vec2 = max;

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
		//for SAT, store center, axes, and halfwidths

		for (int i = 0; i < vertArray.size(); i++)
		{
			DirectX::XMFLOAT3 pos = vertArray[i].Position;
			if (pos.x < min.x)
				min.x = pos.x;
			else if (pos.x > max.x)
				max.x = pos.x;

			if (pos.y < min.y)
				min.y = pos.y;
			else if (pos.y > max.y)
				max.y = pos.y;

			if (pos.y < min.z)
				min.z = pos.z;
			else if (pos.z > max.z)
				max.z = pos.z;
		}

		cmesh.vec2 = halfpoint(min, max);
		cmesh.vec1 = cmesh.vec2;
		cmesh.axes[0] = DirectX::XMFLOAT3(1, 0, 0);
		cmesh.axes[1] = DirectX::XMFLOAT3(0, 1, 0);
		cmesh.axes[2] = DirectX::XMFLOAT3(0, 0, 1);
		break;
	case MeshType::Sphere:
	case MeshType::SBSphere:
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

		DirectX::XMVECTOR centerTemp = v1 + DirectX::XMVector3Normalize(v2 - v1) * cmesh.vec2.x;

		
		while (notFitted)
		{
			for (int i = 0; i < vertArray.size(); i++)
			{
				float tempDistance;

				DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&vertArray[i].Position);
				DirectX::XMVECTOR tempLength = DirectX::XMVector3Length(centerTemp - vec);
				DirectX::XMStoreFloat(&tempDistance, tempLength);
				if (tempDistance > cmesh.vec2.x)
				{
					float change = (tempDistance - cmesh.vec2.x) / 2.0f;
					cmesh.vec2.x += change;

					centerTemp += DirectX::XMVector3Normalize(vec - centerTemp) * change;
					break;
				}
				XMStoreFloat3(&cmesh.vec1, centerTemp);

				if (i + 1 == vertArray.size())
					notFitted = false;

				count++;
				printf("Number of fit iterations: %d", count);
			}
		}
		break;
	default:
		break;
	}
	return cmesh;
}