#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>

class Csv
{
protected: // エイリアス
// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;

public:

	/// <summary>
	/// マップの読み込み
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <param name="_map"></param>
	static void LoadFile(const std::string& _fileName, std::vector<std::vector<int>> _map);

	/// <summary>
	/// オブジェクト座標の配列番号の値の取得
	/// </summary>
	/// <param name="_map">二次元配列</param>
	/// <param name="_chipSize">一つ分の大きさ</param>
	/// <param name="_position">オブジェクト座標</param>
	/// <returns></returns>
	static int GetChipNum(std::vector<std::vector<int>> _map, float _chipSize, const XMFLOAT2& _position);
};