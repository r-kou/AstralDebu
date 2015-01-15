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
	const int OBJ_SIZE = 64;

	const int IMG_COL_CHIP = 10;
	const int IMG_COL_DEBU = 7;
	const int IMG_COL_BLAST = 6;
	const int IMG_COL_ENEMY = 7;

	const int CHIP_ROCK = 5;
	const int CHIP_BOX_W = 20;
	const int CHIP_BOX_S = 21;
	const int CHIP_BOX_L = 22;
	const int CHIP_BOX_B = 23;
	const int CHIP_BOX_H = 24;
	const int CHIP_BOMB = 25;
	const int CHIP_HIBOMB = 26;
	const int CHIP_HAMMER = 27;
	const int CHIP_LADDER = 30;
	const int CHIP_MEAT = 31;
	const int CHIP_HIMEAT = 32;
	const int CHIP_WARP_R = 35;
	const int CHIP_WARP_G = 36;
	const int CHIP_WARP_Y = 37;
	const int CHIP_WARP_E = 38;
	const int CHIP_CURSOR = 40;
	const int CHIP_GOAL = 41;

	const float HOLD_MAR_X = 8;
	const float HOLD_MAR_Y = 16;
	const float VEL_THROW = 300;

	const int LIFE_MAR_X = 120;
	const int LIFE_MAR_Y = 48;
	const int LIFE_LEN_X = 400;
	const int LIFE_LEN_Y = 64;
	const int WORD_MAR_X = 40;
	const int WORD_MAR_Y = 64;
	const int WORD_LEN_X = LIFE_MAR_X - WORD_MAR_X;
	const int WORD_LEN_Y = 24;
	const int DATA_MAR = 4;
	const int STG_MAR_X = 520;
	const int STG_MAR_Y = LIFE_MAR_Y;
	const int STG_LEN_X = WINDOW_W - STG_MAR_X;
	const int STG_LEN_Y = LIFE_LEN_Y;

	const enum GameState{
		S_TITLE, S_STAGE, S_MAIN, S_OVER, S_CLEAR, S_END
	};
}

class AstralDebu :public Game{
private:
	//�ėp�A�j���J�E���^
	int count;
	//���݂̃X�e�[�W
	int stage;
	//�ǂݍ��ݔ���
	bool read;
	//�}�b�v�`�b�v
	int map[MAP_COL][MAP_ROW];
	//�Q�[�����
	astralNS::GameState state;
	//�X�e�[�W�N���A
	bool clear;
	//��l��
	Debu *debu;
	//�I�u�W�F�N�g
	Entity *object[astralNS::OBJ_SIZE];
	//���݂̃I�u�W�F�N�g��
	int obj_num;
	//�f�u���ێ�����I�u�W�F�N�g
	int obj_hold;
	//���[�v�P�ڂ̃C���f�b�N�X
	int warp_r, warp_g, warp_y;

	//��l���̗�
	int life, life_v;

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
	void renderTitle();
	void renderStage();
	void renderMain();
	void renderClear();
	//�X�e�[�W�ǂݍ���
	void loadStage();
	//�`�b�v���蓖��
	void loadChip(int, int, char);
	//�X�e�[�W�ɍ��킹�ă`�b�v��I��
	int chipFormat(int);
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
	void setVertexT(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//�O�p��`��
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//�J�[�\����`��
	void drawCursor();
	//�f�o�b�O�p �����`��
	void drawEdge();
	//�q���g�p����`�� ��
	void drawArrowH(float cx, float cy, bool d, ARGB c);
	//�q���g�p����`�� �c
	void drawArrowV(float cx, float cy, bool d, ARGB c);
	//�q���g�p�p�l����`��
	void drawPanel(std::string str, float cx, float cy, float len, ARGB c);

	//�J�[�\���̈ʒu��Ԃ�
	int cursorX();
	int cursorY();
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