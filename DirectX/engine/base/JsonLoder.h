#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#include <unordered_map>

struct JsonObjectData {
	struct ObjectData {
		// 平行移動
		DirectX::XMFLOAT3 pos;
		// 回転角
		DirectX::XMFLOAT3 rotation;
		// スケーリング
		DirectX::XMFLOAT3 scale;
	};

	//名前のリスト
	std::vector<std::string> nameList;
	//種類ごとの名前配列
	std::unordered_map<std::string, std::vector<ObjectData>> objects;
};

struct JsonMoveData {
	struct MoveData {
		//座標
		DirectX::XMFLOAT3 pos;
		//移動先
		std::vector<int> moveList;
	};

	std::vector<MoveData> movePoint;
};

class JsonLoder
{
public:

	/// <summary>
	/// 名前の探索
	/// </summary>
	/// <param name="_name">探す元の名前</param>
	/// <param name="_nameList">名前のリスト</param>
	/// <returns>見つかったワード</returns>
	static const std::string SearchName(const std::string& _name, const std::vector<std::string>& _nameList);

	/// <summary>
	/// jsonの読み込み
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <param name="_nameList">種類別名前のリスト</param>
	static JsonObjectData* LoadFile(const std::string& _fileName, std::vector<std::string> _nameList);

	/// <summary>
	/// jsonの読み込み
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	static JsonMoveData* LoadMoveFile(const std::string& _fileName);

private:

	static const std::string base_directory;
};