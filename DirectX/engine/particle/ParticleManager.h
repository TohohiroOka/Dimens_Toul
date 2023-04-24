#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include<forward_list>

#include "GraphicsPipelineManager.h"
#include "Texture.h"

class Camera;

class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	// 頂点データ構造体
	struct VERTEX
	{
		XMFLOAT3 pos; // xyz座標
		float scale;//スケール
		XMFLOAT4 color;
	};

	// 定数バッファ用データ構造体
	struct CONST_BUFFER_DATA
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;//ビルボード行列
		unsigned int isBloom;
	};

	//パーティクル一粒
	struct PARTICLE
	{
		//DirectX::を省略
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int numFrame = 0;
		//スケール
		float scale = 1.0f;
		//初期値
		float startScale = 1.0f;
		//最終値
		float endScale = 0.0f;
		//カラー
		XMFLOAT4 color = { 0,0,0,0 };
		//初期カラー
		XMFLOAT4 startColor = { 0,0,0,0 };
		//最終カラー
		XMFLOAT4 endColor = { 0,0,0,0 };
	};

	struct INFORMATION
	{
		bool isDelete = false; //シーン遷移で削除を行うか
		std::unique_ptr<Texture> instance = nullptr;
	};

private: // 定数

	static const int vertexCount = 512;// 頂点数
	std::forward_list<PARTICLE>particle;

public: // 静的メンバ関数

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="_keepName">保存名</param>
	/// <param name="_filename">ファイル名</param>
	/// <param name="_isDelete">シーン遷移で削除を行うか</param>
	static void LoadTexture(const std::string& _keepName, const std::string& _filename, bool _isDelete = true);

	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <param name="_name">テクスチャ名</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<ParticleManager> Create(const std::string& _name);

	/// <summary>
	/// デバイスのセット
	/// </summary>
	/// <param name="_device">デバイス</param>
	static void SetDevice(ID3D12Device* _device) {
		ParticleManager::device = _device;
	}

	/// <summary>
	/// カメラのセット
	/// </summary>
	/// <param name="_camera">カメラ</param>
	static void SetCamera(Camera* _camera) {
		ParticleManager::camera = _camera;
	}

	/// <summary>
	/// シーンごとの解放処理
	/// </summary>
	static void SceneFinalize();

	/// <summary>
	/// 解放処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// パイプラインのセット
	/// </summary>
	/// <param name="pipeline">パイプライン</param>
	static void SetPipeline(const GraphicsPipelineManager::GRAPHICS_PIPELINE& _pipeline) { pipeline = _pipeline; }

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//カメラ
	static Camera* camera;
	//パイプライン
	static GraphicsPipelineManager::GRAPHICS_PIPELINE pipeline;
	//テクスチャ情報
	static std::map<std::string, INFORMATION> texture;
	//ビルボード行列
	static XMMATRIX matBillboard;
	//Y軸回りのビルボード行列
	static XMMATRIX matBillboardY;

private:// 静的メンバ関数

	/// <summary>
	/// ビュー行列の更新
	/// </summary>
	/// <returns>ビュー行列</returns>
	static XMMATRIX UpdateViewMatrix();

public: // メンバ関数

	ParticleManager() {};
	~ParticleManager();

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	void Initialize();

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name="_maxFrame">生存時間</param>
	/// <param name="_position">初期座標</param>
	/// <param name="_velocity">速度</param>
	/// <param name="_accel">加速度</param>
	/// <param name="_startScale">初期サイズ</param>
	/// <param name="_endScale">最終サイズ</param>
	/// <param name="_startColor">初期カラー</param>
	/// <param name="_endColor">最終カラー</param>
	void Add(int _maxFrame, const XMFLOAT3& _position, const XMFLOAT3& _velocity, const XMFLOAT3& _accel,
		float _startScale, float _endScale, const XMFLOAT4& _startColor, const XMFLOAT4& _endColor);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="_cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* _cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 出現しているパーティクルを全て削除する
	/// </summary>
	void ParticlAllDelete();

	/// <summary>
	/// 現在の数
	/// </summary>
	int GetCreateNum() {
		return int(std::distance(particle.begin(), particle.end()));
	}

private: // メンバ変数

	//テクスチャ名
	std::string name;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff = {};
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// 頂点データ配列
	VERTEX vertices[vertexCount] = {};
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//ブルームの有無
	bool isBloom = false;

public:

	/// <summary>
	/// ブルームのセット
	/// </summary>
	/// <param name="isBloom">ブルーム有->true / 無->false</param>
	void SetBloom(bool isBloom) { this->isBloom = isBloom; }
};