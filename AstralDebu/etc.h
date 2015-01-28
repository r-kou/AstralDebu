#ifndef _ETC_H
#define _ETC_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "debu.h"

namespace etcNS{
	const float VEL_HAM = 300;

	const int IMG_ROCK_AREA1 = 4;
	const int IMG_ROCK_AREA2 = 9;
	const int IMG_ROCK_AREA3 = 14;
	const int IMG_MEAT = 31;
	const int IMG_HIMEAT = 32;
	const int IMG_HAMMER = 27;

	const int IMG_SIZE = 32;
	const int IMG_COL = 10;
	const int EDGE_ROCK_X = entityNS::EDGE_MAX;
	const int EDGE_ROCK_Y = entityNS::EDGE_MAX;
	const int EDGE_HAMMER_X = entityNS::EDGE_MAX;
	const int EDGE_HAMMER_Y = entityNS::EDGE_MAX;
	const int EDGE_HAMMER_HOLDED_X = entityNS::EDGE_MAX * 2;
	const int EDGE_MAR_X = 2;
	const int EDGE_MAR_Y = 2;

	const float HAMMER_MAR_X = 32;
	const float HAMMER_MAR_Y = 8;
}

class Rock : public Entity{
public:
	//�R���X�g���N�^ �X�e�[�W�ɂ���ĐF���ς��
	Rock(int stage);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);
};

class Meat : public Entity{
public:
	//�R���X�g���N�^
	Meat();
};

class Himeat : public Entity{
public:
	//�R���X�g���N�^
	Himeat();
};

class Hammer : public Entity{
public:
	//�R���X�g���N�^
	Hammer();

	//�ړ�
	virtual void move(float frameTime);

	//�n�`�ւ̐ڐG����
	virtual void touchMap(int map[MAP_COL][MAP_ROW]);

	//���I�u�W�F�N�g�ւ̐ڐG����
	virtual void touchObj(Entity *e);

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//������
	void setHold(Debu *d);
};
#endif