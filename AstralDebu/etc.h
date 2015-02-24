#ifndef _ETC_H
#define _ETC_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "debu.h"

namespace etcNS{
	const float VEL_HAM = 300;

	const int IMG_ROCK_AREA1 = 4;
	const int IMG_ROCK_AREA2 = 12;
	const int IMG_ROCK_AREA3 = 20;
	const int IMG_ROCK_AREA4 = 28;
	const int IMG_MEAT = 44;
	const int IMG_HIMEAT = 45;
	const int IMG_HAMMER = 42;
	const int IMG_LADDER = 46;

	const int EDGE_ROCK_X = entityNS::EDGE_MAX;
	const int EDGE_ROCK_Y = entityNS::EDGE_MAX;
	const int EDGE_LADDER_X = 12;
	const int EDGE_LADDER_Y = entityNS::EDGE_MAX;
	const int EDGE_MEAT_X = entityNS::EDGE_MAX;
	const int EDGE_MEAT_Y = entityNS::EDGE_MAX;
	const int EDGE_HAMMER_X = entityNS::EDGE_MAX;
	const int EDGE_HAMMER_Y = entityNS::EDGE_MAX;
	const int EDGE_HAMMER_HOLDED_X = entityNS::EDGE_MAX * 2;
	const int EDGE_MAR_X = 2;
	const int EDGE_MAR_Y = 2;

	const float HAMMER_MAR_X = 32;
	const float HAMMER_MAR_Y = 8;
}

class Rock : public Entity{
private:
	bool chipTop, chipBottom;
public:
	//�R���X�g���N�^ �X�e�[�W�ɂ���ĐF���ς��
	Rock(int stage);

	//�n�`�ւ̐ڐG����
	virtual void touchMap(int map[MAP_COL][MAP_ROW]);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//�`�悷��摜��ύX
	virtual void changeImage();

	//�`��
	virtual void draw();
};

class Ladder :public Entity {
public:
	//�R���X�g���N�^
	Ladder();

	//�n�`�ւ̐ڐG�i�������Ȃ��j
	virtual void collideMap(UCHAR t){}

	//���I�u�W�F�N�g�ւ̐ڐG�i�������Ȃ��j
	virtual void collideObj(Entity *e, UCHAR t){}

	//�`��
	virtual void draw();
};

class MeatE : public Entity{
public:
	//�R���X�g���N�^
	MeatE();

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

};
class Meat : public MeatE{
public:
	//�R���X�g���N�^
	Meat();
};

class Himeat : public MeatE{
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