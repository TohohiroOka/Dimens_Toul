#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Material.h"
#include <unordered_map>

/// <summary>
/// 形状データ
/// </summary>
class Mesh
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// 頂点データ構造体（テクスチャあり）
	struct VERTEX
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device;

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <returns></returns>
	Mesh() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Mesh();

	/// <summary>
	/// 名前を取得
	/// </summary>
	/// <returns>名前</returns>
	const std::string& GetName() { return name; }

	/// <summary>
	/// 名前をセット
	/// </summary>
	/// <param name="_name">名前</param>
	void SetName(const std::string& _name);

	/// <summary>
	/// 頂点データの追加
	/// </summary>
	/// <param name="_vertex">頂点データ</param>
	void AddVertex(const VERTEX& _vertex);

	/// <summary>
	/// 頂点インデックスの追加
	/// </summary>
	/// <param name="_index">インデックス</param>
	void AddIndex(const unsigned long& _index);

	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの数</returns>
	inline size_t GetVertexCount() { return vertices.size(); }

	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="_indexPosition">座標インデックス</param>
	/// <param name="_indexVertex">頂点インデックス</param>
	void AddSmoothData(const unsigned long& _indexPosition, const unsigned long& _indexVertex);

	/// <summary>
	/// 平滑化された頂点法線の計算
	/// </summary>
	void CalculateSmoothedVertexNormals();

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	/// <returns>マテリアル</returns>
	Material* GetMaterial() { return material; }

	/// <summary>
	/// マテリアルの割り当て
	/// </summary>
	/// <param name="_material">マテリアル</param>
	void SetMaterial(Material* _material);

	/// <summary>
	/// バッファの生成
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// 頂点バッファ取得
	/// </summary>
	/// <returns>頂点バッファ</returns>
	const D3D12_VERTEX_BUFFER_VIEW& GetVBView() { return vbView; }

	/// <summary>
	/// インデックスバッファ取得
	/// </summary>
	/// <returns>インデックスバッファ</returns>
	const D3D12_INDEX_BUFFER_VIEW& GetIBView() { return ibView; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_cmdList">命令発行先コマンドリスト</param>
	/// <param name="_shaderResourceView">シェーダーリソースビュー番号</param>
	/// <param name="_instanceDrawNum">インスタンシング描画個数</param>
	void Draw(ID3D12GraphicsCommandList* _cmdList, const int _shaderResourceView, const int _instanceDrawNum);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_cmdList">命令発行先コマンドリスト</param>
	void VIDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 頂点配列を取得
	/// </summary>
	/// <returns>頂点配列</returns>
	inline const std::vector<VERTEX>& GetVertices() { return vertices; }

	/// <summary>
	/// インデックス配列を取得
	/// </summary>
	/// <returns>インデックス配列</returns>
	inline const std::vector<unsigned long>& GetIndices() { return indices; }

private: // メンバ変数
	// 名前
	std::string name;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// 頂点データ配列
	std::vector<VERTEX> vertices;
	// 頂点インデックス配列
	std::vector<unsigned long> indices;
	// 頂点法線スムージング用データ
	std::unordered_map<unsigned long, std::vector<unsigned long>> smoothData;
	// マテリアル
	Material* material = nullptr;
};