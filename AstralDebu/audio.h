#ifndef _AUDIO_H
#define _AUDIO_H
#define WIN32_LEAN_AND_MEAN

#include <xact3.h>
#include "const.h"

namespace audioNS{
	const char CT_MUSIC[] = "Music";
	const char CT_DEFAULT[] = "Default";
	const char CT_BGM[] = "bgm";

	const char BLAST1[] = "blast_1";
	const char BLAST2[] = "blast_2";
	const char WARP[] = "warp";
	const char THROW_WOOD[] = "throw_wood";
	const char THROW_STEEL[] = "throw_steel";
	const char THROW_LEAD[] = "throw_lead";
	const char THROW_HAMMER[] = "throw_hammer";
	const char PUT_WOOD[] = "put_wood";
	const char PUT_STEEL[] = "put_steel";
	const char PUT_LEAD[] = "put_lead";
	const char PUT_DEBU[] = "put_debu";
	const char KNOCK[] = "knock";
	const char BRAKE_WOOD[] = "brake_wood";
	const char BRAKE_STEEL[] = "brake_steel";
	const char GOAL[] = "goal";
	const char SELECT[] = "select";
	const char OK[] = "ok";
	const char CANCEL[] = "cancel";
	const char EAT_1[] = "eat_1";
	const char EAT_2[] = "eat_2";
	const char BGM1[] = "bgm1";
	const char BGM2[] = "bgm2";
	const char BGM3[] = "bgm3";
	const char BGM4[] = "bgm4";
	const char BGM_TITLE[] = "bgm_title";
}

//音声関連を扱うクラス XACT使用
class Audio {
private:
	//XACTの本体
	IXACT3Engine* xact;
	//waveを管理するバンク
	IXACT3WaveBank* wave;
	//soundを管理するバンク
	IXACT3SoundBank* sound;
	//音声ファイル名に対応するインデックス
	XACTINDEX cueIndex;
	//音声ファイルが所属するカテゴリ
	XACTCATEGORY category;
	void* map;
	void* data;
	void* global;
	//初期化済みかの判定
	bool initialized;
public:
	//コンストラクタ
	Audio();

	//デストラクタ
	virtual ~Audio();

	//初期化
	HRESULT initialize();

	//定期実行
	void run();

	//指定オーディオの演奏を始める
	void playCue(const char cue[]);

	//指定オーディオの演奏を止める
	void stopCue(const char cue[]);

	//指定オーディオが演奏中か調べる
	bool isPlaying(const char cue[]);

	//Bgmの音量を調整する
	void setVolumeBgm(double volume);

	//Soundの音量を調整する
	void setVolumeSound(double volume);
};

#endif