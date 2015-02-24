#ifndef _BOMB_H
#define _BOMB_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace bombNS{
	const int IMG_BOMB = 40;
	const int IMG_HIBOMB = 41;
	const int IMG_MINE = 43;

	const int EDGE_X = entityNS::EDGE_MAX - 3;
	const int EDGE_Y = entityNS::EDGE_MAX;
	const int EDGE_MAR_X = 3;
	const int EDGE_MAR_Y = 3;
}

class BombE : public Entity{
public:
	//�R���X�g���N�^
	BombE();

	//�ړ�
	//virtual void move(float frameTime);

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};

class Bomb : public BombE{
public:
	//�R���X�g���N�^
	Bomb();
};

class Hibomb : public BombE{
public:
	//�R���X�g���N�^
	Hibomb();
};

class Mine : public Entity {
public:
	//�R���X�g���N�^
	Mine();

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};
#endif