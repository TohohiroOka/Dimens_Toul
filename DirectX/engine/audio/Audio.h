#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3dx12.h>

class Audio
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//チャンクヘッダ
	struct CHUNK_HEADER
	{
		char id[4];//チャンク毎のID
		int	size;  // チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RIFF_HEADER
	{
		CHUNK_HEADER chunk;//"RIFF"
		char type[4];//"WAVE"
	};

	//FMTチャンク
	struct FORMAT_CHUNK
	{
		CHUNK_HEADER chunk;//"fmt"
		WAVEFORMATEX fmt;//波形フォーマット
	};

	//音声データ
	struct SOUND_DATA
	{
		WAVEFORMATEX wfex;//波形フォーマット
		BYTE* pBuffer;//バッファの先頭アドレス
		unsigned int bufferSize;//バッファのサイズ
		IXAudio2SourceVoice* pSourceVoice;
	};

public:

	Audio();
	~Audio();

	/// <summary>
	/// 音声データ読み込み
	/// </summary>
	/// <param name="_fileName">ファイル名</param>
	/// <returns>格納番号</returns>
	int SoundLoadWave(const std::string& _fileName);
	
	/// <summary>
	/// データの生成
	/// </summary>
	/// <param name="_soundData">生成するサウンドデータ情報</param>
	void CreateSoundData(SOUND_DATA& _soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="_number">サウンドデータの格納番号</param>
	/// <param name="_roop">ループの有無</param>
	/// <returns>データの設定を行ったか</returns>
	bool SoundPlayWava(int _number, bool _roop);

	/// <summary>
	/// サウンドの停止
	/// </summary>
	/// <param name="_number">サウンドデータの格納番号</param>
	void StopSound(int _number);

private:
	//サウンドデータ配列
	static std::vector<SOUND_DATA*> soundData;

	//サウンドのバッファ
	XAUDIO2_BUFFER buf{};

	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice* masterVoice;
};