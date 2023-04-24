#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "GraphicsPipelineManager.h"

class Camera;

class DrawLine3D
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
	struct CONST_BUFFER_DATA {
		XMFLOAT4 color;//色
		XMMATRIX matWorld;//3D変換行列
		XMMATRIX viewproj;//3D変換行列
	};

private://静的メンバ関数

	/// <summary>
	/// 角度を求める
	/// </summary>
	/// <param name="_startPoint">始点</param>
	/// <param name="_endPoint">終点</param>
	/// <returns>二点間の角度</returns>
	float GetAngle(const XMFLOAT3& _startPoint, const XMFLOAT3& _endPoint);

public://静的メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <returns></returns>
	DrawLine3D() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DrawLine3D();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// Objectの生成
	/// </summary>
	/// <param name="_lineNum">線の本数</param>
	static std::unique_ptr<DrawLine3D> Create(int _lineNum);

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="_camera">カメラ</param>
	static void SetCamera(Camera* _camera) {
		DrawLine3D::camera = _camera;
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
	/// パイプラインの設定
	/// </summary>
	/// <param name="_pipeline"></param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { DrawLine3D::pipeline = _pipeline; }

public://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(int _lineNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 線の情報セット
	/// </summary>
	/// <param name="_startPoint">始点</param>
	/// <param name="_endPoint">終点</param>
	/// <param name="_width">線の幅</param>
	void SetLine(XMFLOAT3 _startPoint[], XMFLOAT3 _endPoint[], float _width);

	/// <summary>
	/// 色の変更
	/// </summary>
	void SetColor(const XMFLOAT4& _color) { this->color = _color; }

	/// <summary>
	/// //描画
	/// </summary>
	void Draw();

private://静的メンバ変数

	// 一本分の頂点数
	static const int vertNum = 4;
	// 一本分のインデックス数
	static const int indexNum = 6;
	// 一本分のインデックBasics
	unsigned short BASE_INDICES[indexNum] = {
	0,1,2,1,2,3,
	};
	//デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//カメラ
	static Camera* camera;
	//パイプライン
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;

protected://メンバ変数

	//頂点配列
	std::vector<XMFLOAT3> vertices;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//頂点データの要素数
	UINT vertexArrayNum = 0;
	//インデックスデータの要素数
	UINT indexArrayNum = 0;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 色
	XMFLOAT4 color = {};
};