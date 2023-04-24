#include "WindowApp.h"
#include <imgui_impl_win32.h>

UINT WindowApp::windowWidth;
UINT WindowApp::windowHeight;
WNDCLASSEX WindowApp::winClass{};
HWND WindowApp::hwnd = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam);

LRESULT WindowApp::WindowProcdure(HWND _hwnd, UINT _msg, WPARAM _wparam, LPARAM _lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(_hwnd, _msg, _wparam, _lparam)) {
		return 1;
	}

	if (_msg == WM_DESTROY)
	{
		PostQuitMessage((0));
		return 0;
	}
	return DefWindowProc(_hwnd, _msg, _wparam, _lparam);
}

void WindowApp::Initialize(int _windowWidth, int _windowHeight,const wchar_t* _gameName)
{
	windowWidth = _windowWidth;
	windowHeight = _windowHeight;

	winClass.cbSize = sizeof(WNDCLASSEX);
	winClass.lpfnWndProc = (WNDPROC)WindowProcdure;// ウィンドウプロシージャを設定
	winClass.lpszClassName = _gameName;// ウィンドウクラス名
	winClass.hInstance = GetModuleHandle(nullptr);// ウィンドウハンドル
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);// カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&winClass);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, (LONG)windowWidth, (LONG)windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);// 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(winClass.lpszClassName,// クラス名
		_gameName,// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,// 標準的なウィンドウスタイル
		CW_USEDEFAULT,// 表示X座標（OSに任せる）
		CW_USEDEFAULT,// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,// ウィンドウ横幅
		wrc.bottom - wrc.top,// ウィンドウ縦幅
		nullptr,// 親ウィンドウハンドル
		nullptr,// メニューハンドル

		winClass.hInstance,// 呼び出しアプリケーションハンドル
		nullptr);// オプション

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

bool WindowApp::Update()
{
	//メッセージ処理
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//アプリケーションが終わる時にmessageがWM_QUITになる
	if (msg.message == WM_QUIT)
	{
		return true;
	}
	else { return false; }
}

void WindowApp::Release()
{
	UnregisterClass(winClass.lpszClassName, winClass.hInstance);
}