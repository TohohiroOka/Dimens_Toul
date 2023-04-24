#pragma once

#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "InterfaceObject3d.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;
	BaseCollider() = default;
	virtual ~BaseCollider() = default;

	inline void SetObject(InterfaceObject3d* object) {
		this->object3d = object;
	}

	inline InterfaceObject3d* GetObject3d() {
		return object3d;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	inline COLILSION_SHAPE_TYPE GetShapeType() { return shapeType; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="info">衝突情報</param>
	inline void OnCollision(const CollisionInfo& _info) {
		object3d->OnCollision(_info);
	}

	/// <summary>
	/// 当たり判定属性をセット
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void SetAttribute(const unsigned short& _attribute) {
		this->attribute = _attribute;
	}

	/// <summary>
	/// 当たり判定属性を追加
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void AddAttribute(const unsigned short& _attribute) {
		this->attribute |= _attribute;
	}

	/// <summary>
	/// 当たり判定属性を削除
	/// </summary>
	/// <param name="attribute">当たり判定属性</param>
	inline void RemoveAttribute(const unsigned short& _attribute) {
		this->attribute &= !_attribute;
	}

protected:
	InterfaceObject3d* object3d = nullptr;
	// 形状タイプ
	COLILSION_SHAPE_TYPE shapeType = SHAPE_UNKNOWN;
	// 当たり判定属性
	unsigned short attribute = 0b1111111111111111;
};