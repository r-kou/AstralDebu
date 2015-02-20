#ifndef _ENEMY_H
#define _ENEMY_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "debu.h"

namespace enemyNS{
	const float VEL_WALK = 80;
	const float VEL_DASH = 160;
	const float VEL_BULLET = 200;
	const float VEL_MISSILE = 100;
	const float VEL_BOMB_X = 350;
	const float VEL_BOMB_Y = 250;
	const float VEL_KNOCK_X = 250;

	const int IMG_STAND = 0;
	const int IMG_ACTION = 1;
	const int IMG_WALK_START = 2;
	const int IMG_WALK_END = 4;
	const int IMG_DEAD_BACK = 5;
	const int IMG_DEAD_FRONT = 6;
	const int IMG_BULLET = 7;
	const int IMG_MISSILE = 8;

	const int EDGE_X = 10;
	const int EDGE_Y = entityNS::EDGE_MAX;
	const int EDGE_MAR_X = 4;
	const int EDGE_MAR_Y = 6;
	const int BULLET_X = 3;
	const int BULLET_Y = 2;
	const int BULLET_MAR_X = 0;
	const int BULLET_MAR_Y = 0;
	const int MISSILE_X = 12;
	const int MISSILE_Y = 8;
	const int MISSILE_MAR_X = 0;
	const int MISSILE_MAR_Y = 0;
}

class Enemy :public Entity{
protected:
	//�f�u
	Debu *debu;
public:
	//�R���X�g���N�^
	Enemy();

	//������ �f�u�̈ʒu���������
	bool initialize(Game *game, Texture *t, Debu *d, int i, int j);

	//�ړ�
	virtual void move(float frameTime);

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//�`�悷��摜��ύX
	virtual void changeImage();
};

class Enemy1 : public Enemy{
private:
public:
	//�R���X�g���N�^
	Enemy1();

	//�ړ�
	virtual void move(float frameTime);
};

class Enemy2 : public Enemy{
private:
public:
	//�R���X�g���N�^
	Enemy2();

	//�ړ�
	virtual void move(float frameTime);
};

class Enemy3 : public Enemy{
private:
public:
	//�R���X�g���N�^
	Enemy3();

	//�ړ�
	virtual void move(float frameTime);

	//�`�悷��摜��ύX
	virtual void changeImage();
};


class Enemy4 : public Enemy{
private:
public:
	//�R���X�g���N�^
	Enemy4();

	//�ړ�
	virtual void move(float frameTime);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//�`�悷��摜��ύX
	virtual void changeImage();
};


class Enemy5 : public Enemy{
private:
public:
	//�R���X�g���N�^
	Enemy5();

	//�ړ�
	virtual void move(float frameTime);

	//�`�悷��摜��ύX
	virtual void changeImage();
};

class BulletE :public Entity{
public:
	//�R���X�g���N�^
	BulletE();

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//�`�悷��摜��ύX
	virtual void changeImage();
};

class Bullet :public BulletE{
public:
	//�R���X�g���N�^
	Bullet();

	//�ړ�
	virtual void move(float frameTime);
};

class Missile :public BulletE{
public:
	//�R���X�g���N�^
	Missile();

	//�ړ�
	virtual void move(float frameTime);
};
#endif