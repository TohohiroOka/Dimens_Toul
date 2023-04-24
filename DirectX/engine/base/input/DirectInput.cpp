#include "DirectInput.h"
#include "WindowApp.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

DirectInput* DirectInput::GetInstance()
{
	static DirectInput instance;

	return &instance;
}

void DirectInput::Initialize()
{
	HRESULT result;

	//DirectInputのインスタンス生成
	result = DirectInput8Create(
		WindowApp::GetWinInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	if (FAILED(result)) { assert(0); }

	///-----------------キー入力初期化-----------------///

	//キーボードデバイスの設定
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	if (FAILED(result)) { assert(0); }

	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	if (FAILED(result)) { assert(0); }

	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(
		WindowApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) { assert(0); }


	///----------------マウス入力初期化----------------///

	//マウスデバイスの設定
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	if (FAILED(result)) { assert(0); }

	//入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse);	//標準形式
	if (FAILED(result)) { assert(0); }

	//排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(
		WindowApp::GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) { assert(0); }
}

void DirectInput::Update()
{
	HRESULT result;

	///-----------------キー入力更新-----------------///
	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));

	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
	//if (FAILED(result)) { assert(0); }

	//全キーの入力状態を取得する
	result = devkeyboard->GetDeviceState(sizeof(key), key);
	//if (FAILED(result)) { assert(0); }

	///----------------マウス入力更新----------------///
	//前回のマウス入力を保存
	mousePre = mouse;

	//マウス情報の取得開始
	result = devMouse->Acquire();
	//if (FAILED(result)) { assert(0); }

	//マウスの入力状態を取得する
	result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
	//if (FAILED(result)) { assert(0); }

	//マウス座標を取得する
	GetCursorPos(&mousePoint);

	//スクリーン座標をクライアント座標に変換する
	ScreenToClient(WindowApp::GetHwnd(), &mousePoint);
}

bool DirectInput::PushKey(const BYTE& _keyNumber)
{
	//指定のキーを押していればtrueを返す
	if (key[_keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool DirectInput::TriggerKey(const BYTE& _keyNumber)
{
	//指定のキーを押した瞬間ならtrueを返す
	if (key[_keyNumber] && !keyPre[_keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool DirectInput::ReleaseKey(const BYTE& _keyNumber)
{
	//指定のキーを離した瞬間ならtrueを返す
	if (!key[_keyNumber] && keyPre[_keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool DirectInput::PushMouseButton(int _mouseButton)
{
	//指定のボタンを押していればtrueを返す
	if (mouse.rgbButtons[_mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool DirectInput::TriggerMouseButton(int _mouseButton)
{
	//指定のボタンを押した瞬間ならtrueを返す
	if (mouse.rgbButtons[_mouseButton] &&
		!mousePre.rgbButtons[_mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool DirectInput::ReleaseMouseButton(int _mouseButton)
{
	//指定のボタンを離した瞬間ならtrueを返す
	if (!mouse.rgbButtons[_mouseButton] &&
		mousePre.rgbButtons[_mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

DirectX::XMFLOAT2 DirectInput::GetMousePoint()
{
	return DirectX::XMFLOAT2(float(mousePoint.x), float(mousePoint.y));
}

DirectX::XMFLOAT2 DirectInput::GetMouseVelocity()
{
	return DirectX::XMFLOAT2(float(mouse.lX), float(mouse.lY));
}

float DirectInput::GetMouseWheelVelocity()
{
	float wheel = mouse.lZ / 120.0f;
	return wheel;
}