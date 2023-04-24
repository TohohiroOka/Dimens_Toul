#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <DirectXMath.h>

class DirectInput final
{
public: //エイリアス
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //列挙
	//マウスボタン
	enum MOUSE_BUTTON
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_WHEEL
	};

private: //シングルトン化
	//コンストラクタを隠蔽
	DirectInput() = default;
	//デストラクタを隠蔽
	~DirectInput() = default;

public:
	//コピーコンストラクタを無効化
	DirectInput(const DirectInput& input) = delete;
	//代入演算子を無効化
	void operator = (const DirectInput& input) = delete;

public: //メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>入力</returns>
	static DirectInput* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name = "_keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(const BYTE& _keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name = "_keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(const BYTE& _keyNumber);

	/// <summary>
	/// キーを離したかチェック
	/// </summary>
	/// <param name = "_keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>離したか</returns>
	bool ReleaseKey(const BYTE& _keyNumber);

	/// <summary>
	/// マウスボタンの押下をチェック
	/// </summary>
	/// <param name = "_mouseButton">マウスボタン</param>
	/// <returns>押されているか</returns>
	bool PushMouseButton(int _mouseButton);

	/// <summary>
	/// マウスボタンのトリガーをチェック
	/// </summary>
	/// <param name = "_mouseButton">マウスボタン</param>
	/// <returns>トリガーか</returns>
	bool TriggerMouseButton(int _mouseButton);

	/// <summary>
	/// マウスボタンを離したかチェック
	/// </summary>
	/// <param name = "_mouseButton">マウスボタン</param>
	/// <returns>離したか</returns>
	bool ReleaseMouseButton(int _mouseButton);

	/// <summary>
	/// マウスポインターの座標を取得
	/// </summary>
	/// <returns>マウスポインターの座標</returns>
	DirectX::XMFLOAT2 GetMousePoint();

	/// <summary>
	/// 1フレームのマウスの移動量の取得
	/// </summary>
	/// <returns>1フレームのマウスの移動量</returns>
	DirectX::XMFLOAT2 GetMouseVelocity();

	/// <summary>
	/// 1フレームのマウスホイールの移動量の取得
	/// </summary>
	/// <returns>1フレームのマウスホイールの移動量</returns>
	float GetMouseWheelVelocity();

private: //メンバ変数
	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput;
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};
	//マウスのデバイス
	ComPtr<IDirectInputDevice8> devMouse;
	//マウスの状態
	DIMOUSESTATE mouse;
	//前回のマウスの状態
	DIMOUSESTATE mousePre;
	//マウスポインター
	POINT mousePoint;
};