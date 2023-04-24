#include "JsonLoder.h"
#include <fstream>
#include <json.hpp>
#include "GameHelper.h"

using json = nlohmann::json;

const std::string JsonLoder::base_directory="Resources/json/";

const std::string JsonLoder::SearchName(const std::string& _name, const std::vector<std::string>& _nameList)
{
	for (auto& target : _nameList)
	{
		//文字を探す
		int programmerPos = int(_name.find(target));

		//無かった場合は次
		if (programmerPos == std::string::npos) { continue; }
		return target;
	}

	return "null";
}

JsonObjectData* JsonLoder::LoadFile(const std::string& _fileName, std::vector<std::string> _nameList)
{
	const std::string fullpath = base_directory + _fileName;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	if (file.fail()) {
		assert(0);
	}

	//解凍データ保存先
	json deserialized;

	//解凍
	file >> deserialized;

	//データが正しいかのチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	//格納変数
	JsonObjectData* jsonData = new JsonObjectData;

	//名前リスト登録
	size_t listSize = _nameList.size();
	jsonData->nameList.resize(listSize + 1);
	//名前検索でヒットしなかったオブジェクト用
	jsonData->nameList[0] = "null";
	//名前を保存
	for (int i = 0; i < listSize; i++)
	{
		int inSize = i + 1;
		jsonData->nameList[inSize] = _nameList[i];
	}

	//"objects"の全オブジェクトを走査
	for (json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		//種別を取得
		std::string type = object["type"].get<std::string>();

		if (type.compare("MESH") == 0) {
			//名前探索
			std::string inObjectName = SearchName(object["name"], jsonData->nameList);

			// 要素追加
			jsonData->objects[inObjectName].emplace_back(JsonObjectData::ObjectData());
			// 今追加した要素の参照を得る
			JsonObjectData::ObjectData& objectData = jsonData->objects[inObjectName].back();

			//トランスフォーム情報
			json& transform = object["transform"];
			// 平行移動
			objectData.pos.x = float(transform["translation"][0]) * mapSize / 2.0f;
			objectData.pos.y = float(transform["translation"][2]);
			objectData.pos.z = float(transform["translation"][1]) * mapSize / 2.0f;
			// 回転角
			objectData.rotation.x = -float(transform["rotation"][1]);
			objectData.rotation.y = -float(transform["rotation"][2]);
			objectData.rotation.z = float(transform["rotation"][0]);
			// スケーリング
			objectData.scale.x = float(transform["scaling"][1]);
			objectData.scale.y = float(transform["scaling"][2]);
			objectData.scale.z = float(transform["scaling"][0]);
		}
	}

	return jsonData;
}

JsonMoveData* JsonLoder::LoadMoveFile(const std::string& _fileName)
{
	const std::string fullpath = base_directory + _fileName;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	if (file.fail()) {
		assert(0);
	}

	//解凍データ保存先
	json deserialized;

	//解凍
	file >> deserialized;

	//データが正しいかのチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("move_point") == 0);

	//格納変数
	JsonMoveData* jsonData = new JsonMoveData;
	//サイズ変更
	jsonData->movePoint.resize(int(deserialized["total"]));

	//"objects"の全オブジェクトを走査
	for (json& object : deserialized["posints"]) {
		//要素番号
		int number = object["number"];

		//トランスフォーム情報
		json& transform = object["transform"];
		// 平行移動
		jsonData->movePoint[number].pos.x = float(object["point"][0]) * mapSize / 2.0f;
		jsonData->movePoint[number].pos.y = 100.0f;
		jsonData->movePoint[number].pos.z = float(object["point"][1]) * mapSize / 2.0f;

		const int move_num = object["list_num"];
		for (int i = 0; i < move_num; i++) {
			jsonData->movePoint[number].moveList.emplace_back(int(object["move_list"][i]));
		}
	}

	return jsonData;
}
