#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>

#include "DescriptorHeapManager.h"

class Texture
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <param name="_cmdList">ddsファイル時はcmdListを持ってくる</param>
	static std::unique_ptr<Texture> Create(const std::string& _fileName, ID3D12GraphicsCommandList* _cmdList = nullptr);

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Texture() {};

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Texture();

	/// <summary>
	/// テクスチャの読み込み
	/// </summary>
	/// <param name="_fileName">画像ファイルの名前</param
	void LoadTexture(const std::string& _fileName);

	/// <summary>
	/// ddsファイルの読み込み
	/// </summary>
	/// <param name="_fileName">画像ファイルの名前</param>
	/// <param name="_cmdList">コマンドリスト</param>
	void LoadTextureFromDDSFile(const std::string& _fileName, ID3D12GraphicsCommandList* _cmdList);

private:
	
	//デバイス
	static ID3D12Device* device;

public:
	//テクスチャバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> texBuffer = nullptr;
	//テクスチャのデスクリプタ
	std::unique_ptr<DescriptorHeapManager> descriptor = nullptr;
	//ddsのみに使用する
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	Microsoft::WRL::ComPtr<ID3D12Resource> texConstBuffer = nullptr;

};

