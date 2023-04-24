#pragma once

#include <DirectXMath.h>

/// <summary>
/// 平行光源
/// </summary>
class DirectionalLight
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// 定数バッファ用データ構造体
	struct CONST_BUFFER_DATA
	{
		XMVECTOR lightVec;
		XMFLOAT3 lightColor;
		unsigned int active;
	};

public: // メンバ関数
	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="_lightdir">ライト方向</param>
	inline void SetLightDir(const XMVECTOR& _lightdir) { this->lightdir = DirectX::XMVector3Normalize(_lightdir); }

	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const XMVECTOR& GetLightDir() { return lightdir; }

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="_lightcolor">ライト色</param>
	inline void SetLightColor(const XMFLOAT3& _lightcolor) { this->lightcolor = _lightcolor; }

	/// <summary>
	/// ライト色を取得
	/// </summary>
	/// <returns>ライト色</returns>
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="_active">有効フラグ</param>
	inline void SetActive(bool _active) { this->active = _active; }

	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return active; }

private: // メンバ変数
	// ライト方向（単位ベクトル）
	XMVECTOR lightdir = { 1,0,0,0 };
	// ライト色
	XMFLOAT3 lightcolor = { 1,1,1 };
	// 有効フラグ
	bool active = false;
};

