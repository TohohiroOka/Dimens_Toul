#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include <imgui.h>

class DirectXCommon
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private://メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// imgui初期化
	/// </summary>
	void InitImgui();

public://静的メンバ関数

	/// <summary>
	/// directXCommon生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<DirectXCommon> Create();

public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <returns></returns>
	DirectXCommon() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DirectXCommon();

	/// <summary>
	/// 深度バッファ生成
	/// </summary>
	void CreateDepth();

	/// <summary>
	/// 描画前設定
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後設定
	/// </summary>
	void PostDraw();

	/// <summary>
	/// デバイスのgetter
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device.Get(); }

	/// <summary>
	/// コマンドリストのgetter
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

private://メンバ変数

	//デバイス
	ComPtr<ID3D12Device> device;
	//コマンドリスト
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//ファクトリー
	ComPtr<IDXGIFactory6> dxgiFactory;
	//コマンドアロケータ
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//コマンドキュー
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//スワップチェーン
	ComPtr<IDXGISwapChain4> swapchain;
	//バックバッファ
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//レンダーターゲットビュー用ディスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuffer;
	//深度ステンシルビュー用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//深度ビュー用のデスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	//フェンス
	ComPtr<ID3D12Fence> fence;
	//コマンドリスト完了までのカウント
	UINT64 fenceVal = 0;
	//imgui用ヒープ
	ComPtr<ID3D12DescriptorHeap> imguiHeap;
};