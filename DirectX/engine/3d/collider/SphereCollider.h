#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include <DirectXMath.h>

/// <summary>
/// 球衝突判定オブジェクト
/// </summary>
class SphereCollider : public BaseCollider, public Sphere
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	SphereCollider(XMVECTOR _offset = {0,0,0,0}, float _radius = 1.0f) :
		offset(_offset),
		radius(_radius)
	{
		// 球形状をセット
		shapeType = COLLISIONSHAPE_SPHERE;
	}

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 判定描画
	/// </summary>
	void Draw() override;

	inline const XMVECTOR& GetOffset() { return offset; }

	inline void SetOffset(const XMVECTOR& _offset) { this->offset = _offset; }

	inline float GetRadius() { return radius; }

	inline void SetRadius(float _radius) { this->radius = _radius; }

private:
	// オブジェクト中心からのオフセット
	XMVECTOR offset;
	// 半径
	float radius;
};

