#pragma once

#include <DirectXMath.h>

/// <summary>
/// 点光源
/// </summary>
class PointLight
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
	struct ConstBufferData
	{
		XMFLOAT3 lightPos;
		float pad1;
		XMFLOAT3 lightColor;
		float pad2;
		XMFLOAT3 lightAtten;
		unsigned int active;
	};

public: // メンバ関数

	inline void SetLightPos(const XMFLOAT3& _lightpos) { this->lightPos = _lightpos; }
	inline const XMFLOAT3& GetLightPos() { return lightPos; }

	inline void SetLightColor(const XMFLOAT3& _lightcolor) { this->lightColor = _lightcolor; }
	inline const XMFLOAT3& GetLightColor() { return lightColor; }

	inline void SetLightAtten(const XMFLOAT3& _lightAtten) { this->lightAtten = _lightAtten; }
	inline const XMFLOAT3& GetLightAtten() { return lightAtten; }

	inline void SetActive(bool _active) { this->active = _active; }
	inline bool IsActive() { return active; }

private: // メンバ変数
	// ライト座標（ワールド座標系）
	XMFLOAT3 lightPos = { 0,0,0 };
	// ライト色
	XMFLOAT3 lightColor = { 1,1,1 };
	// ライト距離減衰係数
	XMFLOAT3 lightAtten = { 1.0f, 1.0f, 1.0f };
	// 有効フラグ
	bool active = false;
};