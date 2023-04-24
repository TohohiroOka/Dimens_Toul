#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

std::vector<Audio::SOUND_DATA*> Audio::soundData;
Microsoft::WRL::ComPtr<IXAudio2>Audio::xAudio2;
IXAudio2MasteringVoice* Audio::masterVoice;

Audio::Audio()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);//XAudioエンジンのインスタンス生成
	xAudio2->CreateMasteringVoice(&masterVoice);//マスターボイス生成
	masterVoice->SetVolume(0.1f);//全体の音量}
}

Audio::~Audio()
{
	//xAudio2解放
	xAudio2.Reset();

	const int size = (UINT)soundData.size();

	//バッファのメモリを解放
	for (int i = 0; i < size; i++)
	{
		delete[] soundData[i]->pBuffer;

		soundData[i]->pBuffer = 0;
		soundData[i]->bufferSize = 0;
		soundData[i]->wfex = {};
	}

	soundData.clear();
}

int Audio::SoundLoadWave(const std::string& fileName)
{
	//現在の配列サイズ
	int size = (UINT)soundData.size();

	SOUND_DATA* addSoundData = nullptr;
	addSoundData = new SOUND_DATA();

	//ファイル入力ストリームのインスタンス
	std::ifstream file;

	//wavファイルをバイナリモードで開く
	file.open(fileName, std::ios_base::binary);

	//ファイルオープン失敗を検知
	assert(file.is_open());

	///wavデータ読み込み
	//RIFFヘッダーの読み込み
	RIFF_HEADER riff;
	file.read((char*)&riff, sizeof(riff));

	//ファイルがRIFFがチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//タイプがWAVEがチェック
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Formatチャンクの読み込み
	FORMAT_CHUNK format = {};

	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(CHUNK_HEADER));

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	CHUNK_HEADER data;
	file.read((char*)&data, sizeof(data));

	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK ", 4) == 0 ||
		strncmp(data.id, "LIST", 4) == 0)
	{
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);

		//再度読み込み
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	///ファイルクローズ
	//Waveファイルを閉じる
	file.close();

	///読み込んだ音声データを返す
	//returnするための音声データ
	addSoundData->wfex = format.fmt;
	addSoundData->pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	addSoundData->bufferSize = data.size;

	CreateSoundData(*addSoundData);
	float SourceVoiceChannelVolumes[1] = { 0.0 };
	addSoundData->pSourceVoice->SetChannelVolumes(1, SourceVoiceChannelVolumes);

	soundData.push_back(addSoundData);

	return size;
}

void Audio::CreateSoundData(SOUND_DATA& soundData) {
	HRESULT result;

	//波形フォーマットを元にSourceVoiceの生成
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));
}

bool Audio::SoundPlayWava(int number, bool roop)
{
	HRESULT result;

	XAUDIO2_VOICE_STATE xa3state;
	soundData[number]->pSourceVoice->GetState(&xa3state);
	//0でないなら音楽が再生中になるためスルーする
	if (xa3state.BuffersQueued != 0)
	{
		return false;
	}

	//データ生成
	CreateSoundData(*soundData[number]);

	//再生する波形データの設定
	buf.pAudioData = soundData[number]->pBuffer;
	buf.AudioBytes = soundData[number]->bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (roop == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//波形データの再生
	result = soundData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->Start();
	assert(SUCCEEDED(result));

	buf = { NULL };

	return true;
}

void Audio::StopSound(int number)
{
	HRESULT result;

	result = soundData[number]->pSourceVoice->Stop(0);
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->FlushSourceBuffers();
	assert(SUCCEEDED(result));
	result = soundData[number]->pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
}