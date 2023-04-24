#include "Material.h"
#include <DirectXTex.h>
#include <cassert>
#include <string>

using namespace DirectX;
using namespace std;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Material::device = nullptr;

Material::~Material()
{
	texture.reset();
	constBuff.Reset();
}

void Material::StaticInitialize(ID3D12Device* _device)
{
	// 再初期化チェック
	assert(!Material::device);

	Material::device = _device;
}

Material * Material::Create()
{
	Material* instance = new Material;

	instance->Initialize();

	return instance;
}

void Material::Initialize()
{
	// 定数バッファの生成
	CreateConstantBuffer();
}

void Material::CreateConstantBuffer()
{
	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA_B1) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }
}

void Material::LoadTexture(const std::string& _directoryPath)
{
	// テクスチャなし
	if (textureFilename.size() == 0) {
		textureFilename = "white1x1.png";
	}

	// ファイルパスを結合
	string filepath = _directoryPath + textureFilename;

	texture = Texture::Create(filepath);
}

void Material::Update()
{
	HRESULT result;
	// 定数バッファへデータ転送
	CONST_BUFFER_DATA_B1* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->ambient = ambient;
		constMap->diffuse = diffuse;
		constMap->specular = specular;
		constMap->alpha = alpha;
		constBuff->Unmap(0, nullptr);
	}
}
