#pragma once

#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"

#include <d3d12.h>
#include <forward_list>

class BaseCollider;

class CollisionManager
{
public:// 静的メンバ関数
	static CollisionManager* GetInstance();

public:// メンバ関数
	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void AddCollider(BaseCollider* _collider) {
		colliders.push_front(_collider);
	}

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider">コライダー</param>
	inline void RemoveCollider(BaseCollider* _collider) {
		colliders.remove(_collider);
	}

	/// <summary>
	/// 全ての衝突チェック
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="_ray">レイ</param>
	/// <param name="_hitInfo">衝突情報</param>
	/// <param name="_maxDistance">最大距離</param>
	/// <returns>レイが任意のコライダーと交わる場合はtrue、それ以外はfalse</returns>
	bool Raycast(const Ray& _ray, RAYCAST_HIT* _hitInfo = nullptr, float _maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// レイキャスト
	/// </summary>
	/// <param name="_ray">レイ</param>
	/// <param name="_hitInfo">対象の衝突属性</param>
	/// <param name="_hitInfo">衝突情報</param>
	/// <param name="_maxDistance">最大距離</param>
	/// <returns>レイが任意のコライダーと交わる場合はtrue、それ以外はfalse</returns>
	bool Raycast(const Ray& _ray, const unsigned short& _attribute, RAYCAST_HIT* _hitInfo = nullptr, float _maxDistance = D3D12_FLOAT32_MAX);

	/// <summary>
	/// 球による衝突全検索
	/// </summary>
	/// <param name="_sphere">球</param>
	/// <param name="_callback">衝突時コールバック</param>
	/// <param name="_attribute">対象の衝突属性</param>
	void QuerySphere(const Sphere& _sphere, QueryCallback* _callback, const unsigned short& _attribute = (unsigned short)0xffffffff);

	/// <summary>
	/// カプセルによる衝突全検索
	/// </summary>
	/// <param name="_sphere">カプセル</param>
	/// <param name="_hitInfo">衝突情報</param>
	/// <param name="_maxDistance">最大距離</param>
	bool QueryCapsule(const Capsule& _capsule, const unsigned short& _attribute);

private:
	CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	~CollisionManager() = default;
	CollisionManager& operator=(const CollisionManager&) = delete;

	// コライダーのリスト
	std::forward_list<BaseCollider*> colliders;
};

