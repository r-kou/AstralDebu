#ifndef _BOX_H
#define _BOX_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace boxNS{
	const float VEL_BOMB_X = 350;
	const float VEL_BOMB_Y = 250;
	const float VEL_BOMB_A = 350;

	const int IMG_WOOD_BOX = 32;
	const int IMG_STEEL_BOX = 33;
	const int IMG_LEAD_BOX = 34;
	const int IMG_BOMB_BOX = 35;
	const int IMG_HIBOMB_BOX = 36;
	const int IMG_AIR_BOX = 37;
	const int IMG_FRAME_BOX = 38;
	const int IMG_GOAST_BOX = 39;

	const int EDGE_X = entityNS::EDGE_MAX;
	const int EDGE_Y = entityNS::EDGE_MAX;
	const int EDGE_MAR_X = 3;
	const int EDGE_MAR_Y = 3;
}

class Box : public Entity{
public:
	//�R���X�g���N�^
	Box();

	//�ړ�
	//virtual void move(float frameTime);

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//���[�v�̐���𔻒�
	virtual void collideWarp(Entity *e);
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
	//virtual void move(float frameTime);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

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


class GoastBox : public Box {
public :
	//�R���X�g���N�^
	GoastBox();

	//�ړ�
	//virtual void move(float frameTime);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//�`��
	virtual void draw();
};
#endif