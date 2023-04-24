#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "GraphicsPipelineManager.h"
#include "Texture.h"
#include "Model.h"

class Camera;
class LightGroup;

class InstanceObject
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//頂点データ3D
	struct VERTEX {
		XMFLOAT3 pos;
		XMFLOAT3 uv;
		XMFLOAT3 normal;
	};

	//一度に描画できる個数
	static const int draw_max_num = 512;

	struct OBJECT_INFO
	{
		XMFLOAT4 baseColor[draw_max_num];//ベースカラー
		XMMATRIX matWorld[draw_max_num];//world行列
	};

	// 定数バッファ用データ構造体B0
	struct CONST_BUFFER_DATA_B0
	{
		XMMATRIX viewproj; // ビュープロジェクション行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		unsigned int isBloom;//ブルームの有無
		unsigned int isToon;//トゥーンの有無
		unsigned int isOutline;//アウトラインの有無
		unsigned int isLight;//ライティングの有無
	};

public://メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_model">モデル</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<InstanceObject> Create(Model* _model);

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="_camera">カメラ</param>
	static void SetCamera(Camera* _camera) { InstanceObject::camera = _camera; }

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="_light">ライトグループ</param>
	static void SetLightGroup(LightGroup* _light) { InstanceObject::light = _light; }

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="_cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

private://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_model">モデル</param>
	void Initialize(Model* _model);

public:

	InstanceObject() {};
	~InstanceObject();

	/// <summary>
	/// 描画セット
	/// </summary>
	/// <param name="_pos">座標</param>
	/// <param name="_scale">大きさ</param>
	/// <param name="_rotation">回転角</param>
	/// <param name="_color">色</param>
	void DrawInstance(const XMFLOAT3& _pos, const XMFLOAT3& _scale,
		const XMFLOAT3& _rotation, const XMFLOAT4& _color);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// パイプラインのセット
	/// </summary>
	/// <param name="_pipeline">パイプライン</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { pipeline = _pipeline; }

	/// <summary>
	/// インスタンシング描画最大描画数になっていないかのチェック
	/// </summary>
	/// <returns></returns>
	bool GetInstanceDrawCheck() { return instanceDrawNum < draw_max_num; }

	/// <summary>
	/// インスタンシング描画個数
	/// </summary>
	/// <returns></returns>
	int GetInstanceDrawNum() { return instanceDrawNum; }

private:

	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプライン
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;
	//カメラ
	static Camera* camera;
	//ライト
	static LightGroup* light;
	//アウトラインの色
	static XMFLOAT4 outlineColor;
	//アウトラインの幅
	static float outlineWidth;

private:

	//モデル
	Model* model;
	//ベースカラー
	OBJECT_INFO objInform;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB1;
	//ブルームの有無
	bool isBloom = false;
	//トゥーンの有無
	bool isToon = false;
	//アウトラインの有無
	bool isOutline = false;
	//ライティング
	bool isLight = true;
	//インスタンシング描画個数
	int instanceDrawNum = 0;

public:

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
	static void SetOutlineColor(const XMFLOAT4& _outlineColor) { InstanceObject::outlineColor = _outlineColor; }

	/// <summary>
	/// アウトラインの幅セット
	/// </summary>
	/// <param name="_outlineWidth">幅</param>
	static void SetOutlineWidth(float _outlineWidth) { InstanceObject::outlineWidth = _outlineWidth; }

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
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetWorldPosition();
};