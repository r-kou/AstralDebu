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
	//コンストラクタ ステージによって色が変わる
	Rock(int stage);

	//地形への接触判定
	virtual void touchMap(int map[MAP_COL][MAP_ROW]);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//描画する画像を変更
	virtual void changeImage();

	//描画
	virtual void draw();
};

class Ladder :public Entity {
public:
	//コンストラクタ
	Ladder();

	//地形への接触（何もしない）
	virtual void collideMap(UCHAR t){}

	//他オブジェクトへの接触（何もしない）
	virtual void collideObj(Entity *e, UCHAR t){}

	//描画
	virtual void draw();
};

class MeatE : public Entity{
public:
	//コンストラクタ
	MeatE();

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

};
class Meat : public MeatE{
public:
	//コンストラクタ
	Meat();
};

class Himeat : public MeatE{
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