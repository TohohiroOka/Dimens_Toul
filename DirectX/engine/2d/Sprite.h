#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Sprite
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VERTEX
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT2 uv;  // uv座標
	};

	/// <summary>
	/// 定数バッファ用データ構造体
	/// </summary>
	struct CONST_BUFFER_DATA
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

	struct INFORMATION
	{
		bool isDelete = false; //シーン遷移で削除を行うか
		std::unique_ptr<Texture> instance = nullptr;
	};

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="_keepName">保存名</param>
	/// <param name="_filename">画像ファイル名</param>
	/// <param name="_isDelete">シーン遷移で削除を行うか</param>
	static void LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete = true);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="_cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="_name">テクスチャ保存名</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<Sprite> Create(const std::string& _name);

	/// <summary>
	/// シーンごとの解放処理
	/// </summary>
	static void SceneFinalize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// パイプラインのセット
	/// </summary>
	/// <param name="_pipeline">パイプライン</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { pipeline = _pipeline; }

protected: // 静的メンバ変数

	// 頂点数
	static const int vertNum = 4;
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプライン
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;
	//テクスチャ情報
	static std::map<std::string, INFORMATION> texture;
	// 射影行列
	static XMMATRIX matProjection;

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Sprite();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_name">テクスチャ保存名</param>
	/// <param name="_anchorpoint">アンカーポイント</param>
	/// <param name="_isFlipX">左右反転するか</param>
	/// <param name="_isFlipY">上下反転するか</param>
	void Initialize(const std::string& _name, const XMFLOAT2& _anchorpoint,bool _isFlipX = false, bool _isFlipY = false);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

protected: // メンバ変数

	//テクスチャ名
	std::string name;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	XMFLOAT2 position{ 0, 0 };
	// スプライト幅、高さ
	XMFLOAT2 size = { 100.0f, 100.0f };
	// アンカーポイント
	XMFLOAT2 anchorpoint = { 0, 0 };
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	XMFLOAT2 texLeftTop = { 0, 0 };
	// テクスチャ幅、高さ
	XMFLOAT2 texSize = { 500.0f, 500.0f };

protected: // メンバ関数

	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();

public:

	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const XMFLOAT2& GetPosition() { return position; }

	/// <summary>
	/// 回転角の取得
	/// </summary>
	/// <returns>回転角</returns>
	float GetRotation() { return rotation; }

	/// <summary>
	/// 大きさの取得
	/// </summary>
	/// <returns>大きさ</returns>
	const XMFLOAT2& GetSize() { return size; }

	/// <summary>
	/// 高さの取得
	/// </summary>
	/// <returns>高さ</returns>
	const XMFLOAT2& GetTexSize() { return texSize; }

	/// <summary>
	/// 終点の取得
	/// </summary>
	/// <returns>終点</returns>
	const XMFLOAT2& GetTexLeftTop() { return texLeftTop; }

	/// <summary>
	/// 色の取得
	/// </summary>
	/// <returns>色</returns>
	const XMFLOAT4& GetColor() { return color; }

	/// <summary>
	/// アンカーポイントの取得
	/// </summary>
	/// <returns>アンカーポイント</returns>
	const XMFLOAT2& GetAnchorpoint() { return anchorpoint; }

	/// <summary>
	/// 左右反転フラグの取得
	/// </summary>
	/// <returns>左右反転フラグ</returns>
	bool GetIsFlipX() { return isFlipX; }

	/// <summary>
	/// 上下反転フラグの取得
	/// </summary>
	/// <returns>左右反転フラグ</returns>
	bool GetIsFlipY() { return isFlipY; }

	/// <summary>
	/// テクスチャのセット
	/// </summary>
	void SetTexNumber(const std::string& _name) { this->name = _name; };

	/// <summary>
	/// 座標の入力
	/// </summary>
	void SetPosition(const XMFLOAT2& _position) { this->position = _position; }

	/// <summary>
	/// 回転角の入力
	/// </summary>
	void SetRotation(float _rotation) { this->rotation = _rotation; }

	/// <summary>
	/// 大きさの入力
	/// </summary>
	void SetSize(const XMFLOAT2& _size) { this->size = _size; }

	/// <summary>
	/// 高さの入力
	/// </summary>
	void SetTexSize(const XMFLOAT2& _texSize) { this->texSize = _texSize; }

	/// <summary>
	/// 終点の入力
	/// </summary>
	void SetTexLeftTop(const XMFLOAT2& _texLeftTop) { this->texLeftTop = _texLeftTop; }

	/// <summary>
	/// 色の入力
	/// </summary>
	void SetColor(const XMFLOAT4& _color) { this->color = _color; }

	/// <summary>
	/// アンカーポイントの入力
	/// </summary>
	void SetAnchorpoint(const XMFLOAT2& _anchorpoint) { this->anchorpoint = _anchorpoint; }

	/// <summary>
	/// 左右反転フラグの入力
	/// </summary>
	void SetIsFlipX(bool _isFlipX) { this->isFlipX = _isFlipX; };

	/// <summary>
	/// 上下反転フラグの入力
	/// </summary>
	void SetIsFlipY(bool _isFlipY) { this->isFlipY = _isFlipY; };
};