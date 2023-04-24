#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <wrl.h>
#include "Texture.h"

/// <summary>
/// マテリアル
/// </summary>
class Material
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
	// 定数バッファ用データ構造体B1
	struct CONST_BUFFER_DATA_B1
	{
		XMFLOAT3 ambient; // アンビエント係数
		float pad1; // パディング
		XMFLOAT3 diffuse; // ディフューズ係数
		float pad2; // パディング
		XMFLOAT3 specular; // スペキュラー係数
		float alpha;	// アルファ
	};

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// マテリアル生成
	/// </summary>
	/// <returns>生成されたマテリアル</returns>
	static Material* Create();

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device;

public:

	// マテリアル名
	std::string name;
	// アンビエント影響度
	XMFLOAT3 ambient;
	// ディフューズ影響度
	XMFLOAT3 diffuse;
	// スペキュラー影響度
	XMFLOAT3 specular;
	// アルファ
	float alpha;
	// テクスチャファイル名
	std::string textureFilename;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Material();

	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetConstantBuffer() { return constBuff.Get(); }

	/// テクスチャ読み込み
	/// </summary>
	/// <param name="_directoryPath">読み込みディレクトリパス</param>
	void LoadTexture(const std::string& _directoryPath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	const CD3DX12_CPU_DESCRIPTOR_HANDLE& GetCpuHandle() { return texture->descriptor->cpu; }
	const CD3DX12_GPU_DESCRIPTOR_HANDLE& GetGpuHandle() { return texture->descriptor->gpu; }

private:

	//テクスチャ情報
	std::unique_ptr<Texture> texture = nullptr;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

private:
	// コンストラクタ
	Material() {
		ambient = { 0.3f, 0.3f, 0.3f };
		diffuse = { 0.0f, 0.0f, 0.0f };
		specular = { 0.0f, 0.0f, 0.0f };
		alpha = 1.0f;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateConstantBuffer();
};

