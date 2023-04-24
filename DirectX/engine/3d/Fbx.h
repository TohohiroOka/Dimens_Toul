#pragma once
#include "FbxModel.h"
#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Camera;
class LightGroup;

class Fbx
{
protected:// エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private://構造体宣言

	//定数バッファ用データB0
	struct ConstBufferDataB0
	{
		XMFLOAT4 color;
		XMMATRIX viewproj; // ビュープロジェクション行列
		XMMATRIX world; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
		unsigned int isSkinning;//スキニングを行うか
		unsigned int isBloom;//ブルームの有無
		unsigned int isToon;//トゥーンの有無
		unsigned int isOutline;//アウトラインの有無
	};

	// 定数バッファ用データ構造体B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 baseColor;//ベースカラ―
		float pad1; // パディング
		XMFLOAT3 ambient; // アンビエント係数
		float pad2; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float metalness; // 金属度(0 or 1)
		float specular; // 鏡面反射度
		float roughness; // 粗さ
		float alpha;	// アルファ
		//float pad[3];//パディング
	};

private://静的メンバ関数関数

	/// <summary>
	/// パイプライン設定
	/// </summary>
	static void CreateGraphicsPipeline();

public://静的メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <returns></returns>
	Fbx() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Fbx();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <param name="model">モデル</param
	static std::unique_ptr<Fbx> Create(FbxModel* model = nullptr);

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) { Fbx::camera = camera; }

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="lightGroup">ライトグループ</param>
	static void SetLightGroup(LightGroup* lightGroup) { Fbx::lightGroup = lightGroup; }

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="cubeTex">キューブマップ</param>
	static void SetCubeTex(Texture* cubetex) { Fbx::cubetex = cubetex; }

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

public:

	/// <summary>
	/// 生成
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// マテリアル情報を定数バッファに送る
	/// </summary>
	void TransferMaterial();

private://静的メンバ変数

	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// カメラ
	static Camera* camera;
	// ライト
	static LightGroup* lightGroup;
	//パイプライン
	static std::unique_ptr<GraphicsPipelineManager> pipeline;
	//アウトラインの色
	static XMFLOAT4 outlineColor;
	//アウトラインの幅
	static float outlineWidth;
	//キューブマップ
	static Texture* cubetex;

private://メンバ変数

	//モデル
	FbxModel* model = nullptr;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffB0;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuffB1;
	//座標
	XMFLOAT3 position = {};
	// 回転角
	XMFLOAT3 rotation = {};
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// 色
	XMFLOAT4 color = { 1,1,1,1 };
	// ローカルワールド変換行列
	XMMATRIX matWorld = {};
	//マテリアルが変化したか
	bool isTransferMaterial = false;
	//ベースカラ―
	XMFLOAT3 baseColor = { 1,1,1 };
	//金属度(0 or 1)
	float metalness = 0.0f;
	//鏡面反射度
	float specular = 0.5f;
	//粗さ
	float roughness = 0.0f;
	//ブルームの有無
	bool isBloom = false;
	//トゥーンの有無
	bool isToon = false;
	//アウトラインの有無
	bool isOutline = false;

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
	/// 色の取得
	/// </summary>
	/// <returns>色</returns>
	const XMFLOAT4& GetColor() { return color; }

	/// <summary>
	/// 大きさの取得
	/// </summary>
	/// <returns>大きさ</returns>
	const XMFLOAT3& GetScale() { return scale; }

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	FbxModel* GetModel() { return model; }

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// ベースカラー取得
	/// </summary>
	/// <returns>ベースカラー</returns>
	const XMFLOAT3& GetBaseColor() { return baseColor; }

	/// <summary>
	/// 金属度取得
	/// </summary>
	/// <returns>金属度</returns>
	float GetMetalness() { return metalness; }

	/// <summary>
	/// 鏡面反射度取得
	/// </summary>
	/// <returns>金属度</returns>
	float GetSpecular() { return specular; }

	/// <summary>
	/// 金属度取得
	/// </summary>
	/// <returns>金属度</returns>
	float GetRoughness() { return roughness; }

	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(XMFLOAT3 position) { this->position = position; }

	/// <summary>
	/// 回転角の設定
	/// </summary>
	/// <param name="rotation">回転角</param>
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	/// <summary>
	/// 色の設定
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(XMFLOAT4 color) { this->color = color; }

	/// <summary>
	/// ブルームのセット
	/// </summary>
	/// <param name="isBloom">ブルーム有->true / 無->false</param>
	void SetBloom(bool isBloom) { this->isBloom = isBloom; }

	/// <summary>
	/// トゥーンのセット
	/// </summary>
	/// <param name="isToon">トゥーン有->true / 無->false</param>
	void SetToon(bool isToon) { this->isToon = isToon; }

	/// <summary>
	/// アウトラインのセット
	/// </summary>
	/// <param name="isOutline">アウトライン有->true / 無->false</param>
	void SetOutline(bool isOutline) { this->isOutline = isOutline; }

	/// <summary>
	/// アウトラインの色セット
	/// </summary>
	/// <param name="outlineColor">幅</param>
	static void SetOutlineColor(XMFLOAT4 outlineColor) { Fbx::outlineColor = outlineColor; }

	/// <summary>
	/// アウトラインの幅セット
	/// </summary>
	/// <param name="outlineWidth">幅</param>
	static void SetOutlineWidth(float outlineWidth) { Fbx::outlineWidth = outlineWidth; }

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(FbxModel* model) { this->model = model; }

	/// <summary>
	/// スケールの設定
	/// </summary>
	/// <param name="position">スケール</param>
	void SetAnimation(bool isAnimation) { model->isAnimation = isAnimation; }

	/// <summary>
	/// ベースカラーセット
	/// </summary>
	void SetBaseColor(const XMFLOAT3& baseColor) { 
		this->baseColor = baseColor;
		isTransferMaterial = true;
	}

	/// <summary>
	/// 金属度セット
	/// </summary>
	void SetMetalness(float metalness) { 
		this->metalness = metalness;
		isTransferMaterial = true;
	}

	/// <summary>
	/// 鏡面反射度セット
	/// </summary>
	void SetSpecular(float specular) { 
		this->specular = specular;
		isTransferMaterial = true;
	}

	/// <summary>
	/// 金属度セット
	/// </summary>
	void SetRoughness(float roughness) { 
		this->roughness = roughness;
		isTransferMaterial = true;
	}
};