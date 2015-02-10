#ifndef _ASTRAL_H
#define _ASTRAL_H
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <fstream>
#include "game.h"
#include "texture.h"
#include "Image.h"
#include "text.h"
#include "entity.h"
#include "debu.h"
#include "warp.h"
#include "box.h"
#include "bomb.h"
#include "enemy.h"
#include "etc.h"
#include "blast.h"

namespace astralNS {
	//�z��̑傫��
	const int OBJ_SIZE = 128;
	const int STG_SIZE = 40;

	//�e�摜�̉�����
	const int IMG_COL_CHIP = 8;
	const int IMG_COL_DEBU = 7;
	const int IMG_COL_BLAST = 6;
	const int IMG_COL_ENEMY = 7;

	//�摜�`�b�v�̑Ή��l
	const int CHIP_WOOD_BOX = 32;
	const int CHIP_STEEL_BOX = 33;
	const int CHIP_LEAD_BOX = 34;
	const int CHIP_BOMB_BOX = 35;
	const int CHIP_HIBOMB_BOX = 36;
	const int CHIP_AIR_BOX = 37;
	const int CHIP_FRAME_BOX = 38;
	const int CHIP_GOAST_BOX = 39;
	const int CHIP_BOMB = 40;
	const int CHIP_HIBOMB = 41;
	const int CHIP_HAMMER = 42;
	const int CHIP_MINE = 43;
	const int CHIP_MEAT = 44;
	const int CHIP_HIMEAT = 45;
	const int CHIP_LADDER = 46;
	const int CHIP_CURSOR = 47;

	//�����I�u�W�F�N�g�̈ʒu�␳
	const float HOLD_MAR_X = 8;
	const float HOLD_MAR_Y = 16;
	const float VEL_THROW = 300;

	//�Z�a�`��̈ʒu
	const float LIFE_MAR_X = 120;
	const float LIFE_MAR_Y = 48;
	const float LIFE_LEN_X = 400;
	const float LIFE_LEN_Y = 64;
	const float WORD_MAR_X = 40;
	const float WORD_MAR_Y = 64;
	const float WORD_LEN_X = LIFE_MAR_X - WORD_MAR_X;
	const float WORD_LEN_Y = 24;
	const float DATA_MAR = 4;
	const float STG_MAR_X = 520;
	const float STG_MAR_Y = LIFE_MAR_Y;
	const float STG_LEN_X = WINDOW_W - STG_MAR_X;
	const float STG_LEN_Y = LIFE_LEN_Y;
	namespace mainMenuNS{
		const float FRAME = 10;
		const float SHADOW = 2.5f;
		const float MAR_X = 50;
		const float MAR_Y = 50;
		const float LEN_X = WINDOW_W - MAR_X * 2;
		const float LEN_Y = WINDOW_H - MAR_Y * 2;
		const float TEX_MAR_X = 100;
		const float TEX_MAR_Y = 100;
		const float TEX_LEN_X = WINDOW_W - TEX_MAR_X * 2;
		const float TEX_LEN_Y = WINDOW_H - TEX_MAR_Y * 2;
		const float TEX_SEP_X = TEX_LEN_X / 2;
		const float TEX_SEP_Y = TEX_LEN_Y / 8;
		const float ARR_MAR_X = TEX_MAR_X - CHIP(0.5f);
		const float ARR_MAR_Y = TEX_MAR_Y + TEX_SEP_Y * 7 + 10.0f;
		const ARGB BACK = D3DCOLOR_ARGB(255, 160, 64, 16);
		const ARGB BACK_SHADE = D3DCOLOR_ARGB(255, 144, 48, 0);
		const ARGB BACK_LIGHT = D3DCOLOR_ARGB(255, 176, 80, 32);
	}
	//�F
	const ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const ARGB GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);
	const ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const ARGB STG1_BACK = D3DCOLOR_ARGB(255, 128, 224, 255);
	const ARGB STG2_BACK = D3DCOLOR_ARGB(255, 255, 255, 255);
	const ARGB STG3_BACK = D3DCOLOR_ARGB(255, 32, 48, 48);
	const ARGB STG4_BACK = D3DCOLOR_ARGB(255, 192, 255, 255);
	const ARGB LIFE_GAUGE = D3DCOLOR_ARGB(255, 224, 224, 0);
	const ARGB LIFE_VITAL_PLUS = D3DCOLOR_ARGB(255, 0, 224, 0);
	const ARGB LIFE_VITAL_MINUS = D3DCOLOR_ARGB(255, 224, 0, 0);
	const ARGB MENU_BACK = D3DCOLOR_ARGB(255, 160, 64, 16);
	const ARGB MENU_TEXT = D3DCOLOR_ARGB(255, 224, 224, 32);
	const ARGB MENU_HIDE = D3DCOLOR_ARGB(255, 96, 64, 32);
	const ARGB HINT_ARROW = D3DCOLOR_ARGB(255, 255, 255, 32);
	const ARGB HINT_PANEL = D3DCOLOR_ARGB(255, 64, 192, 64);

	const std::string FONT = "���C���I";

	const enum GameState{
		S_TITLE, S_STAGE, S_MAIN, S_OVER, S_CLEAR, S_END
	};
}

