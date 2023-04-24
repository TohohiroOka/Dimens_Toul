#include "DrawLine.h"
#include "WindowApp.h"
#include "DirectXCommon.h"
#include <cassert>
#include <DirectXTex.h>

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* DrawLine::device = nullptr;
ID3D12GraphicsCommandList* DrawLine::cmdList = nullptr;
std::unique_ptr<GraphicsPipelineManager> DrawLine::pipeline;
XMMATRIX DrawLine::matProjection;

const float PI = 3.141592f;

DrawLine::~DrawLine()
{
	vertBuff.Reset();
	constBuff.Reset();
}

void DrawLine::Finalize()
{
	pipeline.reset();
}

bool DrawLine::StaticInitialize(ID3D12Device* device)
{
	// 初期化チェック
	assert(!DrawLine::device);

	// nullptrチェック
	assert(device);

	DrawLine::device = device;

	// グラフィックパイプラインの生成
	CreateGraphicsPipeline();

	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)WindowApp::GetWindowWidth(),
		(float)WindowApp::GetWindowHeight(), 0.0f,
		0.0f, 1.0f);

	return true;
}

void DrawLine::CreateGraphicsPipeline()
{
	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	pipeline = GraphicsPipelineManager::Create("DrawLine2d",
		GraphicsPipelineManager::OBJECT_KINDS::DRAW_LINE_2D, inputLayout, _countof(inputLayout));
}

void DrawLine::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(DrawLine::cmdList == nullptr);

	DrawLine::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipeline->pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipeline->rootSignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DrawLine::PostDraw()
{
	// コマンドリストを解除
	DrawLine::cmdList = nullptr;
}

std::unique_ptr<DrawLine> DrawLine::Create()
{
	// Spriteのインスタンスを生成
	DrawLine* instance = new DrawLine();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	instance->Update();

	return std::unique_ptr<DrawLine>(instance);
}

bool DrawLine::Initialize()
{
	// nullptrチェック
	assert(device);

	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPos) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 頂点バッファへのデータ転送
	SetLine({}, {}, {}, 0);

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPos) * 4;
	vbView.StrideInBytes = sizeof(VertexPos);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->mat = matProjection;
		constBuff->Unmap(0, nullptr);
	}

	constBuff->SetName(L"texBuff");


	return true;
}

float DrawLine::GetAngle(XMFLOAT2 startPoint, XMFLOAT2 endPoint) {
	float angle = atan2f(endPoint.y - startPoint.y, endPoint.x - startPoint.x) * (180.0f / PI);
	return angle;
}

void DrawLine::SetLine(XMFLOAT2 startPoint, XMFLOAT2 endPoint, XMFLOAT4 color, float width)
{
	HRESULT result;

	this->color = color;

	//幅
	XMFLOAT2 lineWidth1 = {};
	XMFLOAT2 lineWidth2 = {};
	const float angle = GetAngle(startPoint, endPoint);
	lineWidth1.x = width * cosf((angle + 90.0f) * (PI / 180.0f));
	lineWidth1.y = width * sinf((angle + 90.0f) * (PI / 180.0f));
	lineWidth2.x = width * cosf((angle - 90.0f) * (PI / 180.0f));
	lineWidth2.y = width * sinf((angle - 90.0f) * (PI / 180.0f));

	// 頂点データ
	VertexPos vertices[vertNum];

	vertices[0].pos = { startPoint.x + lineWidth2.x, startPoint.y + lineWidth2.y, 0.0f }; // 左上
	vertices[1].pos = { endPoint.x + lineWidth2.x, endPoint.y + lineWidth2.y, 0.0f }; // 左下
	vertices[2].pos = { startPoint.x + lineWidth1.x, startPoint.y + lineWidth1.y, 0.0f }; // 右上
	vertices[3].pos = { endPoint.x + lineWidth1.x, endPoint.y + lineWidth1.y, 0.0f }; // 右下

	// 頂点バッファへのデータ転送
	VertexPos* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void DrawLine::Update()
{
	// ワールド行列の更新
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	this->matWorld *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	// 定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->color = this->color;
		constMap->mat = this->matWorld * matProjection;	// 行列の合成	
		this->constBuff->Unmap(0, nullptr);
	}
}

void DrawLine::Draw()
{
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}