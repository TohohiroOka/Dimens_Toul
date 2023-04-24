#pragma once
#include "DirectXCommon.h"
#include "SceneManager.h"
#include "DirectInput.h"
#include "XInputManager.h"
#include "PostEffect.h"
#include "FrameRateKeep.h"
#include "CubeMap.h"

#include <sstream>
#include <iomanip>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

class WindowApp;

class MainEngine
{
public:

	MainEngine() = default;
	~MainEngine();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <returns>ゲームを続けるか</returns>
	bool Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フレームレートキープ
	/// </summary>
	void FrameControl();

private:

	//DirectXCommonのインスタンス
	std::unique_ptr<DirectXCommon> dXCommon = nullptr;
	//Inputのインスタンス
	DirectInput* input = nullptr;
	//XInputManager
	XInputManager* xinput = nullptr;
	//GameSceneのインスタンス
	std::unique_ptr<SceneManager> scene = nullptr;
	//ポストエフェクトのインスタンス
	std::unique_ptr<PostEffect> postEffect = nullptr;
	//Fps固定用クラスのインスタンス
	std::unique_ptr<FrameRateKeep> fps = nullptr;
	//キューブマップ
	//std::unique_ptr<CubeMap> cubemap;
	//数字表示デバッグ用
	wchar_t str[256] = {};
};