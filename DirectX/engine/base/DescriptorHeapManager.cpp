#include "DescriptorHeapManager.h"

ID3D12Device* DescriptorHeapManager::device = nullptr;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> DescriptorHeapManager::descHeap = nullptr;
std::array<bool, DescriptorHeapManager::DescriptorsSize> DescriptorHeapManager::TableManager = {};

void DescriptorHeapManager::StaticInitialize(ID3D12Device* _device)
{
	HRESULT result = S_FALSE;

	// nullptrチェック
	assert(!DescriptorHeapManager::device);
	assert(_device);
	DescriptorHeapManager::device = _device;

	//デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = DescriptorsSize;
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));
}

void DescriptorHeapManager::PreDraw(ID3D12GraphicsCommandList* _cmdList)
{
	assert(descHeap);
	//デスクリプタヒープをセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	_cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DescriptorHeapManager::Finalize()
{
	descHeap.Reset();
}

DescriptorHeapManager::~DescriptorHeapManager()
{
	TableManager[heapNumber] = false;
}

void DescriptorHeapManager::CreateSRV(
	Microsoft::WRL::ComPtr<ID3D12Resource> _texBuffer, D3D12_SHADER_RESOURCE_VIEW_DESC _srvDesc)
{
	for (; heapNumber < DescriptorsSize; heapNumber++)
	{
		if (!TableManager[heapNumber])
		{
			TableManager[heapNumber] = true;
			break;
		}
	}

	cpu = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), heapNumber,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	gpu = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), heapNumber,
		device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	//ヒープのtexnumber番目にシェーダーリソースビューを作成
	device->CreateShaderResourceView(
		_texBuffer.Get(),
		&_srvDesc,
		cpu);
}