class AstralDebu :public Game{
private:
	//�ėp�A�j���J�E���^
	int count;
	//�ėp�A�j���J�E���^
	float fCount;
	//���݂̃X�e�[�W
	int stage;
	//�ǂݍ��ݔ���
	bool read;
	//�}�b�v�`�b�v
	int map[MAP_COL][MAP_ROW];
	//�Q�[�����
	astralNS::GameState state;
	//�^�C�g���Ƃ��̏��
	int stateNumber;
	//�X�e�[�W�N���A�ɂ�����������
	DWORD clearTimeStart, clearTimeEnd;
	//�N���A���Ԉꗗ
	double clearTime[astralNS::STG_SIZE];
	//�N���A�����Ō�̃X�e�[�W
	int clearedStage;
	//���y�̏������Ƃ�
	bool bgm;
	//���j���[���
	bool menu;
	//����
	double bgmVolume, soundVolume;

	//�X�e�[�W�N���A
	bool clear;
	//��l��
	Debu *debu;
	//�I�u�W�F�N�g
	Entity *object[astralNS::OBJ_SIZE];
	//���݂̃I�u�W�F�N�g��
	int objMax;
	//�f�u���ێ�����I�u�W�F�N�g
	int objHolded;
	//���[�v�P�ڂ̃C���f�b�N�X
	int warpRed, warpGreen, warpYellow;
	//�`�[�g���[�h�𔭓�������
	bool cheat;

	//��l���̗�
	int life, vitalLife;

	//�摜�f�[�^
	Texture debuT, chipT, enemyT, bombT, titleT;
	//�摜�̂݃f�[�^
	Image chip, title;
	//�t�H���g
	Text stageF, numberF, bigF, middleF, smallF;
	//�`��p�Z�a
	RECT rect;
	//vertex
	Vertex vtx[4];
	LP_VERTEX vertex;

	//�e�N�X�`���̏�����
	void initTexture(Texture &t, std::string file);
	//�t�H���g�̏�����
	void initFont(Text &t, int point);

	//�Q�[����Ԃɉ����čX�V
	void updateTitle();
	void updateStage();
	void updateMain();
	void updateClear();
	void updateMenu();
	void renderTitle();
	void renderStage();
	void renderMain();
	void renderClear();
	void renderMenu();

	//�Z�[�u�f�[�^�ǂݍ���
	void loadData();
	//�Z�[�u�f�[�^��������
	void saveData();
	//�N���A�^�C���̍X�V
	void updateClearTime();

	//�X�e�[�W�ǂݍ���
	void loadStage();
	//�`�b�v���蓖��
	void loadChip(int, int, short);
	//�X�e�[�W�ɍ��킹�ă`�b�v��I��
	int setChipImage(int);

