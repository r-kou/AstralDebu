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
	//コンストラクタ
	Box();

	//移動
	//virtual void move(float frameTime);

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//ワープの是非を判定
	virtual void collideWarp(Entity *e);
};

class WoodBox : public Box{
public:
	//コンストラクタ
	WoodBox();

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};

class SteelBox : public Box{
public:
	//コンストラクタ
	SteelBox();

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};

class LeadBox : public Box{
public:
	//コンストラクタ
	LeadBox();

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};

class BombBox : public Box{
public:
	//コンストラクタ
	BombBox();

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};

class HibombBox : public Box{
public:
	//コンストラクタ
	HibombBox();

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};


class AirBox : public Box{
public:
	//コンストラクタ
	AirBox();

	//移動
	//virtual void move(float frameTime);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//描画
	virtual void draw();
};


class FrameBox : public Box{
public:
	//コンストラクタ
	FrameBox();

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);
};


class GoastBox : public Box {
public :
	//コンストラクタ
	GoastBox();

	//移動
	//virtual void move(float frameTime);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//描画
	virtual void draw();
};
#endif