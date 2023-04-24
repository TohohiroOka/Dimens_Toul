#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"
#include "Model.h"
#include "Texture.h"
#include "CollisionInfo.h"

class BaseCollider;
class Camera;
class LightGroup;

class InterfaceObject3d
{
protected:// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected:// サブクラス

	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA_B0
	{
		XMFLOAT4 baseColor;//ベースカラー
		XMMATRIX viewproj; // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		unsigned int isSkinning;//スキニングを行うか
		unsigned int isBloom;//ブルームの有無
		unsigned int isToon;//トゥーンの有無
		unsigned int isOutline;//アウトラインの有無
		unsigned int isLight;//ライティングの有無
	};

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// コマンドリストのセット
	/// </summary>
	/// <param name="_cmdList">コマンドリスト</param>
	static void SetCmdList(ID3D12GraphicsCommandList* _cmdList) { InterfaceObject3d::cmdList = _cmdList; }

	/// <summary>
	/// コマンドリストのセット
	/// </summary>
	static void ReleaseCmdList() { InterfaceObject3d::cmdList = nullptr; }

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="_camera">カメラ</param>
	static void SetCamera(Camera* _camera) { InterfaceObject3d::camera = _camera; }

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="_light">ライトグループ</param>
	static void SetLightGroup(LightGroup* _light) { InterfaceObject3d::light = _light; }

	/// <summary>
	/// キューブマップのセット
	/// </summary>
	/// <param name="_cubetex">キューブマップテクスチャ</param>
	static void SetLightGroup(Texture* _cubetex) { InterfaceObject3d::cubetex = _cubetex; }

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:

	InterfaceObject3d() = default;
	virtual ~InterfaceObject3d();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 当たり判定の描画
	/// </summary>
	virtual void ColliderDraw();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateWorldMatrix();

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="_info">衝突情報</param>
	virtual void OnCollision(const CollisionInfo& _info) {}

protected:

	// デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//カメラ
	static Camera* camera;
	//ライト
	static LightGroup* light;
	//アウトラインの色
	static XMFLOAT4 outlineColor;
	//アウトラインの幅
	static float outlineWidth;
	//キューブマップ
	static Texture* cubetex;

protected:

	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB1;
	//ベースカラー
	XMFLOAT4 baseColor = { 1,1,1,1 };
	//ブルームの有無
	bool isBloom = false;
	//トゥーンの有無
	bool isToon = false;
	//アウトラインの有無
	bool isOutline = false;
	//スキニング
	bool isSkinning = false;
	//ライティング
	bool isLight = true;
	// モデル
	Model* model = nullptr;
	// コライダー
	BaseCollider* collider = nullptr;
	// 親オブジェクト
	InterfaceObject3d* parent = nullptr;
	// ローカルワールド変換行列
	XMMATRIX matWorld = {};
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };

public:

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT3& GetPosition() { return position; }

	/// <summary>
	/// 回転の取得
	/// </summary>
	/// <returns>回転</returns>
	const XMFLOAT3& GetRotation() { return rotation; }

	/// <summary>
	/// 大きさの取得
	/// </summary>
	/// <returns>大きさ</returns>
	const XMFLOAT3& GetScale() { return scale; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="_position">座標</param>
	void SetPosition(const XMFLOAT3& _position) { this->position = _position; }

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="_rotation">回転角</param>
	void SetRotation(const XMFLOAT3& _rotation) { this->rotation = _rotation; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="_scale">スケール</param>
	void SetScale(const XMFLOAT3& _scale) { this->scale = _scale; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="_color">色</param>
	void SetColor(const XMFLOAT4& _color) { this->baseColor = _color; }

	/// <summary>
	/// ブルームのセット
	/// </summary>
	/// <param name="_isBloom">ブルーム有->true / 無->false</param>
	void SetBloom(bool _isBloom) { this->isBloom = _isBloom; }

	/// <summary>
	/// トゥーンのセット
	/// </summary>
	/// <param name="_isToon">トゥーン有->true / 無->false</param>
	void SetToon(bool _isToon) { this->isToon = _isToon; }

	/// <summary>
	/// ライティングのセット
	/// </summary>
	/// <param name="_isLight">ライティング有->true / 無->false</param>
	void SetLight(bool _isLight) { this->isLight = _isLight; }

	/// <summary>
	/// アウトラインのセット
	/// </summary>
	/// <param name="_isOutline">アウトライン有->true / 無->false</param>
	void SetOutline(bool _isOutline) { this->isOutline = _isOutline; }

	/// <summary>
	/// アウトラインの色セット
	/// </summary>
	/// <param name="_outlineColor">幅</param>
	static void SetOutlineColor(const XMFLOAT4& _outlineColor) { InterfaceObject3d::outlineColor = _outlineColor; }

	/// <summary>
	/// アウトラインの幅セット
	/// </summary>
	/// <param name="_outlineWidth">幅</param>
	static void SetOutlineWidth(float _outlineWidth) { InterfaceObject3d::outlineWidth = _outlineWidth; }

	/// <summary>
	/// アウトラインの色取得
	/// </summary>
	/// <param name="outlineColor">幅</param>
	static XMFLOAT4 GetOutlineColor() { return outlineColor; }

	/// <summary>
	/// アウトラインの幅取得
	/// </summary>
	/// <param name="outlineWidth">幅</param>
	static float GetOutlineWidth() { return outlineWidth; }

	/// <summary>
	/// コライダーのセット
	/// </summary>
	/// <param name="_collider">コライダー</param>
	void SetCollider(BaseCollider* _collider);

	/// <summary>
	/// コライダーの取得
	/// </summary>
	/// <returns>コライダー</returns>
	BaseCollider* GetCollider() { return collider; }

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// ワールド行列のセット
	/// </summary>
	/// <param name="_matWorld">ワールド行列</param>
	void SetMatWorld(const XMMATRIX& _matWorld) { this->matWorld = _matWorld; }

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetWorldPosition();

	/// <summary>
	/// 親オブジェクトのセット
	/// </summary>
	void SetParent(InterfaceObject3d* _parent) { parent = _parent; }
};