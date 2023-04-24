#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"

class Camera;

class NormalMap
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://構造体宣言
	//定数バッファの構造体
	struct ConstBufferData {
		XMFLOAT4 color1;//1枚目
		XMFLOAT4 color2;//2枚目
		XMFLOAT4 color3;//3枚目
		XMMATRIX matWorld;//3D変換行列
		XMMATRIX matView;//3D変換行列
		XMMATRIX maProjection;//3D変換行列
		float light;
		float uvPos;
	};

	//頂点データ3D
	struct Vertex {
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};

private://静的メンバ関数

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// テクスチャデスクリプタの生成
	/// </summary>
	static void CommonCreate();

public://静的メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <returns></returns>
	NormalMap() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NormalMap();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="filename"></param>
	/// <returns>テクスチャ番号</returns>
	static int LoadTexture(const wchar_t* filename);

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<NormalMap> Create();

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="camera">カメラ</param>
	static void SetCamera(Camera* camera) {
		NormalMap::camera = camera;
	}

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

public://メンバ関数

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// //描画
	/// </summary>
	/// <param name="colorTex">メインテクスチャ</param>
	/// <param name="normalTex">法線テクスチャ1</param>
	/// <param name="normalTex2">法線テクスチャ2</param>
	void Draw(int colorTex, int normalTex, int normalTex2);

private://静的メンバ変数

	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//カメラ
	static Camera* camera;
	//パイプライン
	static std::unique_ptr<GraphicsPipelineManager> pipeline;
	//テクスチャ用デスクリプタヒープの生成
	static ComPtr<ID3D12DescriptorHeap>descHeap;
	//テクスチャ最大登録数
	static const int textureNum = 512;
	//テクスチャリソース(テクスチャバッファ)の配列
	static ComPtr<ID3D12Resource>texBuffer[textureNum];
	//一辺の長さ
	static const int edge = 5;
	//頂点データ
	static Vertex vertices[24];
	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	static const UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * _countof(vertices));
	//インデックスデータ
	static const unsigned short indices[36];
	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * _countof(indices));
	//テクスチャの合計
	static int alltextureNum;

private://メンバ変数

	//テクスチャ番号
	UINT texNumber = -1;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 色1
	XMFLOAT4 color1 = {};
	// 色2
	XMFLOAT4 color2 = {};
	// 色3
	XMFLOAT4 color3 = {};
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//テクスチャの上下座標
	XMFLOAT2 texLeftTop = { 0,0 };
	//テクスチャ切り出しサイズ
	XMFLOAT2 texSise = { 500,500 };
	//UV位置
	float uvPos = 0;
	//光の位置
	float light = 0;

public:

	/// <summary>
	/// 座標
	/// </summary>
	void SetPosition(const XMFLOAT3& position) { this->position = position; }

	/// <summary>
	/// 回転角
	/// </summary>
	void SetRotation(const XMFLOAT3& rotation) { this->rotation = rotation; }

	/// <summary>
	/// 大きさ
	/// </summary>
	void SetScale(const XMFLOAT3& scale) { this->scale = scale; }

	/// <summary>
	/// メインテクスチャの色
	/// </summary>
	void SetMainTexColor(const XMFLOAT4& color1) { this->color1 = color1; }

	/// <summary>
	/// サブテクスチャ1の色
	/// </summary>
	void SetSubTexColor1(const XMFLOAT4& subColor1) { this->color2 = subColor1; }

	/// <summary>
	/// サブテクスチャ2の色
	/// </summary>
	void SetSubTexColor2(const XMFLOAT4& subColor2) { this->color3 = subColor2; }

	/// <summary>
	/// ライトの位置
	/// </summary>
	void SetLightPosition(const float& light) { this->light = light; }

	/// <summary>
	/// uv座標
	/// </summary>
	void SetUvPosition(bool uvPos) { this->uvPos = uvPos; };
};