	//�R�}���h�ǂݍ���
	void readCommand();
	//�I�u�W�F�N�g�̒ǉ�
	void addObject(Entity *e, Texture &t, int i, int j);
	//�G�̒ǉ�
	void addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j);
	//���[�v�̒ǉ�
	void addWarp(int i, int j);
	//�I�u�W�F�N�g�̎擾����
	void handleObject();
	//�I�u�W�F�N�g��͂�
	void holdObject(int i);
	//�I�u�W�F�N�g�����
	void putObject();
	//�I�u�W�F�N�g������
	void pushObject(int i);
	//�I�u�W�F�N�g�𓊂���
	void throwObject();
	//�����I�u�W�F�N�g���f�u�̑O�Ɉړ�
	void moveHold(int i);
	//�I�u�W�F�N�g�̏��ŏ���
	void deadObject(int i);
	//�I�u�W�F�N�g�̓���s��
	void actionObject(int i);
	//�I�u�W�F�N�g�f�[�^�Ƃ���������
	void resetObject();

	//�^�C�g���Ɣw�i��`��
	void renderTitleBack();
	//�X�^�[�g��`��
	void renderTitleStart();
	//���j���[��`��
	void renderTitleMenu();
	//�X�e�[�W�Z���N�g��`��
	void renderTitleSelect();
	//���ʑI����`��
	void renderTitleVolume();
	//�w�i�Ə�̂�`��
	void renderBack();
	//�}�b�v�`�b�v��`��
	void renderChip();
	//�q���g�Ƃ��`��
	void renderHint();
	//�I�u�W�F�N�g��`��
	void renderObject();

	//vertex��ݒ�
	void setVertex(float l, float t, float r, float b, ARGB c);
	//�l�p��`��
	void drawQuad(float l, float t, float r, float b, ARGB c);
	//�O�p��vertex��`��
	void setVertexTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//�O�p��`��
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//�O�p��`��
	void drawTriangleHorizontal(float l, float t, float r, float b, bool d, ARGB c);
	//�O�p��`��
	void drawTriangleVertical(float l, float t, float r, float b, bool d, ARGB c);
	//�J�[�\����`��
	void drawCursor();
	//�f�o�b�O�p �����`��
	void drawEdge();
	//����`�� ��
	void drawArrowHorizontal(float cx, float cy, bool d, ARGB c);
	//����`�� �c
	void drawArrowVertical(float cx, float cy, bool d, ARGB c);
	//�q���g�p����`�� ��
	void drawArrowHorizontal(float l, float t, float r,float b, bool d, ARGB c);
	//�q���g�p����`�� �c
	void drawArrowVertical(float l, float t, float r, float b, bool d, ARGB c);
	//�p�l����`��
	void drawPanel(std::string str, float cx, float cy, float len, ARGB c);
	//�摜���p�l����`��
	void drawPanel(int img,float cx,float cy,ARGB c);
	//�g�t���l�p��`��
	void drawFrame(float l, float t, float r, float b, float f, ARGB c, ARGB fc);
	//�A����g�t���l�p��`��
	void drawFrame(float l, float t, float r, float b, float f, float s, ARGB c, ARGB fc, ARGB fs, ARGB fl);

	//�J�[�\���̈ʒu��Ԃ�
	int getCursorChipX();
	int getCursorChipY();
	//�I�u�W�F�N�g�����Ă邩����
	bool canHold(Entity *e);
	//�I�u�W�F�N�g����������
	bool canEat(Entity *e);
	//�I�u�W�F�N�g���ڐG�\������
	bool canTouch(Entity *e);
	//�I�u�W�F�N�g���`��\������
	bool canMove(Entity *e);
	//�̗͂̑���
	void addLife(int i);
	//�̗͂̌���
	void subLife(int i);
	//���g�p�̃I�u�W�F�N�g���擾
	int getEmptyIndex();
	//�}�b�v�`�b�v�̂ŃR�[�h
	short decodeChip(short c, int i, int j) {return (c - ((i*(j + 1)) << 4)) / ((i + 1) * 11);}
	//bgm�Đ�
	void playBgm();
	//bgm��~
	void stopBgm();
	//double�𐮐��ɂ��ĕ�����
	std::string doubleToString(double f);
	//�F��I��
	ARGB menuText(bool b) { return (b ? astralNS::MENU_TEXT : astralNS::MENU_HIDE); }

public:
	//�R���X�g���N�^
	AstralDebu();
	//�f�X�g���N�^
	virtual ~AstralDebu();
	//������
	virtual void initialize(HWND hwnd);
	//�����̍X�V
	virtual void update();
	//�`��
	virtual void render();
	//�|�C���^�̉���Ƃ�
	virtual void releaseAll();
	virtual void resetAll();
	virtual void deleteAll();
};


#endif