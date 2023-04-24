#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>
#include "PrimitiveObject3D.h"

#include "Vector3.h"

/// <summary>
/// メッシュ衝突判定オブジェクト
/// </summary>
class MeshCollider :
	public BaseCollider
{
public:
	MeshCollider()
	{
		// メッシュ形状をセット
		shapeType = COLLISIONSHAPE_MESH;
	}

	/// <summary>
	/// 八分木の最大最小の保存
	/// </summary>
	/// <param name="_model">モデル</param>
	void MinMax(Model* _model);

	/// <summary>
	/// 八分木の最大最小の保存
	/// </summary>
	/// <param name="_vertices">頂点</param>
	void MinMax(const std::vector<Mesh::VERTEX>& _vertices);

	/// <summary>
	/// 八分木の現在地セット
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <returns>現在番号</returns>
	DirectX::XMINT2 OctreeSet(const DirectX::XMFLOAT3& _pos);

	/// <summary>
	/// 三角形の配列を構築する
	/// </summary>
	/// <param name="_model">モデル</param>
	void ConstructTriangles(Model* _model);

	/// <summary>
	/// 三角形の配列を構築する
	/// </summary>
	/// <param name="_vertices">頂点</param>
	/// <param name="_indices">インデック</param>
	void ConstructTriangles(const std::vector<Mesh::VERTEX>& _vertices, const std::vector<unsigned long>& _indices);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 球との当たり判定
	/// </summary>
	/// <param name="_sphere">球</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <param name="_reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionSphere(const Sphere& _sphere, DirectX::XMVECTOR* _inter = nullptr, DirectX::XMVECTOR* _reject = nullptr);

	/// <summary>
	/// レイとの当たり判定
	/// </summary>
	/// <param name="_sphere">レイ</param>
	/// <param name="_distance">距離（出力用）</param>
	/// <param name="_inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionRay(const Ray& _ray, float* _distance, DirectX::XMVECTOR* _inter);

	/// <summary>
	/// カプセルとの当たり判定
	/// </summary>
	/// <param name="_capsule">カプセル</param>
	/// <returns>交差しているか否か</returns>
	bool CheckCollisionCapsule(const Capsule& _capsule);

private:

	//八分木分割個数
	static const int octreeSplit = 8;
	//判定用メッシュの情報
	std::array<std::array<std::vector<Triangle>, octreeSplit>, octreeSplit> triangle;
	//八分木の最小値
	DirectX::XMVECTOR min = {};
	//八分木の最大値
	DirectX::XMVECTOR max = {};
	//八分木の分割範囲
	DirectX::XMFLOAT3 octtreeRange[9] = {};
	// ワールド行列
	DirectX::XMMATRIX matWorld;
	// ワールド行列の逆行列
	DirectX::XMMATRIX invMatWorld;
	//オブジェクトの初期化フラグ
	bool isInit = false;
	//オブジェクトのbuffer生成フラグ
	bool isCreateBuffer = false;
	//コリジョンの視覚化用
	std::unique_ptr<PrimitiveObject3D> object = nullptr;
};