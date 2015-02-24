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
	//コンストラクタ
	BombE();

	//移動
	//virtual void move(float frameTime);

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};

class Bomb : public BombE{
public:
	//コンストラクタ
	Bomb();
};

class Hibomb : public BombE{
public:
	//コンストラクタ
	Hibomb();
};

class Mine : public Entity {
public:
	//コンストラクタ
	Mine();

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};
#endif