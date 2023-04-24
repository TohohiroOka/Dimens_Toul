#include "WindowApp.h"
#include "MainEngine.h"
#include "SafeDelete.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ウィンドウサイズ
	const int window_width = 1280;// 横幅
	const int window_height = 720;// 縦幅

	//ゲーム名
	const wchar_t* gameName = L"DirectX";

	//ウィンドウ初期化
	WindowApp* winApp = new WindowApp();
	winApp->Initialize(window_width, window_height, gameName);

	//全体の初期化
	MainEngine* engine = new MainEngine();
	engine->Initialize();

	while (true)
	{
		if (engine->Update()|| winApp->Update()) { break; }
		engine->Draw();

		//フレームレート管理
		engine->FrameControl();
	}

	//登録解除
	safe_delete(engine);
	winApp->Release();

	return 0;
}