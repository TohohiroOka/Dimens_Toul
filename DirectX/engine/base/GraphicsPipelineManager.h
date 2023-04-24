#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "ShaderManager.h"

#include <unordered_map>

class GraphicsPipelineManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://メンバenum

	struct GRAPHICS_PIPELINE
	{
		// ルートシグネチャ
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		// パイプラインステートオブジェクト
		ComPtr<ID3D12PipelineState> pipelineState = nullptr;
	};

	//ブレンドモード
	enum BLEND_MODE
	{
		NOBLEND,//デフォルト設定
		ALPHA,//α合成
		ADD,//加算合成
		SUB,//減算合成
		MULA,//乗算合成
		INVSRC,//反転合成
	};

	//頂点レイアウトメンバー
	enum INPUT_LAYOUT_NUMBER
	{
		POSITION,
		NORMAL,
		TEXCOORD_2D,
		TEXCOORD_3D,
		BONEINDICES,
		BONEWEIGHTS,
		SCALE,
		COLOR,
	};

	//パイプライン設定
	struct PEPELINE_DESC
	{
		//2D描画か
		bool object2d = false;
		//パーティクル描画か
		bool particl = false;
		//頂点シェーダー
		std::string vertShader = "null";
		//ピクセルシェーダー
		std::string pixelShader = "null";
		//ジオメトリシェーダー
		std::string geometryShader = "null";
		//レイアウト数
		int layoutNum = 0;
		//頂点レイアウト設定
		D3D12_INPUT_ELEMENT_DESC* inputLayout = nullptr;
		//ブレンドステート設定数
		int stateNum = 1;
		//ブレンドの種類
		BLEND_MODE blendMode = BLEND_MODE::ALPHA;
		//描画方法
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		//レンダーターゲット数
		int rtvNum = 1;
	};

	//ルートシグネチャ設定
	struct SIGNATURE_DESC
	{
		//2D描画か
		bool object2d = false;
		//マテリアルデータの有無
		bool materialData = true;
		//インスタンシング描画
		bool instanceDraw = false;
		//テクスチャ数
		int textureNum = 1;
		//ライト有無
		bool light = true;
		//キューブマップ有無
		bool cubemap = false;
	};

private://メンバ関数

	/// <summary>
	/// ブレンドモードの設定
	/// </summary>
	/// <param name="_mode">ブレンドの種類</param>
	/// <returns>ブレンド設定</returns>
	D3D12_RENDER_TARGET_BLEND_DESC CreateBlendDesc(const BLEND_MODE& _mode);

	/// <summary>
	/// パイプラインデスクの生成
	/// </summary>
	/// <param name="_pepelineDescSet">パイプライン設定</param>
	/// <returns>パイプラインデスク</returns>
	D3D12_GRAPHICS_PIPELINE_STATE_DESC CreatepelineDesc(const PEPELINE_DESC& _pepelineDescSet);

	/// <summary>
	/// ルートシグネチャの生成
	/// </summary>
	/// <param name="_signatureDescSet">ルートシグネチャ設定</param>
	void CreateRootSignature(const SIGNATURE_DESC& _signatureDescSet);

public://メンバ関数

	GraphicsPipelineManager();
	~GraphicsPipelineManager();

	/// <summary>
	/// デバイスのセット
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void SetDevice(ID3D12Device* _device) { GraphicsPipelineManager::device = _device; }

	/// <summary>
	/// パイプラインの生成
	/// enum OBJECT_KINDSの中から一を引数とする
	/// </summary>
	/// <param name="_name">パイプライン名</param>
	/// <param name="_pepelineDescSet">パイプラインの設定</param>
	/// <param name="_signatureDescSet">ルートシグネチャ設定</param>
	void CreatePipeline(const std::string& _name, const PEPELINE_DESC& _pepelineDescSet, const SIGNATURE_DESC& _signatureDescSet);

private://静的メンバ変数

	// デバイス
	static ID3D12Device* device;

private://メンバ変数

	//シェーダー
	std::unique_ptr<ShaderManager> shaderManager;

public://メンバ変数

	//名前の保持
	std::string name;
	//パイプライン保存配列
	std::unordered_map<std::string, GRAPHICS_PIPELINE> graphicsPipeline;

};