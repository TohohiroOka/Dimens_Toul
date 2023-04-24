#pragma once

/// <summary>
/// クエリーによる情報を得る為の構造体
/// </summary>
struct QUERY_HIT
{
	// 衝突相手のオブジェクト
	InterfaceObject3d* object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
	// 排斥ベクトル
	DirectX::XMVECTOR reject;
};

/// <summary>
/// クエリーで交差を検出した時の動作を規定するクラス
/// </summary>
class QueryCallback
{
public:
	QueryCallback() = default;
	virtual ~QueryCallback() = default;

	/// <summary>
	/// 交差時コールバック
	/// </summary>
	/// <param name="info">交差情報</param>
	/// <returns>クエリーを続けるならtrue、打ち切るならfalseを返す</returns>
	virtual bool OnQueryHit(const QUERY_HIT& _info) = 0;
};

