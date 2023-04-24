#pragma once

#include <DirectXMath.h>

class InterfaceObject3d;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(InterfaceObject3d* _object, BaseCollider* _collider, const DirectX::XMVECTOR& _inter) {
		this->object = _object;
		this->collider = _collider;
		this->inter = _inter;
	}

	// 衝突相手のオブジェクト
	InterfaceObject3d* object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
};

