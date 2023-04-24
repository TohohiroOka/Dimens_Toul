#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Mesh.h"

/// <summary>
/// モデルデータ
/// </summary>
class Model
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

private:
	static const std::string baseDirectory;

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void StaticInitialize(ID3D12Device* _device);

	/// <summary>
	/// OBJファイルからメッシュ生成
	/// </summary>
	/// <param name="_modelname">モデル名</param>
	/// <param name="_smoothing">エッジ平滑化フラグ</param>
	/// <returns>生成されたモデル</returns>
	static std::unique_ptr<Model> CreateFromOBJ(const std::string& _modelname, bool _smoothing = false);

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Model();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_modelname">モデル名</param>
	/// <param name="_smoothing">エッジ平滑化フラグ</param>
	void Initialize(const std::string& _modelname, bool _smoothing);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_cmdList">命令発行先コマンドリスト</param>
	/// <param name="_shaderResourceView">シェーダーリソースビュー番号</param>
	/// <param name="_instanceDrawNum">インスタンシング描画個数</param>
	void Draw(ID3D12GraphicsCommandList* _cmdList, const int _shaderResourceView = 3, const int _instanceDrawNum = 1);

	/// <summary>
	/// メッシュコンテナを取得
	/// </summary>
	/// <returns>メッシュコンテナ</returns>
	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="_cmdList">命令発行先コマンドリスト</param>
	void VIDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// メッシュコンテナを取得
	/// </summary>
	/// <returns>メッシュコンテナ</returns>
	inline void SetMeshes(Mesh* meshes) { this->meshes.push_back(meshes); }

private: // メンバ変数
	// 名前
	std::string name;
	// メッシュコンテナ
	std::vector<Mesh*> meshes;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials;
	// デフォルトマテリアル
	Material* defaultMaterial = nullptr;

private: // メンバ関数

	/// <summary>
	/// モデル読み込み
	/// </summary>
	/// <param name="_modelname">モデル名</param>
	/// <param name="_smoothing">エッジ平滑化フラグ</param>
	void LoadModel(const std::string& _modelname, bool _smoothing);

	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	/// <param name="_directoryPath">ファイルパス</param>
	/// <param name="_filename">ファイル名</param>
	void LoadMaterial(const std::string& _directoryPath, const std::string& _filename);

	/// <summary>
	/// マテリアル登録
	/// </summary>
	/// <param name="_material">マテリアル</param>
	void AddMaterial(Material* _material);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTextures();

};