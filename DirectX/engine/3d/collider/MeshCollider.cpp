#include "MeshCollider.h"
#include "Collision.h"
#include <bitset>
#include "Matrix4.h"

using namespace DirectX;

const bool operator==(const XMINT2& a, const XMINT2& b) {
	bool x = a.x == b.x;
	bool y = a.y == b.y;

	return x * y;
}

const bool operator!=(const XMINT2& a, const XMINT2& b) {
	bool x = a.x == b.x;
	bool y = a.y == b.y;

	return !x || !y;
}

void MeshCollider::MinMax(Model* _model)
{
	const std::vector<Mesh*>& meshes = _model->GetMeshes();
	std::vector<Mesh*>::const_iterator it = meshes.cbegin();

	//[0]最大[1]最小
	XMFLOAT3 minmax[2] = { {0,0,0},{100,100,100} };

	for (; it != meshes.cend(); ++it) {
		Mesh* mesh = *it;
		const std::vector<Mesh::VERTEX>& vertices = mesh->GetVertices();
		const int size = static_cast<int>(vertices.size());

		for (int i = 0; i < size; i++)
		{
			if (vertices[i].pos.x > minmax[0].x)
			{
				minmax[0].x = vertices[i].pos.x;
			} else if (vertices[i].pos.x < minmax[1].x)
			{
				minmax[1].x = vertices[i].pos.x;
			}

			if (vertices[i].pos.y > minmax[0].y)
			{
				minmax[0].y = vertices[i].pos.y;
			} else if (vertices[i].pos.y < minmax[1].y)
			{
				minmax[1].y = vertices[i].pos.y;
			}

			if (vertices[i].pos.z > minmax[0].z)
			{
				minmax[0].z = vertices[i].pos.z;
			} else if (vertices[i].pos.z < minmax[1].z)
			{
				minmax[1].z = vertices[i].pos.z;
			}
		}
	}

	min = { minmax[1].x,minmax[1].y,minmax[1].z };
	max = { minmax[0].x,minmax[0].y,minmax[0].z };
}

void MeshCollider::MinMax(const std::vector<Mesh::VERTEX>& _vertices)
{
	//[0]最大[1]最小
	XMFLOAT3 minmax[2] = { {0,0,0},{100,100,100} };

	const std::vector<Mesh::VERTEX> vertices = _vertices;
	const int size = static_cast<int>(vertices.size());

	for (int i = 0; i < size; i++)
	{
		if (vertices[i].pos.x > minmax[0].x)
		{
			minmax[0].x = vertices[i].pos.x;
		} else if (vertices[i].pos.x < minmax[1].x)
		{
			minmax[1].x = vertices[i].pos.x;
		}

		if (vertices[i].pos.y > minmax[0].y)
		{
			minmax[0].y = vertices[i].pos.y;
		} else if (vertices[i].pos.y < minmax[1].y)
		{
			minmax[1].y = vertices[i].pos.y;
		}

		if (vertices[i].pos.z > minmax[0].z)
		{
			minmax[0].z = vertices[i].pos.z;
		} else if (vertices[i].pos.z < minmax[1].z)
		{
			minmax[1].z = vertices[i].pos.z;
		}
	}

	min = { minmax[1].x,minmax[1].y,minmax[1].z };
	max = { minmax[0].x,minmax[0].y,minmax[0].z };
}

