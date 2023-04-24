#include "Mesh.h"
#include <cassert>
#include <vector>

using namespace DirectX;

ID3D12Device* Mesh::device = nullptr;

Mesh::~Mesh()
{
	vertBuff.Reset();
	indexBuff.Reset();
}

void Mesh::StaticInitialize(ID3D12Device* _device)
{
	// 再初期化チェック
	assert(!Mesh::device);

	Mesh::device = _device;

	// マテリアルの静的初期化
	Material::StaticInitialize(_device);
}

void Mesh::SetName(const std::string& _name)
{
	this->name = _name;
}

void Mesh::AddVertex(const VERTEX& _vertex)
{
	vertices.emplace_back(_vertex);
}

void Mesh::AddIndex(const unsigned long& _index)
{
	indices.emplace_back(_index);
}

void Mesh::AddSmoothData(const unsigned long& _indexPosition, const unsigned long& _indexVertex)
{
	smoothData[_indexPosition].emplace_back(_indexVertex);
}

void Mesh::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr) {
		// 各面用の共通頂点コレクション
		std::vector<unsigned long>& v = itr->second;
		// 全頂点の法線を平均する
		XMVECTOR normal = {};
		for (unsigned long index : v) {
			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / float(v.size()));

		for (unsigned long index : v) {
			vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void Mesh::SetMaterial(Material* _material)
{
	this->material = _material;
}

void Mesh::CreateBuffers()
{
	HRESULT result;

	UINT sizeVB = static_cast<UINT>(sizeof(VERTEX) * vertices.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	if (FAILED(result)) {
		assert(0);
		return;
	}

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned long) * indices.size());
	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) { assert(0); }

	// インデックスバッファへのデータ転送
	unsigned long* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R32_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* _cmdList,const int _shaderResourceView, const int _instanceDrawNum)
{
	// 頂点バッファをセット
	_cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	_cmdList->IASetIndexBuffer(&ibView);
	// シェーダリソースビューをセット
	_cmdList->SetGraphicsRootDescriptorTable(_shaderResourceView, material->GetGpuHandle());

	// マテリアルの定数バッファをセット
	ID3D12Resource* constBuff = material->GetConstantBuffer();
	_cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());

	// 描画コマンド
	_cmdList->DrawIndexedInstanced((UINT)indices.size(), _instanceDrawNum, 0, 0, 0);
}

void Mesh::VIDraw(ID3D12GraphicsCommandList* _cmdList)
{
	// 頂点バッファをセット
	_cmdList->IASetVertexBuffers(0, 1, &vbView);
	// インデックスバッファをセット
	_cmdList->IASetIndexBuffer(&ibView);
}