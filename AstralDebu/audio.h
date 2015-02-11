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

//�����֘A�������N���X XACT�g�p
class Audio {
private:
	//XACT�̖{��
	IXACT3Engine* xact;
	//wave���Ǘ�����o���N
	IXACT3WaveBank* wave;
	//sound���Ǘ�����o���N
	IXACT3SoundBank* sound;
	//�����t�@�C�����ɑΉ�����C���f�b�N�X
	XACTINDEX cueIndex;
	//�����t�@�C������������J�e�S��
	XACTCATEGORY category;
	void* map;
	void* data;
	void* global;
	//�������ς݂��̔���
	bool initialized;
public:
	//�R���X�g���N�^
	Audio();

	//�f�X�g���N�^
	virtual ~Audio();

	//������
	HRESULT initialize();

	//������s
	void run();

	//�w��I�[�f�B�I�̉��t���n�߂�
	void playCue(const char cue[]);

	//�w��I�[�f�B�I�̉��t���~�߂�
	void stopCue(const char cue[]);

	//�w��I�[�f�B�I�����t�������ׂ�
	bool isPlaying(const char cue[]);

	//Bgm�̉��ʂ𒲐�����
	void setVolumeBgm(double volume);

	//Sound�̉��ʂ𒲐�����
	void setVolumeSound(double volume);
};

#endif