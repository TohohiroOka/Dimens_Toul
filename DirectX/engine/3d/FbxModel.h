#pragma once
#include <fbxsdk.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <map>
#include "Texture.h"

class FbxModel
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public://固定値

	//ボーンの最大値
	static const int MAX_BONES = 32;
	//テクスチャ最大登録数
	static const int textureNum = 256;

private://構造体宣言

	static const int MAX_BONE_INDICES = 4;

	//頂点データ3D
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		UINT boneIndex[MAX_BONE_INDICES];
		float boneWhight[MAX_BONE_INDICES];
	};

	//ノード
	struct Node
	{
		//名前
		std::string name;
		//ローカルスケール
		XMVECTOR scaling = { 1.0f,1.0f,1.0f,0.0f };
		//ローカル回転
		XMVECTOR rotation = { 0.0f,0.0f,0.0f,0.0f };
		//ローカル移動
		XMVECTOR translation = { 0.0f,0.0f,0.0f,1.0f };
		//ローカル変形行列
		XMMATRIX transform = {};
		//グローバル変形行列
		XMMATRIX globalTransform = {};
		//親ノード
		Node* parent = nullptr;
	};

	//ボーン
	struct Bone
	{
		//名前
		std::string name;

		//初期姿勢行列
		DirectX::XMMATRIX invInitialPose;

		//クラスター
		FbxCluster* fbxCluster;

		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

	//マテリアル用
	struct Material
	{
		std::string name;//マテリアル名
		XMFLOAT3 ambient = {};//アンビエント影響度
		XMFLOAT3 diffuse = {};//ディフューズ影響度
		float alpha = 0;//アルファ
		XMFLOAT3 baseColor = { 1,1,1 };//ベースカラ―
		float metalness = 0.0f;//金属度(0 or 1)
		float specular = 0.5f;//鏡面反射度
		float roughness = 0.0f;//粗さ
	};

	//スキン用定数バッファデータ
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};

	//アニメーションデータ
	struct FbxUpdate
	{
		bool isAnimation = false;//アニメーション可能か
		FbxScene* fbxScene = nullptr;
		FbxTime startTime = {};//フレームのスタート
		FbxTime stopTime = {};//フレームの最後
		FbxTime nowTime = {};//現在の進行フレーム
	};

	//Fbxデータ
	struct Data
	{
		Material material;
		std::vector<Vertex> vertices;
		std::vector<unsigned short>indices;
		std::vector<Node> nodes;
		Node* meshNode;
		std::vector<Bone> bones;
		FbxUpdate fbxUpdate;
	};

private://メンバ関数

	/// <summary>
	/// ノード読み込み
	/// </summary>
	/// <param name="parent">親ノード</param>
	void LoadNode(FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュを探す
	/// </summary>
	void CollectMesh(FbxNode* fbxNode);

	/// <summary>
	/// 頂点読み込み
	/// </summary>
	/// <param name="fbxMesh">メッシュ</param>
	void CollectVertices(FbxMesh* fbxMesh);

	/// <summary>
	/// 面ごとの読み込み
	/// </summary>
	/// <param name="fbxMesh">メッシュ</param>
	void CollectMeshFaces(FbxMesh* fbxMesh);

	/// <summary>
	/// スキニング情報の読み取り
	/// </summary>
	/// <param name="fbxMesh">メッシュ</param>
	void CollectSkin(FbxMesh* fbxMesh);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(FbxNode* fbxNode);

	/// <summary>
	/// アニメーション読み込み
	/// </summary>
	void LoadAnimation(FbxScene* fbxScene);

	/// <summary>
	/// Fbxファイルの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	void LoadFbx(const std::string modelName);

	/// <summary>
	/// 生成
	/// </summary>
	void Initialize();

	/// <summary>
	/// //行列の変換
	/// </summary>
	/// <param name="dst">格納するXMMATRIX型変数</param>
	/// <param name="src">変換するFbxMatrix型変数</param>
	void ConvertMatrixFormFbx(DirectX::XMMATRIX* dst, const FbxMatrix& src);

	/// <summary>
	/// //ファイル名抽出
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns></returns>
	std::string ExtractFileName(const std::string& path);

public://静的メンバ関数

	FbxModel() {};
	~FbxModel();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	static std::unique_ptr<FbxModel> Create(const std::string fileName);
	
	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

public:

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param
	void Draw(ID3D12GraphicsCommandList* cmdList);

private://静的メンバ変数

	//デバイス
	static ID3D12Device* device;
	//Fbxの基盤
	static FbxManager* fbxManager;
	//FBXインポータ
	static FbxImporter* fbxImporter;
	//1フレームの時間
	static FbxTime frameTime;
	//textureが無い時のtexture
	static const std::string defaultTexture;
	//ファイルパス
	static const std::string baseDirectory;
	//Fbxのデータ
	std::unique_ptr<Data> data = nullptr;

public://メンバ変数

	//モデル名
	std::string name;
	//テクスチャ情報
	std::unique_ptr<Texture> texture = nullptr;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuffSkin = nullptr;
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff = nullptr;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//インデックスバッファ
	ComPtr<ID3D12Resource> indexBuff = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView;
	//アニメーション可能か
	bool isAnimation = false;
	//スキニングを行うか
	bool isSkinning = true;

public:

	/// <summary>
	/// アンビエント影響度の取得
	/// </summary>
	/// <returns>アンビエント</returns>
	XMFLOAT3& GetAmbient() { return data->material.ambient; }

	/// <summary>
	/// ディフューズ影響度の取得
	/// </summary>
	/// <returns>ディフューズ</returns>
	XMFLOAT3& GetDiffuse() { return data->material.diffuse; }

	/// <summary>
	/// アルファの取得
	/// </summary>
	/// <returns>アルファ</returns>
	float& GetAlpha() { return data->material.alpha; }

	/// <summary>
	/// ベースカラ―の取得
	/// </summary>
	/// <returns>ベースカラ―</returns>
	XMFLOAT3& GetBaseColor() { return data->material.baseColor; }

	/// <summary>
	/// 金属度の取得
	/// </summary>
	/// <returns>金属度(0 or 1)</returns>
	float& GetMetalness() { return data->material.metalness; }

	/// <summary>
	/// 鏡面反射度の取得
	/// </summary>
	/// <returns>鏡面反射度</returns>
	float& GetSpecular() { return data->material.specular; }

	/// <summary>
	/// 粗さの取得
	/// </summary>
	/// <returns>粗さ</returns>
	float& GetRoughness() { return data->material.roughness; }
};