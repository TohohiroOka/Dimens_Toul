#pragma once
#include "InterfaceObject3d.h"

class BaseCollider;
class Camera;
class LightGroup;

class HeightMap : public InterfaceObject3d
{
private: // エイリアス

	struct HEIGHT_MAP_INFO {		// Heightmap structure
		int terrainWidth;		// Width of heightmap
		int terrainHeight;		// Height (Length) of heightmap
		std::vector<XMFLOAT3> heightMap;	// Array to store terrain's vertex positions
	};

	enum TEXTURE {
		HEIGHT_MAP_TEX,
		GRAPHIC_TEX_1,
		GRAPHIC_TEX_2,
		SIZE,
	};

	struct OBJECT_INFO
	{
		float ratio;
	};

public://メンバ関数

	/// <summary>
	/// directXCommon生成
	/// </summary>
	/// <param name="_heightmapFilename">heightmap名</param>
	/// <param name="_filename">ファイル名1</param>
	/// <param name="_filename2">ファイル名2</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<HeightMap> Create(const std::string& _heightmapFilename,
		const std::string& _filename1 = "null", const std::string& _filename2 = "null");

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

private://メンバ関数

	/// <summary>
	/// ハイトマップ
	/// </summary>
	/// <param name="_filename">ファイル名</param>
	/// <returns>成功か</returns>
	bool HeightMapLoad(const std::string& _filename);

	/// <summary
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="_filename1">ファイル名1</param>
	/// <param name="_filename2">ファイル名2</param>
	void LoadTexture(const std::string& _filename1, const std::string& _filename2);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

public:

	HeightMap() {};
	~HeightMap();

	/// <summary>
	/// 追加の定数バッファの更新
	/// </summary>
	void AddConstBufferUpdate(const float _ratio = 1.0f);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// パイプラインのセット
	/// </summary>
	/// <param name="_pipeline">パイプライン</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { HeightMap::pipeline = _pipeline; }

private:

	//パイプライン
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;
	//基礎ファイル
	static const std::string baseDirectory;

private:

	//テクスチャ情報
	std::array<std::unique_ptr<Texture>, TEXTURE::SIZE> texture;
	//定数バッファ
	ComPtr<ID3D12Resource> constBufferOData;
	// モデル
	Model* model = nullptr;
	//インデックスの大きさ
	int indexNum = 0;
	//頂点数
	int vertNum = 0;
	//ハイトマップの情報
	HEIGHT_MAP_INFO hmInfo;
	//当たり判定用頂点情報
	std::vector<Mesh::VERTEX> hitVertices;
	std::vector<unsigned long> hitIndices;

public:

	Model* GetModel() { return model; }
	std::vector<Mesh::VERTEX>& GetHitVertices() { return hitVertices; }
	std::vector<unsigned long>& GetHitIndices() { return hitIndices; }
};