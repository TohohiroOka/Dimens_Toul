#include "Camera.h"
#include "WindowApp.h"
#include "GameHelper.h"

using namespace DirectX;

Camera::Camera()
{
	aspectRatio = float(WindowApp::GetWindowWidth() / WindowApp::GetWindowHeight());

	// 射影行列の計算
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, 1200.0f//奥行/手前,最奥
	);
}

std::unique_ptr<Camera> Camera::Create()
{
	// Spriteのインスタンスを生成
	Camera* instance = new Camera();
	if (instance == nullptr) {
		return nullptr;
	}

	//ユニークポインタを返す
	return std::unique_ptr<Camera>(instance);
}

void Camera::Update()
{
	XMFLOAT3 inoutEye = { ShakeDifference.x + eye.x,ShakeDifference.y + eye.y,ShakeDifference.z + eye.z };

	matView = XMMatrixLookAtLH(XMLoadFloat3(&inoutEye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::StartCameraShake(int _strength)
{
	float x = Randomfloat(_strength * 2) - float(_strength);
	float y = Randomfloat(_strength * 2) - float(_strength);

	ShakeDifference = { x ,y,0.0f };
}

void Camera::EndCameraShake()
{
	ShakeDifference = { 0.0f,0.0f,0.0f };
}

void Camera::SetMatProjection(float _back)
{
	aspectRatio = float(WindowApp::GetWindowWidth() / WindowApp::GetWindowHeight());

	// 射影行列の計算
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, _back//奥行/手前,最奥
	);
}