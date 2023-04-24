#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <array>

class DescriptorHeapManager
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// 描画前準備
	/// </summary>
	/// <param name="_cmdList">コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DescriptorHeapManager() {};

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~DescriptorHeapManager();

	/// <summary>
	/// シェーダーリソースビューの作成
	/// </summary>
	/// <param name="_texBuffer">テクスチャバッファ</param>
	/// <param name="_srvDesc">シェーダーリソースビュー設定</param>
	void CreateSRV(Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer, D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc);

private:

	//デバイス
	static ID3D12Device* device;
	//デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	//デスクリプタの大きさ
	static const int DescriptorsSize = 512;
	//デスクリプタテーブルの制御
	static std::array<bool, DescriptorsSize> TableManager;

public:

	int heapNumber = 0;
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpu;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpu;
};