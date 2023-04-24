#include "ParticleManager.h"
#include <DirectXTex.h>
#include"Camera.h"

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* ParticleManager::device = nullptr;
ID3D12GraphicsCommandList* ParticleManager::cmdList = nullptr;
Camera* ParticleManager::camera = nullptr;
GraphicsPipelineManager::GRAPHICS_PIPELINE ParticleManager::pipeline;
std::map<std::string, ParticleManager::INFORMATION> ParticleManager::texture;
XMMATRIX ParticleManager::matBillboard = XMMatrixIdentity();
XMMATRIX ParticleManager::matBillboardY = XMMatrixIdentity();

ParticleManager::~ParticleManager()
{
	vertBuff.Reset();
	constBuff.Reset();
}

//XMFLOAT3同士の加算処理
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& _lhs, const DirectX::XMFLOAT3& _rhs)
{
	XMFLOAT3 result;
	result.x = _lhs.x + _rhs.x;
	result.y = _lhs.y + _rhs.y;
	result.z = _lhs.z + _rhs.z;

	return result;
}

void ParticleManager::LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete)
{
	// nullptrチェック
	assert(device);

	//同じキーがあればエラーを出力
	assert(!texture.count(_keepName));

	//テクスチャ読み込み
	texture[_keepName].instance = Texture::Create(_filename);
	texture[_keepName].isDelete = _isDelete;
}

void ParticleManager::Initialize()
{
	assert(pipeline.pipelineState);
	assert(pipeline.rootSignature);

	HRESULT result = S_FALSE;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) { assert(0); }

	// 頂点バッファへのデータ転送
	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONST_BUFFER_DATA) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	if (FAILED(result)) { assert(0); }
}

std::unique_ptr<ParticleManager> ParticleManager::Create(const std::string& _name)
{
	// 3Dオブジェクトのインスタンスを生成
	ParticleManager* instance = new ParticleManager();

	instance->name = _name;

	// 初期化
	instance->Initialize();

	return std::unique_ptr<ParticleManager>(instance);
}

void ParticleManager::Add(int _maxFrame, const XMFLOAT3& _position, const XMFLOAT3& _velocity,
	const XMFLOAT3& _accel, float _startScale, float _endScale, const XMFLOAT4& _startColor, const XMFLOAT4& _endColor)
{
	//リストに要素を追加
	particle.emplace_front();
	//追加した要素の参照
	PARTICLE& p = particle.front();
	//値のリセット
	p.position = _position;
	p.velocity = _velocity;
	p.accel = _accel;
	p.numFrame = _maxFrame;
	p.scale = _startScale;
	p.startScale = _startScale;
	p.endScale = _endScale;
	p.color = _startColor;
	p.startColor = _startColor;
	p.endColor = _endColor;
}

XMMATRIX ParticleManager::UpdateViewMatrix()
{
	//注意点
	XMVECTOR eyePosition = XMLoadFloat3(&camera->GetEye());
	//注意点座標
	XMVECTOR targetPosition = XMLoadFloat3(&camera->GetTarget());
	//上方向
	XMVECTOR upVector = XMLoadFloat3(&camera->GetUp());

	//カメラZ軸
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3Equal(upVector, XMVectorZero()));

	//ベクトル正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸
	XMVECTOR cameraAxisX;
	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);
	//ベクトル正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのY軸
	XMVECTOR cameraAxisY;
	//Y軸はX軸→Z軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);
	//ベクトル正規化
	cameraAxisY = XMVector3Normalize(cameraAxisY);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	//カメラ座標系
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//転置
	XMMATRIX matView = XMMatrixTranspose(matCameraRot);

	//視点座標に-1を掛けた座標
	XMVECTOR reverssEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点への変換
	XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverssEyePosition);
	XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverssEyePosition);
	XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverssEyePosition);
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//ビュー変換に平行移動成分を設定
	matView.r[3] = translation;

#pragma region 線方向ビルボード行列の計算
	//ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y軸回りのビルボード行列の計算
	//カメラX軸,Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のY軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(cameraAxisY, cameraAxisX);
	;
	//Y軸回りビルボード行列
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

	return matView;
}

void ParticleManager::Update()
{
	HRESULT result;

	//表示時間をが過ぎたパーティクルを削除
	particle.remove_if([](PARTICLE& x) {
		return x.frame >= x.numFrame;
		}
	);

	//全パーティクル更新
	for (std::forward_list<PARTICLE>::iterator it = particle.begin();
		it != particle.end(); it++) {
		//経過フレーム数をカウント
		it->frame++;
		//速度に加速度を加算
		it->velocity = it->velocity + it->accel;
		//速度による移動
		it->position = it->position + it->velocity;
		//大きさの変更
		it->scale = it->scale - (it->startScale - it->endScale) / it->numFrame;
		//色の変更
		it->color.x = it->color.x - (it->startColor.x - it->endColor.x) / it->numFrame;//赤
		it->color.y = it->color.y - (it->startColor.y - it->endColor.y) / it->numFrame;//緑
		it->color.z = it->color.z - (it->startColor.z - it->endColor.z) / it->numFrame;//青
		it->color.w = it->color.w - (it->startColor.w - it->endColor.w) / it->numFrame;//明度
	}

	VERTEX* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	int count = 0;
	if (SUCCEEDED(result)) {
		//パーティクルの情報を一つずつ反映
		for (std::forward_list<PARTICLE>::iterator it = particle.begin();
			it != particle.end(); it++) {
			if (count >= vertexCount) { break; }
			//座標
			vertMap->pos = it->position;
			//スケール
			vertMap->scale = it->scale;
			//色
			vertMap->color = it->color;
			//次の頂点へ
			vertMap++;
			count++;
		}
	}

	// 定数バッファへデータ転送
	CONST_BUFFER_DATA* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = UpdateViewMatrix() * camera->GetProjection();// 行列の合成
	constMap->matBillboard = matBillboard;// 行列の合成
	constMap->isBloom = isBloom;
	constBuff->Unmap(0, nullptr);
}

void ParticleManager::PreDraw(ID3D12GraphicsCommandList* _cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ParticleManager::cmdList == nullptr);

	ParticleManager::cmdList = _cmdList;

	// パイプラインステートの設定
	_cmdList->SetPipelineState(pipeline.pipelineState.Get());
	// ルートシグネチャの設定
	_cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	//プリミティブ形状の設定コマンド
	_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void ParticleManager::PostDraw()
{
	// コマンドリストを解除
	ParticleManager::cmdList = nullptr;
}

void ParticleManager::Draw()
{
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, texture[name].instance->descriptor->gpu);

	//描画コマンド
	cmdList->DrawInstanced((UINT)std::distance(particle.begin(), particle.end()), 1, 0, 0);
}

void ParticleManager::ParticlAllDelete()
{
	particle.clear();
}

void ParticleManager::SceneFinalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr) {
		if ((*itr).second.isDelete)
		{
			(*itr).second.instance.reset();
			auto deleteItr = itr;
			itr--;
			texture.erase(deleteItr);
			if (itr == texture.end()) { break; }
		}
	}
}

void ParticleManager::Finalize()
{
	for (auto itr = texture.begin(); itr != texture.end(); ++itr)
	{
		(*itr).second.instance.reset();
	}
	texture.clear();
}