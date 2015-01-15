#ifndef _WARP_H
#define _WARP_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace warpNS{
	const int IMG_R = 35;
	const int IMG_G = 36;
	const int IMG_Y = 37;
	const int IMG_E = 38;
	const int IMG_W_D = 41;
	const int IMG_W_S = 42;
	const int IMG_W_E = 44;

	const int IMG_SIZE = 32;
	const int IMG_COL = 10;
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