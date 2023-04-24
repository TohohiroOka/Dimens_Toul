#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

class LightGroup
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // 定数
	// 平行光源の数
	static const int DirLightNum = 3;
	// 点光源の数
	static const int PointLightNum = 3;
	// スポットライトの数
	static const int SpotLightNum = 3;
	// 丸影の数
	static const int CircleShadowNum = 1;

public: // サブクラス

	// 定数バッファ用データ構造体
	struct CONST_BUFFER_DATA
	{
		// 環境光の色
		XMFLOAT3 ambientColor;
		float pad1;
		// 平行光源用
		DirectionalLight::CONST_BUFFER_DATA dirLights[DirLightNum];
		// 点光源用
		PointLight::ConstBufferData pointLights[PointLightNum];
		// スポットライト用
		SpotLight::CONST_BUFFER_DATA spotLights[SpotLightNum];
		// 丸影用
		CircleShadow::CONST_BUFFER_DATA circleShadows[CircleShadowNum];
	};

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<LightGroup> Create();

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device;

public: // メンバ関数

	LightGroup() {};
	~LightGroup();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_cmdList">コマンドリスト</param>
	/// <param name="_rootParameterIndex">格納ルートパラメータ番号</param>
	void Draw(ID3D12GraphicsCommandList* _cmdList, const UINT& _rootParameterIndex);

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 標準のライト設定
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// 環境光のライト色をセット
	/// </summary>
	/// <param name="_color">ライト色</param>
	void SetAmbientColor(const XMFLOAT3& _color);

	/// <summary>
	/// 平行光源の有効フラグをセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_active">有効フラグ</param>
	void SetDirLightActive(int _index, bool _active);

	/// <summary>
	/// 平行光源のライト方向をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightdir">ライト方向</param>
	void SetDirLightDir(int _index, const XMVECTOR& _lightdir);

	/// <summary>
	/// 平行光源のライト色をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightcolor">ライト色</param>
	void SetDirLightColor(int _index, const XMFLOAT3& _lightcolor);

	/// <summary>
	/// 点光源の有効フラグをセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_active">有効フラグ</param>
	void SetPointLightActive(int _index, bool _active);

	/// <summary>
	/// 点光源のライト座標をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightpos">ライト座標</param>
	void SetPointLightPos(int _index, const XMFLOAT3& _lightpos);

	/// <summary>
	/// 点光源のライト色をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightcolor">ライト色</param>
	void SetPointLightColor(int _index, const XMFLOAT3& _lightcolor);

	/// <summary>
	/// 点光源のライト距離減衰係数をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="lightatten">ライト距離減衰係数</param>
	void SetPointLightAtten(int _index, const XMFLOAT3& _lightAtten);

	/// <summary>
	/// スポットライトの有効フラグをセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_active">有効フラグ</param>
	void SetSpotLightActive(int _index, bool _active);

	/// <summary>
	/// スポットライトのライト方向をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightdir">ライト方向</param>
	void SetSpotLightDir(int _index, const XMVECTOR& _lightdir);

	/// <summary>
	/// スポットライトのライト座標をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightpos">ライト座標</param>
	void SetSpotLightPos(int _index, const XMFLOAT3& _lightpos);

	/// <summary>
	/// スポットライトのライト色をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightcolor">ライト色</param>
	void SetSpotLightColor(int _index, const XMFLOAT3& _lightcolor);

	/// <summary>
	/// スポットライトのライト距離減衰係数をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="lightatten">ライト距離減衰係数</param>
	void SetSpotLightAtten(int _index, const XMFLOAT3& _lightAtten);

	/// <summary>
	/// スポットライトのライト減衰角度をセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_lightFactorAngle">x:減衰開始角度 y:減衰終了角度</param>
	void SetSpotLightFactorAngle(int _index, const XMFLOAT2& _lightFactorAngle);

	/// <summary>
	/// 丸影の有効フラグをセット
	/// </summary>
	/// <param name="_index">ライト番号</param>
	/// <param name="_active">有効フラグ</param>
	void SetCircleShadowActive(int _index, bool _active);

	/// <summary>
	/// 丸影のキャスター座標をセット
	/// </summary>
	/// <param name="_index">番号</param>
	/// <param name="_lightpos">キャスター座標</param>
	void SetCircleShadowCasterPos(int _index, const XMFLOAT3& _casterPos);

	/// <summary>
	/// 丸影の方向をセット
	/// </summary>
	/// <param name="_index">番号</param>
	/// <param name="_lightdir">方向</param>
	void SetCircleShadowDir(int _index, const XMVECTOR& _lightdir);

	/// <summary>
	/// 丸影のキャスターとライトの距離をセット
	/// </summary>
	/// <param name="_index">番号</param>
	/// <param name="_distanceCasterLight">キャスターとライトの距離</param>
	void SetCircleShadowDistanceCasterLight(int _index, float _distanceCasterLight);

	/// <summary>
	/// 丸影の距離減衰係数をセット
	/// </summary>
	/// <param name="_index">番号</param>
	/// <param name="lightatten">距離減衰係数</param>
	void SetCircleShadowAtten(int _index, const XMFLOAT3& _lightAtten);

	/// <summary>
	/// 丸影の減衰角度をセット
	/// </summary>
	/// <param name="_index">番号</param>
	/// <param name="_lightFactorAngle">x:減衰開始角度 y:減衰終了角度</param>
	void SetCircleShadowFactorAngle(int _index, const XMFLOAT2& _lightFactorAngle);

private: // メンバ変数
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	// 環境光の色
	XMFLOAT3 ambientColor = { 1,1,1 };

	// 平行光源の配列
	DirectionalLight dirLights[DirLightNum];

	// 点光源の配列
	PointLight pointLights[PointLightNum];

	// スポットライトの配列
	SpotLight spotLights[SpotLightNum];

	// 丸影の配列
	CircleShadow circleShadows[CircleShadowNum];

	// ダーティフラグ
	bool dirty = false;
};