#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"

class DrawLine
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
	struct VertexPos
	{
		XMFLOAT3 pos; // xyz座標
	};

	/// <summary>
	/// 定数バッファ用データ構造体
	/// </summary>
	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色 (RGBA)
		XMMATRIX mat;	// ３Ｄ変換行列
	};

private: // 静的メンバ関数

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device* device);

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
	/// スプライト生成
	/// </summary>
	static std::unique_ptr<DrawLine> Create();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

protected: // 静的メンバ変数

	// 頂点数
	static const int vertNum = 4;
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプライン
	static std::unique_ptr<GraphicsPipelineManager> pipeline;
	// 射影行列
	static XMMATRIX matProjection;

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DrawLine() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DrawLine();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 角度を求める
	/// </summary>
	/// <param name="startPoint">始点</param>
	/// <param name="endPoint">終点</param>
	/// <returns>二点間の角度</returns>
	float GetAngle(XMFLOAT2 startPoint, XMFLOAT2 endPoint);

	/// <summary>
	/// 線の情報セット
	/// </summary>
	/// <param name="startPoint">始点</param>
	/// <param name="endPoint">終点</param>
	/// <param name="color">色</param>
	/// <param name="width">線の幅</param>
	void SetLine(DirectX::XMFLOAT2 startPoint, DirectX::XMFLOAT2 endPoint, XMFLOAT4 color, float width);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

protected: // メンバ変数

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// ワールド行列
	XMMATRIX matWorld{};
	// 色
	XMFLOAT4 color = { 1, 1, 1, 1 };
};