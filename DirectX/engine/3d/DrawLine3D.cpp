#include "DrawLine3D.h"
#include "Camera.h"

#include <string>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* DrawLine3D::device = nullptr;
ID3D12GraphicsCommandList* DrawLine3D::cmdList = nullptr;
Camera* DrawLine3D::camera = nullptr;
GraphicsPipelineManager::GRAPHICS_PIPELINE DrawLine3D::pipeline;

const float PI = 3.141592f;

DrawLine3D::~DrawLine3D()
{
	//バッファを解放
	vertBuff.Reset();
	indexBuff.Reset();
	constBuff.Reset();
}

std::unique_ptr<DrawLine3D> DrawLine3D::Create(int _lineNum)
{
	DrawLine3D* instance = new DrawLine3D();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(_lineNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	instance->Update();

	return std::unique_ptr<DrawLine3D>(instance);
}

void DrawLine3D::StaticInitialize(ID3D12Device* _device)
{
	// 初期化チェック
	assert(!DrawLine3D::device);

	// nullptrチェック
	assert(_device);

	DrawLine3D::device = _device;
}

bool DrawLine3D::Initialize(int _lineNum)
{
	HRESULT result = S_FALSE;

	//頂点データの要素数
	vertexArrayNum = vertNum * _lineNum;
	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	const UINT sizeVB = static_cast<UINT>(sizeof(XMFLOAT3) * vertexArrayNum);
	//頂点データの要素数変更
	vertices.resize(vertexArrayNum);

	//頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// 頂点バッファへのデータ転送
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの生成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(XMFLOAT3);

	//インデック配列の要素数
	indexArrayNum = indexNum * _lineNum;
	//インデックスデータ全体のサイズ
	const UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indexArrayNum);

	//使うインデックスの作成
	std::vector<unsigned short> indices;
	for (int i = 0; i < int(indexArrayNum); i++)
	{
		//仮インデックス
		unsigned short addIndex;

		//ベース + 一本で使うインデックス番号の最大値 * (現在の配列数 / 一本分の配列の最大数)
		addIndex = BASE_INDICES[i % indexNum] + 4 * int(i / indexNum);

		//配列の一番後ろに入れる
		indices.push_back(addIndex);
	}

	//インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), //アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	//インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	std::copy(indices.begin(), indices.end(), indexMap);
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

float DrawLine3D::GetAngle(const XMFLOAT3& _startPoint, const XMFLOAT3& _endPoint)
{
	float angle = atan2f(_endPoint.y - _startPoint.y, _endPoint.x - _startPoint.x) * (180.0f / PI);
	return angle;
}

void DrawLine3D::SetLine(XMFLOAT3 _startPoint[], XMFLOAT3 _endPoint[], float _width)
{
	HRESULT result;

	//幅
	XMFLOAT2 lineWidth1 = {};
	XMFLOAT2 lineWidth2 = {};

	//線の本数分回す
	const int LineNum = vertexArrayNum / vertNum;
	for (int i = 0; i < LineNum; i++)
	{
		//角度
		const float angle = GetAngle(_startPoint[i], _endPoint[i]);

		//幅調整用値
		float LEFT = (angle + 90.0f) * (PI / 180.0f);
		float RIGHT = (angle - 90.0f) * (PI / 180.0f);

		lineWidth1.x = _width * cosf(LEFT);
		lineWidth1.y = _width * sinf(LEFT);
		lineWidth2.x = _width * cosf(RIGHT);
		lineWidth2.y = _width * sinf(RIGHT);

		// 頂点データ
		int arrayNum = i * 4;
		vertices[arrayNum] = { _endPoint[i].x + lineWidth2.x, _endPoint[i].y + lineWidth2.y, _endPoint[i].z }; // 左下
		arrayNum++;
		vertices[arrayNum] = { _startPoint[i].x + lineWidth2.x, _startPoint[i].y + lineWidth2.y, _startPoint[i].z }; // 左上
		arrayNum++;
		vertices[arrayNum] = { _endPoint[i].x + lineWidth1.x, _endPoint[i].y + lineWidth1.y, _endPoint[i].z }; // 右下
		arrayNum++;
		vertices[arrayNum] = { _startPoint[i].x + lineWidth1.x, _startPoint[i].y + lineWidth1.y, _startPoint[i].z }; // 右上
	}

	// 頂点バッファへのデータ転送
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void DrawLine3D::PreDraw(ID3D12GraphicsCommandList* _cmdList)
{
	DrawLine3D::cmdList = _cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipeline.pipelineState.Get());

	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	//プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void DrawLine3D::PostDraw()
{
	// コマンドリストを解除
	DrawLine3D::cmdList = nullptr;
}

void DrawLine3D::Update()
{
	//ワールド行列変換
	XMMATRIX matWorld = XMMatrixIdentity();
	XMMATRIX matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	matWorld *= matScale;

	XMMATRIX matRot;//角度
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	matRot *= XMMatrixRotationY(XMConvertToRadians(0.0f));
	matWorld *= matRot;

	XMMATRIX matTrans;//平行方向
	matTrans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	matWorld *= matTrans;

	//定数バッファにデータを転送
	CONST_BUFFER_DATA* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);//マッピング
	if (SUCCEEDED(result)) {
		constMap->color = color;
		constMap->matWorld = matWorld;
		if (camera != nullptr)
		{
			const XMMATRIX& matViewProjection = camera->GetView() * camera->GetProjection();
			constMap->viewproj = matViewProjection;
		} else {
			constMap->viewproj = XMMatrixIdentity();
		}
	}
	constBuff->Unmap(0, nullptr);
}

void DrawLine3D::Draw()
{
	//インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//描画コマンド
	cmdList->DrawIndexedInstanced(indexArrayNum, 1, 0, 0, 0);
}