#pragma once
#include"ParticleManager.h"

class Camera;

class Emitter
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public://静的メンバ関数

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <param name="_name">テクスチャ名</param>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<Emitter> Create(const std::string& _name);

public://メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Emitter() {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Emitter() {};

	/// <summary>
	/// パーティクルの発生地点
	/// </summary>
	/// <param name="_maxFrame">生存時間</param>
	/// <param name="_position">初期座標</param>
	/// <param name="_velocity">速度</param>
	/// <param name="_accel">加速度</param>
	/// <param name="_startScale">初期サイズ</param>
	/// <param name="_endScale">最終サイズ</param>
	/// <param name="_startColor">初期カラー</param>
	/// <param name="_endColor">最終カラー</param>
	void InEmitter(int _maxFrame, const XMFLOAT3& _position, const XMFLOAT3& _velocity,
		const XMFLOAT3& _accel, float _startScale, float _endScale, const XMFLOAT4& _startColor, const XMFLOAT4& _endColor);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 削除
	/// </summary>
	void ParticlAllDelete();

private:
	//パーティクルクラス
	std::unique_ptr<ParticleManager> particleManager = nullptr;

public:

	/// <summary>
	/// ブルームセット
	/// </summary>
	void SetBloom() { particleManager->SetBloom(true); }

	/// <summary>
	/// 現在の数
	/// </summary>
	int GetCreateNum() {
		return particleManager->GetCreateNum();
	}

};