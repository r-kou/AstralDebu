#ifndef _BOMB_H
#define _BOMB_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace bombNS{
	const int IMG_B = 25;
	const int IMG_H = 26;

	const int IMG_SIZE = 32;
	const int IMG_COL = 10;
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
	virtual void move(float frameTime);

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

#endif