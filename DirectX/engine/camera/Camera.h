#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>

/// <summary>
/// カメラ基本機能
/// </summary>
class Camera
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://静的メンバ関数

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<Camera> Create();

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Camera() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラのシェイク開始
	/// </summary>
	/// <param name="_strength">シェイクの強さ</param>
	void StartCameraShake(int _strength);

	/// <summary>
	/// カメラのシェイク終了
	/// </summary>
	void EndCameraShake();

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	/// <returns>ビュー行列</returns>
	inline const XMMATRIX& GetView() { return matView; }

	/// <summary>
	/// 射影行列の取得
	/// </summary>
	/// <returns>射影行列</returns>
	inline const XMMATRIX& GetProjection() { return matProjection; }

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const XMFLOAT3& GetEye() { return eye; }

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	inline const XMFLOAT3& GetTarget() { return target; }

	/// <summary>
	/// カメラの傾きの取得
	/// </summary>
	/// <returns>上方向ベクトル</returns>
	inline const XMFLOAT3& GetUp() { return up; }

	/// <summary>
	/// 視点座標セット
	/// </summary>
	/// <param name="eye">視点座標</param>
	void SetEye(const XMFLOAT3& _eye) { this->eye = _eye; }

	/// <summary>
	/// 注視点座標セット
	/// </summary>
	/// <param name="position">注視点座標</param>
	void SetTarget(const XMFLOAT3& _target) { this->target = _target; }

	/// <summary>
	/// カメラの傾きのセット
	/// </summary>
	/// <param name="_up">カメラの傾き</param>
	void SetUp(const XMFLOAT3& _up) { up = _up; }

	/// <summary>
	/// 射影行列の再設定
	/// </summary>
	/// <param name="_back">最奥</param>
	void SetMatProjection(float _back);

protected: // メンバ変数
	// ビュー行列
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// 視点座標
	XMFLOAT3 eye = { 0, 0, -20 };
	// 注視点座標
	XMFLOAT3 target = { 0, 0, 0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	// アスペクト比
	float aspectRatio = 1.0f;
	//シェイク座標
	XMFLOAT3 ShakeDifference = {};
};