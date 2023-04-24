#include "ShaderManager.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

const std::wstring pass = L"Resources/Shaders/";

//エラーの出力
void OutputErrstr(ID3DBlob* _errorBlob)
{
	// errorBlobからエラー内容をstring型にコピー
	std::string errstr;
	errstr.resize(_errorBlob->GetBufferSize());

	std::copy_n((char*)_errorBlob->GetBufferPointer(),
		_errorBlob->GetBufferSize(),
		errstr.begin());
	errstr += "\n";
	// エラー内容を出力ウィンドウに表示
	OutputDebugStringA(errstr.c_str());
	exit(1);
}

//パスの取得
std::wstring GetFullPath(LPCWSTR _assetName)
{
	return pass + _assetName;
}

//シェーダーの読み込みとコンパイル
ID3DBlob* CompileShader(const LPCWSTR _fileName, LPCSTR _target)
{
	HRESULT result = S_FALSE;
	//出力用オブジェクト
	ID3DBlob* blob = nullptr;
	//インクルード設定
	ID3DInclude* includeFrag = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	// デバッグ用設定
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	//エラーオブジェクト
	ID3DBlob* error;

	result = D3DCompileFromFile(GetFullPath(_fileName).c_str(), nullptr, includeFrag, "main", _target,
		compileFlags, 0, &blob, &error);
	if (FAILED(result))
	{
		OutputErrstr(error);
	}

	return blob;
}

std::unique_ptr<ShaderManager> ShaderManager::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	ShaderManager* instance = new ShaderManager();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	instance->LoadShader();

	return std::unique_ptr<ShaderManager>(instance);
}

void ShaderManager::LoadShader()
{
	HRESULT result = S_FALSE;

	//頂点シェーダーモデル
	LPCSTR vsModel = "vs_5_0";
	//ピクセルシェーダーモデル
	LPCSTR psModel = "ps_5_0";
	//ジオメトリシェーダーモデル
	LPCSTR gsModel = "gs_5_0";
	//コンピュートシェーダーモデル
	LPCSTR csModel = "cs_5_0";

	//Obj
	shaderObjectVS["OBJ"] = CompileShader(L"ObjVS.hlsl", vsModel);
	shaderObjectPS["OBJ"] = CompileShader(L"ObjPS.hlsl", psModel);
	//HeightMap
	shaderObjectVS["InstanceObject"] = CompileShader(L"InstanceObjectVS.hlsl", vsModel);
	shaderObjectPS["InstanceObject"] = CompileShader(L"InstanceObjectPS.hlsl", psModel);
	//Fbx
	shaderObjectVS["FBX"] = CompileShader(L"FbxVS.hlsl", vsModel);
	shaderObjectPS["FBX"] = CompileShader(L"FbxPS.hlsl", psModel);
	//DrawLine3d
	shaderObjectVS["DRAW_LINE_3D"] = CompileShader(L"DrawLine3DVS.hlsl", vsModel);
	shaderObjectPS["DRAW_LINE_3D"] = CompileShader(L"DrawLine3DPS.hlsl", psModel);
	//Sprite
	shaderObjectVS["SPRITE"] = CompileShader(L"SpriteVS.hlsl", vsModel);
	shaderObjectPS["SPRITE"] = CompileShader(L"SpritePS.hlsl", psModel);
	//DrawLine2d
	shaderObjectVS["DRAW_LINE_2D"] = CompileShader(L"DrawLine2DVS.hlsl", vsModel);
	shaderObjectPS["DRAW_LINE_2D"] = CompileShader(L"DrawLine2DPS.hlsl", psModel);
	//PostEffect
	shaderObjectVS["POST_EFFECT"] = CompileShader(L"PostEffectVS.hlsl", vsModel);
	shaderObjectPS["POST_EFFECT"] = CompileShader(L"PostEffectPS.hlsl", psModel);
	//Particle
	shaderObjectVS["PARTICLE"] = CompileShader(L"ParticleVS.hlsl", vsModel);
	shaderObjectPS["PARTICLE"] = CompileShader(L"ParticlePS.hlsl", psModel);
	shaderObjectGS["PARTICLE"] = CompileShader(L"ParticleGS.hlsl", gsModel);
	//CubeBox
	shaderObjectVS["CUBE_BOX"] = CompileShader(L"CubeBoxVS.hlsl", vsModel);
	shaderObjectPS["CUBE_BOX"] = CompileShader(L"CubeBoxPS.hlsl", psModel);
	//HeightMap
	shaderObjectVS["HEIGHT_MAP"] = CompileShader(L"HeightMapVS.hlsl", vsModel);
	shaderObjectPS["HEIGHT_MAP"] = CompileShader(L"HeightMapPS.hlsl", psModel);
}

void ShaderManager::Finalize()
{
	for (auto shader : shaderObjectVS)
	{
		shader.second.Reset();
	}
	shaderObjectVS.clear();

	for (auto shader : shaderObjectPS)
	{
		shader.second.Reset();
	}
	shaderObjectPS.clear();

	for (auto shader : shaderObjectGS)
	{
		shader.second.Reset();
	}
	shaderObjectGS.clear();

	for (auto shader : shaderObjectCS)
	{
		shader.second.Reset();
	}
	shaderObjectCS.clear();
}