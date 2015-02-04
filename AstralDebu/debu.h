#ifndef _DEBU_H
#define _DEBU_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace debuNS{
	const float VEL_RATE = 800;
	const float VEL_MAX_WALK = 140;
	const float VEL_JUMP = -300;
	const float VEL_DUMP = 2000;
	const float VEL_RATE_JUMP = 600;
	const float VEL_LADDER = 100;
	const float VEL_BOMB_X = 350;
	const float VEL_BOMB_Y = 250;
	const float VEL_KNOCK_X = 250;

	const int IMG_SIZE = 32;
	const int IMG_COL = 7;
	const int IMG_STAND = 0;
	const int IMG_HOLD = 1;
	const int IMG_WALK_START = 2;
	const int IMG_WALK_END = 4;
	const int IMG_HOLD_WALK_START = 5;
	const int IMG_HOLD_WALK_END = 7;
	const int IMG_FALL_FRONT = 9;
	const int IMG_FALL_BACK = 10;
	const int IMG_JUMP_FRONT = 11;
	const int IMG_JUMP_BACK = 12;
	const int IMG_FALL_HOLD_FRONT = 13;
	const int IMG_FALL_HOLD_BACK = 14;
	const int IMG_JUMP_HOLD_FRONT = 15;
	const int IMG_JUMP_HOLD_BACK = 16;
	const int IMG_DEAD_START = 17;
	const int IMG_DEAD_END = 21;
	const int IMG_LADDER = 22;
	const int IMG_LADDER_START = 23;
	const int IMG_LADDER_END = 24;
	const int IMG_CLEAR_START = 25;
	const int IMG_CLEAR_END = 27;

	const int EDGE_X = 10;
	const int EDGE_Y = entityNS::EDGE_MAX;
	const int EDGE_MAR_X = 4;
	const int EDGE_MAR_Y = 6;
}

class Debu :public Entity{
private:
	//����
	Input *input;
	//�I�u�W�F�N�g���������Ă��邩
	bool hold;
	//�n���}�[��p�̌���
	bool directHammer;

	//�͂����ɕ߂܂�
	void getLadder();
public:
	//�R���X�g���N�^
	Debu();

	//������ ���͂������
	bool initialize(Game *game, Texture *t, Input *in, int i, int j);

	//�ړ�
	virtual void move(float frameTime);

	//�㉺���E�̏Փ˔���
	virtual UCHAR touchMapDirect(int c, UCHAR t);

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//�`�悷��摜��ύX
	virtual void changeImage();

	//setter
	void setHold(bool h) { hold = h; }
	void setHammer(bool d) { directHammer = d; }

	//getter
	bool getHold() { return hold; }
	bool getHammer() { return directHammer; }
};

#endif