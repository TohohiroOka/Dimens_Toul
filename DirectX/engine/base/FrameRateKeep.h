#pragma once
#include <chrono>
#include <d3dx12.h>

class WindowApp;

class FrameRateKeep
{
private:

public:

	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static std::unique_ptr<FrameRateKeep> Create();

	/// <summary>
	/// Fps値の固定
	/// </summary>
	void FixedFps();

public:

	//スタート時間
	float deltaTime = 0.0f;
	float frameRate = 0.0f;
	float commandWaitTime = 0.0f;
	int count = 0;
	std::chrono::steady_clock::time_point lastUpdate;

};

