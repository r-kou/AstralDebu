#ifndef _BOX_H
#define _BOX_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace boxNS{
	const float VEL_BOMB_X = 350;
	const float VEL_BOMB_Y = 250;
	const float VEL_BOMB_M = 150;
	const float VEL_BOMB_A = 350;

	const int IMG_W = 20;
	const int IMG_S = 21;
	const int IMG_L = 22;
	const int IMG_B = 23;
	const int IMG_H = 24;
	const int IMG_A = 45;
	const int IMG_F = 46;

	const int IMG_SIZE = 32;
	const int IMG_COL = 10;
	const int EDGE_X = entityNS::EDGE_MAX;
	const int EDGE_Y = entityNS::EDGE_MAX;
	const int EDGE_MAR_X = 2;
	const int EDGE_MAR_Y = 2;
}

class Box : public Entity{
public:
	//�R���X�g���N�^
	Box();

	//�ړ�
	virtual void move(float frameTime);

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};

class WoodBox : public Box{
public:
	//�R���X�g���N�^
	WoodBox();

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};

class SteelBox : public Box{
public:
	//�R���X�g���N�^
	SteelBox();

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//���I�u�W�F�N�g�ւ̔���
	virtual void responseObj();
};

class LeadBox : public Box{
public:
	//�R���X�g���N�^
	LeadBox();

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};

class BombBox : public Box{
public:
	//�R���X�g���N�^
	BombBox();

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};

class HibombBox : public Box{
public:
	//�R���X�g���N�^
	HibombBox();

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};


class AirBox : public Box{
public:
	//�R���X�g���N�^
	AirBox();

	//�ړ�
	virtual void move(float frameTime);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//���I�u�W�F�N�g�ւ̔���
	virtual void responseObj();

	//�`��
	virtual void draw();
};


class FrameBox : public Box{
public:
	//�R���X�g���N�^
	FrameBox();

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};

#endif