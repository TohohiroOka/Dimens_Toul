#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Camera;

class CubeMap
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
	};

	//定数バッファの構造体
	struct CONST_BUFFER_DATA {
		XMMATRIX viewproj; // ビュープロジェクション行列
		XMMATRIX matWorld; // ワールド行列
		XMFLOAT3 cameraPos; // カメラ座標（ワールド座標）
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
	/// <param name="_cmdList">コマンドリスト</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<CubeMap> Create(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="_camera">カメラ</param>
	static void SetCamera(Camera* _camera) {
		CubeMap::camera = _camera;
	}

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
	/// 解放処理
	/// </summary>
	static void Finalize();

private://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

public:

	CubeMap() {};
	~CubeMap();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// キューブマップの転送
	/// </summary>
	/// <param name="_cmdList">描画コマンドリスト</param>
	/// <param name="_rootParameterIndex"></param>
	void TransferTextureBubber(ID3D12GraphicsCommandList* _cmdList, const UINT& _rootParameterIndex);

	/// <summary>
	/// キューブマップのセット
	/// </summary>
	/// <returns></returns>
	Texture* SetTexture() { return texture.get(); }

	/// <summary>
	/// パイプラインのセット
	/// </summary>
	/// <param name="_pipeline">パイプライン</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { pipeline = _pipeline; }

private:

	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプライン
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;
	//カメラ
	static Camera* camera;
	//インデックス数
	static const int indexNum = 36;

private:

	//テクスチャ情報
	std::unique_ptr<Texture> texture = nullptr;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//キューブマップバッファ
	ComPtr<ID3D12Resource> texConstBuffer;
	//リソース配列
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//座標
	XMFLOAT3 position = { 0,400,0 };
	//大きさ
	XMFLOAT3 scale = { 500,500,500 };
	//回転
	XMFLOAT3 rotation = {};
};