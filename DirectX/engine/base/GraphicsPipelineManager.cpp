#include "GraphicsPipelineManager.h"
#include "SafeDelete.h"

#include <string>

using namespace Microsoft::WRL;

ID3D12Device* GraphicsPipelineManager::device = nullptr;

//名前を結合する
LPCWSTR GetName(std::string _className, std::string _setName)
{
	//名前の結合
	std::string name = _className + _setName;
	//大きさ取得
	size_t size = name.size();
	//名前のサイズ+1の配列に作り直す
	LPWSTR returnName = new wchar_t[size + 1];
	std::copy(name.begin(), name.end(), returnName);
	//配列の末に0を代入
	returnName[size] = 0;

	return returnName;
}

D3D12_RENDER_TARGET_BLEND_DESC GraphicsPipelineManager::CreateBlendDesc(const BLEND_MODE& _mode)
{
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc;
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA全てのチャンネルを描画
	if (_mode == BLEND_MODE::NOBLEND)
	{
		blenddesc.BlendEnable = false;
		return blenddesc;
	}

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	if (_mode == BLEND_MODE::ALPHA)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	} else if (_mode == BLEND_MODE::ADD)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
	} else if (_mode == BLEND_MODE::SUB)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_SUBTRACT;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
	} else if (_mode == BLEND_MODE::MULA)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ZERO;
		blenddesc.DestBlend = D3D12_BLEND_INV_DEST_COLOR;
	} else if (_mode == BLEND_MODE::INVSRC)
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
	}
	blenddesc.LogicOpEnable = false;

	return blenddesc;
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC GraphicsPipelineManager::CreatepelineDesc(const PEPELINE_DESC& _pepelineDescSet)
{
	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//シェーダーのセット
	if (_pepelineDescSet.vertShader != "null")
	{
		gpipeline.VS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectVS[_pepelineDescSet.vertShader].Get());
	}
	if (_pepelineDescSet.pixelShader != "null")
	{
		gpipeline.PS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectPS[_pepelineDescSet.pixelShader].Get());
	}
	if (_pepelineDescSet.geometryShader != "null")
	{
		gpipeline.GS = CD3DX12_SHADER_BYTECODE(shaderManager->shaderObjectGS[_pepelineDescSet.geometryShader].Get());
	}

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	//ブレンドモード設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc = CreateBlendDesc(_pepelineDescSet.blendMode);
	for (int i = 0; i < _pepelineDescSet.stateNum; i++)
	{
		gpipeline.BlendState.RenderTarget[i] = blenddesc;
	}

	gpipeline.NumRenderTargets = _pepelineDescSet.rtvNum;    // 描画対象は1つ
	for (int i = 0; i < _pepelineDescSet.rtvNum; i++)
	{
		gpipeline.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	}
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 図形の形状設定
	gpipeline.PrimitiveTopologyType = _pepelineDescSet.topologyType;

	//2D描画なら上書きモードに設定
	if (_pepelineDescSet.object2d)
	{
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

		if (_pepelineDescSet.particl)
		{
			gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
		else
		{
			gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			gpipeline.DepthStencilState.DepthEnable = false;
		}
	}

	gpipeline.InputLayout.pInputElementDescs = _pepelineDescSet.inputLayout;
	gpipeline.InputLayout.NumElements = _pepelineDescSet.layoutNum;

	return gpipeline;
}

void GraphicsPipelineManager::CreateRootSignature(const SIGNATURE_DESC& _signatureDescSet)
{
	HRESULT result = S_FALSE;

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc;

	// ルートパラメータ
	const int rootparam_num = 1 + (_signatureDescSet.materialData + _signatureDescSet.light +
		_signatureDescSet.instanceDraw) + _signatureDescSet.textureNum;

	std::vector<CD3DX12_ROOT_PARAMETER> rootparams(rootparam_num);
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);

	//2d描画
	if (_signatureDescSet.object2d)
	{
		// デスクリプタレンジ
		const int tex_num = _signatureDescSet.textureNum;
		CD3DX12_DESCRIPTOR_RANGE* descRangeSRV = new CD3DX12_DESCRIPTOR_RANGE[tex_num];

		for (int i = 0; i < tex_num; i++)
		{
			// デスクリプタレンジ
			descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i); // t0 レジスタ


			// SRV（テクスチャ）
			int paramNum = 1 + i;
			rootparams[paramNum].InitAsDescriptorTable(1, &descRangeSRV[i], D3D12_SHADER_VISIBILITY_ALL);
		}

		//サンプラー設定
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	}
	//3d描画
	else
	{
		int rootNum = 1;
		if (_signatureDescSet.materialData)
		{
			// CBV（マテリアルデータ用）
			rootparams[rootNum].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
			rootNum++;
		}
		if (_signatureDescSet.light)
		{
			// CBV (ライト)
			rootparams[rootNum].InitAsConstantBufferView(2, 0, D3D12_SHADER_VISIBILITY_ALL);
			rootNum++;
		}
		if (_signatureDescSet.instanceDraw)
		{
			// インスタンシング描画用情報
			rootparams[rootNum].InitAsConstantBufferView(3, 0, D3D12_SHADER_VISIBILITY_ALL);
			rootNum++;
		}

		// デスクリプタレンジ
		const int tex_num = _signatureDescSet.textureNum;
		CD3DX12_DESCRIPTOR_RANGE* descRangeSRV = new CD3DX12_DESCRIPTOR_RANGE[tex_num];

		for (int i = 0; i < tex_num; i++)
		{
			// デスクリプタレンジ
			descRangeSRV[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i); // t0 レジスタ

			// SRV（テクスチャ）
			int paramNum = rootNum + i;
			rootparams[paramNum].InitAsDescriptorTable(1, &descRangeSRV[i], D3D12_SHADER_VISIBILITY_ALL);
		}

		//サンプラー設定
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR,
			D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, 1);
	}

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0((UINT)rootparams.size(), rootparams.data(), 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, nullptr);
	if (FAILED(result)) { assert(0); }

	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&graphicsPipeline[name].rootSignature));
	if (FAILED(result)) { assert(0); }
}

GraphicsPipelineManager::GraphicsPipelineManager()
{
	//シェーダーのコンパイル
	shaderManager = ShaderManager::Create();
}

GraphicsPipelineManager::~GraphicsPipelineManager()
{
	for (auto& pipeline : graphicsPipeline)
	{
		pipeline.second.pipelineState.Reset();
		pipeline.second.rootSignature.Reset();
	}
	shaderManager.reset();
}

void GraphicsPipelineManager::CreatePipeline(const std::string& _name, const PEPELINE_DESC& _pepelineDescSet,
	const SIGNATURE_DESC& _signatureDescSet)
{
	HRESULT result = S_FALSE;

	this->name = _name;

	//同じキーがあればエラーを出力
	assert(!graphicsPipeline.count(name));
	size_t L_size = graphicsPipeline.size() + 1;
	graphicsPipeline.reserve(L_size);

	//グラフィックスパイプラインの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC L_gpipeline = CreatepelineDesc(_pepelineDescSet);

	//ルートシグネチャの生成
	CreateRootSignature(_signatureDescSet);

	//パイプラインデスクにルートシグネチャを登録
	L_gpipeline.pRootSignature = graphicsPipeline[name].rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&L_gpipeline, IID_PPV_ARGS(&graphicsPipeline[name].pipelineState));
	if (FAILED(result)) { assert(0); }

	graphicsPipeline[name].pipelineState->SetName(GetName(name, "PipelineState"));
	graphicsPipeline[name].rootSignature->SetName(GetName(name, "RootSignature"));
}