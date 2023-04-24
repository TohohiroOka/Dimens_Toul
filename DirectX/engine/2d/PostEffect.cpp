#include "PostEffect.h"
#include "WindowApp.h"
#include "DirectInput.h"
#include "InterfaceObject3d.h"

using namespace DirectX;
using namespace Microsoft::WRL;

const float PostEffect::clearColor[2][4] = {
	{ 0.0f,0.0f,0.0f,0.0f },
	{ 0.0f,0.0f,0.0f,0.0f }
};
GraphicsPipelineManager::GRAPHICS_PIPELINE PostEffect::pipeline;

PostEffect::PostEffect()
	:Sprite()
{
}

PostEffect::~PostEffect()
{
}

void PostEffect::Finalize()
{
	descHeapRTV.Reset();
	descHeapDSV.Reset();
}

void PostEffect::StaticInitialize()
{
}

void PostEffect::Initialize()
{
	HRESULT result;

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VERTEX) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// 頂点データ
	VERTEX vertices
		[vertNum] = {
		{{ -1,-1,0.0f }, {0.0f,1.0f}}, // 左下
		{{ -1,+1,0.0f }, {0.0f,0.0f}}, // 左上
		{{ +1,-1,0.0f }, {1.0f,1.0f}}, // 右下
		{{ +1,+1,0.0f }, {1.0f,0.0f}}, // 右上
	};

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VERTEX) * 4;
	vbView.StrideInBytes = sizeof(VERTEX);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }

	//テクスチャバッファ生成用変数
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WindowApp::GetWindowWidth(), (UINT)WindowApp::GetWindowHeight(),
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//深度以外のテクスチャ枚数
	const int break_num = TEX_TYPE::SIZE - 1;

	for (int i = 0; i < break_num; i++)
	{
		// テクスチャ用バッファの生成
		texture[i] = std::make_unique<Texture>();

		const int clear_color_num = i != TEX_TYPE::NORMAL;

		result = device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, // テクスチャ用指定
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor[clear_color_num]),
			IID_PPV_ARGS(&texture[i]->texBuffer));
		if (FAILED(result)) { assert(0); }

		//テクスチャを赤クリア
		{
			//画素数
			const UINT pixel_count = WindowApp::GetWindowWidth() * WindowApp::GetWindowHeight();
			//画素一行分のデータサイズ
			const UINT row_pitch = sizeof(UINT) * WindowApp::GetWindowWidth();
			//画素全体のデータサイズ
			const UINT depth_pitch = row_pitch * WindowApp::GetWindowHeight();
			//画素イメージ
			UINT* img = new UINT[pixel_count];
			for (UINT j = 0; j < pixel_count; j++) { img[j] = 0xff0000ff; }

			// テクスチャバッファにデータ転送
			result = texture[i]->texBuffer->WriteToSubresource(0, nullptr,
				img, row_pitch, depth_pitch);
			if (FAILED(result)) { assert(0); }
			delete[] img;
		}
	}

	//SRV設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < break_num; i++)
	{
		//デスクリプタヒープにSRV生成
		texture[i]->descriptor = std::make_unique<DescriptorHeapManager>();
		texture[i]->descriptor->CreateSRV(texture[i]->texBuffer, srvDesc);
	}

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHescDesc{};
	rtvDescHescDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHescDesc.NumDescriptors = break_num;

	//RTV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(&rtvDescHescDesc, IID_PPV_ARGS(&descHeapRTV));
	if (FAILED(result)) { assert(0); }

	//デスクリプタヒープにRTV生成
	for (int i = 0; i < break_num; i++)
	{
		device->CreateRenderTargetView(texture[i]->texBuffer.Get(), nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}
}

void PostEffect::CreateDepthBuffer()
{
	HRESULT result;

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32_TYPELESS,
			WindowApp::GetWindowWidth(),
			(UINT)WindowApp::GetWindowHeight(),
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	texture[TEX_TYPE::DEPTH] = std::make_unique<Texture>();
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&texture[TEX_TYPE::DEPTH]->texBuffer));
	if (FAILED(result)) { assert(0); }

	//DSV用デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デプスステンシルビュー
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは1つ

	//デスクリプタヒープにDSV生成
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	if (FAILED(result)) { assert(0); }

	//深度バッファリソース設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(texture[TEX_TYPE::DEPTH]->texBuffer.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV生成
	texture[TEX_TYPE::DEPTH]->descriptor = std::make_unique<DescriptorHeapManager>();
	texture[TEX_TYPE::DEPTH]->descriptor->CreateSRV(texture[TEX_TYPE::DEPTH]->texBuffer, srvDesc);
}

std::unique_ptr<PostEffect> PostEffect::Create()
{
	// Spriteのインスタンスを生成
	PostEffect* instance = new PostEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();
	//深度バッファ生成
	instance->CreateDepthBuffer();

	//ユニークポインタを返す
	return std::unique_ptr<PostEffect>(instance);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* _cmdList)
{
	// 定数バッファへデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->outlineColor = InterfaceObject3d::GetOutlineColor();
	constMap->outlineWidth = InterfaceObject3d::GetOutlineWidth();
	constMap->isFog = isFog;
	constBuff->Unmap(0, nullptr);

	// パイプラインステートの設定
	_cmdList->SetPipelineState(pipeline.pipelineState.Get());

	// ルートシグネチャの設定
	_cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	//プリミティブ形状を設定
	_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点バッファの設定
	_cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// 定数バッファビューをセット
	_cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビュー
	for (int i = 0; i < TEX_TYPE::SIZE; i++)
	{
		_cmdList->SetGraphicsRootDescriptorTable(i + 1, texture[i]->descriptor->gpu);
	}

	// 描画コマンド
	_cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	const int break_num = TEX_TYPE::SIZE - 1;

	//リソースバリアを変更
	for (int i = 0; i < break_num; i++)
	{
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texture[i]->texBuffer.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}

	//レンダーターゲットビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHs[break_num];
	for (int i = 0; i < break_num; i++)
	{
		if (break_num == i) { break; }
		rtvHs[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}

	//深度ステンシルビュー用デスクリプタヒープのハンドル取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(break_num, rtvHs, false, &dsvH);

	//ビューポート設定
	CD3DX12_VIEWPORT viewports[break_num];
	CD3DX12_RECT scissorRects[break_num];
	for (int i = 0; i < break_num; i++)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f, FLOAT(WindowApp::GetWindowWidth()), FLOAT(WindowApp::GetWindowHeight()));
		scissorRects[i] = CD3DX12_RECT(0, 0, LONG(WindowApp::GetWindowWidth()), LONG(WindowApp::GetWindowHeight()));
	}

	cmdList->RSSetViewports(1, viewports);
	//シザリング矩形設定
	cmdList->RSSetScissorRects(1, scissorRects);

	//全画面クリア
	for (int i = 0; i < break_num; i++)
	{
		int clearColorNum = i != TEX_TYPE::NORMAL;
		cmdList->ClearRenderTargetView(rtvHs[i], clearColor[clearColorNum], 0, nullptr);
	}

	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	const int break_num = TEX_TYPE::DEPTH - 1;

	//リソースバリアを変更
	for (int i = 0; i < break_num; i++)
	{
		cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[i]->texBuffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	}
}