#include "XInputManager.h"

//スティック制限
#define INPUT_DEADZONE  ( 0.41f * FLOAT(0x7FFF) )
//スティックの上限
const int STICK_MAX = 32767;

XInputManager::~XInputManager()
{
#pragma warning(disable:4995)
	XInputEnable(FALSE);
}

XInputManager* XInputManager::GetInstance()
{
	static XInputManager instance;

	return &instance;
}

void XInputManager::Initialize()
{
#pragma warning(disable:4995)
	XInputEnable(TRUE);
}

void XInputManager::Update()
{
	controllers.lastState = controllers.state;
	controllers.dwResult = XInputGetState(0, &controllers.state);
}

bool XInputManager::PushButton(const XInputManager::PUD_BUTTON& _button)
{
	bool check = false;
	if (_button == PUD_BUTTON::PAD_A &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_A)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_B &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_B)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_Y &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_X &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_X)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_LB &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_RB &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_LT &&
		controllers.state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_RT &&
		controllers.state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_START &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_START)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_BUCK &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_UP &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_DOWN &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_LEFT &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_RIGHT &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_LEFT_STICK_PUSH &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_RIGHT_STICK_PUSH &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		check = true;
	}
	return check;
}

bool XInputManager::TriggerButton(const PUD_BUTTON& _button)
{
	bool check = false;
	if (_button == PUD_BUTTON::PAD_A &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_A) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_B &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_B) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_Y &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_X &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_X) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_LB &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_RB &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_LT &&
		controllers.state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		controllers.lastState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_RT &&
		controllers.state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD &&
		controllers.lastState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_START &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_START &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_START) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_BUCK &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_UP &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_DOWN &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_LEFT &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) == false)
	{
		check = true;
	} else if (_button == PUD_BUTTON::PAD_RIGHT &&
		controllers.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT &&
		(controllers.lastState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) == false)
	{
		check = true;
	}
	return check;
}

bool XInputManager::LeftStickX(bool _leftRight)
{
	if (_leftRight == true && controllers.state.Gamepad.sThumbLX < -INPUT_DEADZONE)
	{
		return true;
	} else if (_leftRight == false && controllers.state.Gamepad.sThumbLX > INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::LeftStickY(bool _upDown)
{
	if (_upDown == true && controllers.state.Gamepad.sThumbLY > INPUT_DEADZONE)
	{
		return true;
	} else if (_upDown == false && controllers.state.Gamepad.sThumbLY < -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::RightStickX(bool _leftRight)
{
	if (_leftRight == true && controllers.state.Gamepad.sThumbRX < -INPUT_DEADZONE)
	{
		return true;
	} else if (_leftRight == false && controllers.state.Gamepad.sThumbRX > INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::RightStickY(bool _upDown)
{
	if (_upDown == true && controllers.state.Gamepad.sThumbRY > INPUT_DEADZONE)
	{
		return true;
	} else if (_upDown == false && controllers.state.Gamepad.sThumbRY < -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerLeftStickX(bool _leftRight)
{
	if (_leftRight == true && controllers.state.Gamepad.sThumbLX < -INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbLX >= -INPUT_DEADZONE)
	{
		return true;
	} if (_leftRight == false && controllers.state.Gamepad.sThumbLX > INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbLX <= INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerLeftStickY(bool _upDown)
{
	if (_upDown == true && controllers.state.Gamepad.sThumbLY > INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbLY <= INPUT_DEADZONE)
	{
		return true;
	} else if (_upDown == false && controllers.state.Gamepad.sThumbLY < -INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbLY >= -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerRightStickX(bool _leftRight)
{
	if (_leftRight == true && controllers.state.Gamepad.sThumbRX < -INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbRX >= -INPUT_DEADZONE)
	{
		return true;
	}if (_leftRight == false && controllers.state.Gamepad.sThumbRX > INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbRX <= INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

bool XInputManager::TriggerRightStickY(bool _upDown)
{
	if (_upDown == true && controllers.state.Gamepad.sThumbRY > INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbRY <= INPUT_DEADZONE)
	{
		return true;
	} else if (_upDown == false && controllers.state.Gamepad.sThumbRY < -INPUT_DEADZONE &&
		controllers.lastState.Gamepad.sThumbRY >= -INPUT_DEADZONE)
	{
		return true;
	} else { return false; }
}

DirectX::XMFLOAT2 XInputManager::GetPadLStickIncline()
{
	return DirectX::XMFLOAT2(float(controllers.state.Gamepad.sThumbLX / STICK_MAX), float(controllers.state.Gamepad.sThumbLY / STICK_MAX));
}

DirectX::XMFLOAT2 XInputManager::GetPadRStickIncline()
{
	return DirectX::XMFLOAT2(float(controllers.state.Gamepad.sThumbRX / STICK_MAX), float(controllers.state.Gamepad.sThumbRY / STICK_MAX));
}

float XInputManager::GetPadLStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadLStickIncline();
	float h = pad.x;
	float v = -pad.y;

	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	if (radian < 0)
	{
		radian += 360;
	}

	//右向きなので上向きに直す
	radian += 90;

	return radian;
}

float XInputManager::GetPadRStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadRStickIncline();
	float h = pad.x;
	float v = -pad.y;

	//上向きが 0 (360)になるようにする
	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	if (radian < 0)
	{
		radian += 360;
	}

	//右向きなので上向きに直す
	radian += 90;

	return radian;
}

void XInputManager::StartVibration(const STRENGTH& _strength)
{
	controllers.vibration.wLeftMotorSpeed = _strength;
	controllers.vibration.wRightMotorSpeed = _strength;

	XInputSetState(0, &controllers.vibration);
}

void XInputManager::EndVibration()
{
	controllers.vibration.wLeftMotorSpeed = 0;
	controllers.vibration.wRightMotorSpeed = 0;

	XInputSetState(0, &controllers.vibration);
}
