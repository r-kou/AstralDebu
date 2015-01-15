#ifndef _ETC_H
#define _ETC_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "debu.h"

namespace etcNS{
	const float VEL_HAM = 300;

	const int IMG_R1 = 4;
	const int IMG_R2 = 9;
	const int IMG_R3 = 10;
	const int IMG_M = 31;
	const int IMG_H = 32;
	const int IMG_HAM = 27;

	const int IMG_SIZE = 32;
	const int IMG_COL = 10;
	const int EDGE_RX = entityNS::EDGE_MAX;
	const int EDGE_RY = entityNS::EDGE_MAX;
	const int EDGE_HX = entityNS::EDGE_MAX;
	const int EDGE_HY = entityNS::EDGE_MAX;
	const int EDGE_HS = entityNS::EDGE_MAX * 2;
	const int EDGE_MAR_X = 2;
	const int EDGE_MAR_Y = 2;

	const float HAMMER_MAR_X = 32;
	const float HAMMER_MAR_Y = 8;
}

class Rock : public Entity{
public:
	//コンストラクタ ステージによって色が変わる
	Rock(int stage);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};

class Meat : public Entity{
public:
	//コンストラクタ
	Meat();
};

class Himeat : public Entity{
public:
	//コンストラクタ
	Himeat();
};

class Hammer : public Entity{
public:
	//コンストラクタ
	Hammer();

	//移動
	virtual void move(float frameTime);

	//地形への接触判定
	virtual void touchMap(int map[MAP_COL][MAP_ROW]);

	//他オブジェクトへの接触判定
	virtual void touchObj(Entity *e);

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//持つ処理
	void setHold(Debu *d);
};
#endif