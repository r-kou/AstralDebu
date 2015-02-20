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
	//デブ
	Debu *debu;
public:
	//コンストラクタ
	Enemy();

	//初期化 デブの位置情報も入れる
	bool initialize(Game *game, Texture *t, Debu *d, int i, int j);

	//移動
	virtual void move(float frameTime);

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//描画する画像を変更
	virtual void changeImage();
};

class Enemy1 : public Enemy{
private:
public:
	//コンストラクタ
	Enemy1();

	//移動
	virtual void move(float frameTime);
};

class Enemy2 : public Enemy{
private:
public:
	//コンストラクタ
	Enemy2();

	//移動
	virtual void move(float frameTime);
};

class Enemy3 : public Enemy{
private:
public:
	//コンストラクタ
	Enemy3();

	//移動
	virtual void move(float frameTime);

	//描画する画像を変更
	virtual void changeImage();
};


class Enemy4 : public Enemy{
private:
public:
	//コンストラクタ
	Enemy4();

	//移動
	virtual void move(float frameTime);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//描画する画像を変更
	virtual void changeImage();
};


class Enemy5 : public Enemy{
private:
public:
	//コンストラクタ
	Enemy5();

	//移動
	virtual void move(float frameTime);

	//描画する画像を変更
	virtual void changeImage();
};

class BulletE :public Entity{
public:
	//コンストラクタ
	BulletE();

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//描画する画像を変更
	virtual void changeImage();
};

class Bullet :public BulletE{
public:
	//コンストラクタ
	Bullet();

	//移動
	virtual void move(float frameTime);
};

class Missile :public BulletE{
public:
	//コンストラクタ
	Missile();

	//移動
	virtual void move(float frameTime);
};
#endif