XMINT2 MeshCollider::OctreeSet(const XMFLOAT3& _pos)
{
	XMINT2 octtreenum = {};
	for (int i = 0; i < 8; i++)
	{
		if (octtreeRange[i].x <= _pos.x && _pos.x < octtreeRange[i + 1].x)
		{
			octtreenum.x = i;
			break;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (octtreeRange[i].z <= _pos.z && _pos.z < octtreeRange[i + 1].z)
		{
			octtreenum.y = i;
			break;
		}
	}

	return octtreenum;
}

void MeshCollider::ConstructTriangles(Model* _model)
{
	if (!isInit)
	{
		object = std::make_unique<PrimitiveObject3D>();
		isInit = true;
	}

	for (int i = 0; i < octreeSplit; i++)
	{
		for (int j = 0; j < octreeSplit; j++)
		{
			triangle[i][j].clear();
		}
	}

	const std::vector<Mesh*>& meshes = _model->GetMeshes();

	int start = 0;

	MinMax(_model);

	XMFLOAT3 minmaxRange = {
		max.m128_f32[0] - min.m128_f32[0],
		max.m128_f32[1] - min.m128_f32[1],
		max.m128_f32[2] - min.m128_f32[2]
	};

	for (int i = 0; i < 9; i++)
	{
		octtreeRange[i].x = minmaxRange.x / 8 * i;
		octtreeRange[i].y = minmaxRange.y / 8 * i;
		octtreeRange[i].z = minmaxRange.z / 8 * i;
	}

	octtreeRange[8].x++;
	octtreeRange[8].y++;
	octtreeRange[8].z++;

	std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	for (; it != meshes.cend(); ++it) {
		Mesh* mesh = *it;
		const std::vector<Mesh::VERTEX>& vertices = mesh->GetVertices();
		const std::vector<unsigned long>& indices = mesh->GetIndices();

		size_t triangleNum = indices.size() / 3;

		for (int i = 0; i < triangleNum; i++)
		{
			Triangle addTriangle;
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			addTriangle.p0 = {
				vertices[idx0].pos.x,
				vertices[idx0].pos.y,
				vertices[idx0].pos.z,
				1 };
			addTriangle.p1 = {
				vertices[idx1].pos.x,
				vertices[idx1].pos.y,
				vertices[idx1].pos.z,
				1 };

			addTriangle.p2 = {
				vertices[idx2].pos.x,
				vertices[idx2].pos.y,
				vertices[idx2].pos.z,
				1 };

			addTriangle.ComputeNormal();

			XMINT2 Octree1 = OctreeSet(vertices[idx0].pos);
			XMINT2 Octree2 = OctreeSet(vertices[idx1].pos);
			XMINT2 Octree3 = OctreeSet(vertices[idx2].pos);

			triangle[Octree1.x][Octree1.y].emplace_back(addTriangle);
			if (Octree1 != Octree2)
			{
				triangle[Octree2.x][Octree2.y].emplace_back(addTriangle);
			}
			if (Octree1 != Octree3 && Octree2 != Octree3)
			{
				triangle[Octree3.x][Octree3.y].emplace_back(addTriangle);
			}

			object->SetVertex(vertices[idx0].pos);
			object->SetVertex(vertices[idx1].pos);
			object->SetVertex(vertices[idx2].pos);
		}
	}
}

void MeshCollider::ConstructTriangles(const std::vector<Mesh::VERTEX>& _vertices, const std::vector<unsigned long>& _indices)
{
	if (!isInit)
	{
		object = std::make_unique<PrimitiveObject3D>();
		isInit = true;
	}

	for (int i = 0; i < octreeSplit; i++)
	{
		for (int j = 0; j < octreeSplit; j++)
		{
			triangle[i][j].clear();
		}
	}

	int start = 0;

	MinMax(_vertices);

	XMFLOAT3 minmaxRange = {
		max.m128_f32[0] - min.m128_f32[0],
		max.m128_f32[1] - min.m128_f32[1],
		max.m128_f32[2] - min.m128_f32[2]
	};
	for (int i = 0; i < 9; i++)
	{
		octtreeRange[i].x = minmaxRange.x / 8 * i;
		octtreeRange[i].y = minmaxRange.y / 8 * i;
		octtreeRange[i].z = minmaxRange.z / 8 * i;
	}
	octtreeRange[8].x++;
	octtreeRange[8].y++;
	octtreeRange[8].z++;

	const std::vector<Mesh::VERTEX> vertices = _vertices;
	const std::vector<unsigned long> indices = _indices;

	size_t triangleNum = indices.size() / 3;

	XMINT2 aa = { 0,1 };
	XMINT2 bb = { 0,1 };
	bool aaa=aa!=bb;

	for (int i = 0; i < triangleNum; i++)
	{
		Triangle addTriangle;
		int idx0 = indices[i * 3 + 0];
		int idx1 = indices[i * 3 + 1];
		int idx2 = indices[i * 3 + 2];

		addTriangle.p0 = {
			vertices[idx0].pos.x,
			vertices[idx0].pos.y,
			vertices[idx0].pos.z,
			1 };
		addTriangle.p1 = {
			vertices[idx1].pos.x,
			vertices[idx1].pos.y,
			vertices[idx1].pos.z,
			1 };

		addTriangle.p2 = {
			vertices[idx2].pos.x,
			vertices[idx2].pos.y,
			vertices[idx2].pos.z,
			1 };

		addTriangle.ComputeNormal();

		XMINT2 Octree1 = OctreeSet(vertices[idx0].pos);
		XMINT2 Octree2 = OctreeSet(vertices[idx1].pos);
		XMINT2 Octree3 = OctreeSet(vertices[idx2].pos);

		triangle[Octree1.x][Octree1.y].emplace_back(addTriangle);
		if (Octree1 != Octree2)
		{
			triangle[Octree2.x][Octree2.y].emplace_back(addTriangle);
		}
		if ((Octree1 != Octree3) && (Octree2 != Octree3))
		{
			triangle[Octree3.x][Octree3.y].emplace_back(addTriangle);
		}

		object->SetVertex(vertices[idx0].pos);
		object->SetVertex(vertices[idx1].pos);
		object->SetVertex(vertices[idx2].pos);
	}

	int a = 0;
	a = 1;
}

void MeshCollider::Update()
{
	matWorld = GetObject3d()->GetMatWorld();
	invMatWorld = XMMatrixInverse(nullptr, matWorld);
	if (isInit && !isCreateBuffer)
	{
		object->Initialize();
	}
}

void MeshCollider::Draw()
{
	object->SetMatWorld(GetObject3d()->GetMatWorld());
	object->Draw();
}

bool MeshCollider::CheckCollisionSphere(const Sphere& _sphere, DirectX::XMVECTOR* _inter, DirectX::XMVECTOR* _reject)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Sphere localSphere;
	localSphere.center = XMVector3Transform(_sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	if (_sphere.center.m128_f32[0] < worldmin.m128_f32[0] || _sphere.center.m128_f32[0] > worldmax.m128_f32[0] ||
		_sphere.center.m128_f32[2] <  worldmin.m128_f32[2] || _sphere.center.m128_f32[2] >  worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const XMINT2 Octree = OctreeSet({ localSphere.center.m128_f32[0],localSphere.center.m128_f32[1],localSphere.center.m128_f32[2] });

	std::vector<Triangle>::const_iterator it = triangle[Octree.x][Octree.y].cbegin();
	for (; it != triangle[Octree.x][Octree.y].cend(); ++it) {
		const Triangle& mesh = *it;

		if (Collision::CheckSphere2Triangle(localSphere, mesh, _inter, _reject)) {
			if (_inter) {
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();

				*_inter = XMVector3Transform(*_inter, matWorld);
			}
			if (_reject) {
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();

				*_reject = XMVector3TransformNormal(*_reject, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& _ray, float* _distance, DirectX::XMVECTOR* _inter)
{
	// オブジェクトのローカル座標系でのレイを得る
	Ray localRay;
	localRay.start = XMVector3Transform(_ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(_ray.dir, invMatWorld);

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	if (_ray.start.m128_f32[0] < worldmin.m128_f32[0] || _ray.start.m128_f32[0] > worldmax.m128_f32[0] ||
		_ray.start.m128_f32[2] <  worldmin.m128_f32[2] || _ray.start.m128_f32[2] >  worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const XMINT2 Octree = OctreeSet({ localRay.start.m128_f32[0],localRay.start.m128_f32[1],localRay.start.m128_f32[2] });

	std::vector<Triangle>::const_iterator it = triangle[Octree.x][Octree.y].cbegin();

	for (; it != triangle[Octree.x][Octree.y].cend(); ++it) {
		const Triangle& mesh = *it;

		XMVECTOR tempInter;
		if (Collision::CheckRay2Triangle(localRay, mesh, nullptr, &tempInter)) {

			const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();

			tempInter = XMVector3Transform(tempInter, matWorld);

			if (_distance) {
				XMVECTOR sub = tempInter - _ray.start;
				*_distance = XMVector3Dot(sub, _ray.dir).m128_f32[0];
			}

			if (_inter) {
				*_inter = tempInter;
			}

			return true;
		}
	}

	return false;
}

bool MeshCollider::CheckCollisionCapsule(const Capsule& _capsule)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Capsule localCapsule;
	localCapsule.startPosition = _capsule.startPosition.DirectXVector3Transform(invMatWorld);
	localCapsule.endPosition = _capsule.endPosition.DirectXVector3Transform(invMatWorld);
	localCapsule.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	XMVECTOR worldmax = XMVector3Transform(max, matWorld);
	XMVECTOR worldmin = XMVector3Transform(min, matWorld);

	if (_capsule.startPosition.x < worldmin.m128_f32[0] || _capsule.startPosition.x > worldmax.m128_f32[0] ||
		_capsule.startPosition.z <  worldmin.m128_f32[2] || _capsule.startPosition.z >  worldmax.m128_f32[2] ||
		_capsule.endPosition.x < worldmin.m128_f32[0] || _capsule.endPosition.x > worldmax.m128_f32[0] ||
		_capsule.endPosition.z <  worldmin.m128_f32[2] || _capsule.endPosition.z >  worldmax.m128_f32[2])
	{
		return false;
	}

	//プレイヤーの八分木位置
	const XMINT2 Octree[2] = {
		OctreeSet({ localCapsule.startPosition.x,localCapsule.startPosition.y,localCapsule.startPosition.z }),
		OctreeSet({ localCapsule.endPosition.x,localCapsule.endPosition.y,localCapsule.endPosition.z })
	};

	const int roopNum = 1 + (Octree[0] != Octree[1]);

	for (int i = 0; i < roopNum; i++)
	{
		std::vector<Triangle>::const_iterator it = triangle[Octree[i].x][Octree[i].y].cbegin();

		for (; it != triangle[Octree[i].x][Octree[i].y].cend(); ++it) {
			const Triangle& mesh = *it;

			if (Collision::CheckTriangleCapsule(mesh, localCapsule)) {
				return true;
			}
		}
	}

	//for (int x = 0; x < 8; x++)
	//{
	//	for (int y = 0; y < 8; y++)
	//	{
	//		std::vector<Triangle>::const_iterator it = triangle[x][y].cbegin();

	//		for (; it != triangle[x][y].cend(); ++it) {
	//			const Triangle& mesh = *it;

	//			if (Collision::CheckTriangleCapsule(mesh, localCapsule)) {
	//				return true;
	//			}
	//		}
	//	}
	//}

	return false;
}