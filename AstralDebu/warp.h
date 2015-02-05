#ifndef _WARP_H
#define _WARP_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace warpNS{
	const int IMG_RED_WARP = 48;
	const int IMG_GREEN_WARP = 49;
	const int IMG_YELLOW_WARP = 50;
	const int IMG_EMPTY_WARP = 51;
	const int IMG_GOAL_BASE = 52;
	const int IMG_GOAL_START = 53;
	const int IMG_GOAL_END = 55;

	const int EDGE_X = 2;
	const int EDGE_Y = 1;
	const int EDGE_MAR_X = 0;
	const int EDGE_MAR_Y = 0;
	const int EDGE_SLIDE = 12;
}

class Warp : public Entity{
public:
	//コンストラクタ
	Warp(entityNS::ENTITY_TYPE t);

	//描画
	virtual void draw();

	//判定の位置を設定
	virtual void setEdge();
};

class Goal : public Entity{
public:
	//コンストラクタ
	Goal();

	//初期化 色を決める
	bool initialize(Game *game, Texture *t, int i, int j);

	//描画
	virtual void draw();

	//判定の位置を設定
	virtual void setEdge();
};

#endif