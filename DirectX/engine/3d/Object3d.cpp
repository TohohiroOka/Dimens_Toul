#include "Object3d.h"

#include "BaseCollider.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "LightGroup.h"
#include "Model.h"
#include "Texture.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;

GraphicsPipelineManager::GRAPHICS_PIPELINE Object3d::pipeline;

std::unique_ptr<Object3d> Object3d::Create(Model* _model)
{
	// 3Dオブジェクトのインスタンスを生成
	Object3d* instance = new Object3d();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->Initialize();

	if (_model) {
		instance->SetModel(_model);
	}

	return std::unique_ptr<Object3d>(instance);
}

void Object3d::Initialize()
{
	// nullptrチェック
	assert(device);

	name = typeid(*this).name();

	InterfaceObject3d::Initialize();
}

void Object3d::PreDraw()
{
	// パイプラインステートの設定
	cmdList->SetPipelineState(pipeline.pipelineState.Get());

	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipeline.rootSignature.Get());

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3d::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Object3d::cmdList);

	// モデルの割り当てがなければ描画しない
	if (model == nullptr) {
		return;
	}

	InterfaceObject3d::Draw();

	// モデル描画
	model->Draw(cmdList);
}