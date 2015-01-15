#ifndef _DEBU_H
#define _DEBU_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace debuNS{
	const float VEL_RATE = 800;
	const float VEL_MAX = 140;
	const float VEL_JUMP = -300;
	const float VEL_DUMP = 2000;
	const float VEL_RATE_JUMP = 600;
	const float VEL_LADDER = 100;
	const float VEL_BOMB_X = 350;
	const float VEL_BOMB_Y = 250;
	const float VEL_BOMB_M = 150;
	const float VEL_ENEMY_X = 250;
	const float VEL_ENEMY_Y = -200;

	const int IMG_SIZE = 32;
	const int IMG_COL = 7;
	const int IMG_STAND = 0;
	const int IMG_HOLD = 1;
	const int IMG_WALK_S = 2;
	const int IMG_WALK_E = 4;
	const int IMG_HOLD_WALK_S = 5;
	const int IMG_HOLD_WALK_E = 7;
	const int IMG_FALL_F = 9;
	const int IMG_FALL_B = 10;
	const int IMG_JUMP_F = 11;
	const int IMG_JUMP_B = 12;
	const int IMG_FALL_HOLD_F = 13;
	const int IMG_FALL_HOLD_B = 14;
	const int IMG_JUMP_HOLD_F = 15;
	const int IMG_JUMP_HOLD_B = 16;
	const int IMG_DEAD_S = 17;
	const int IMG_DEAD_E = 21;
	const int IMG_LADDER = 22;
	const int IMG_LADDER_S = 23;
	const int IMG_LADDER_E = 24;
	const int IMG_CLEAR_S = 25;
	const int IMG_CLEAR_E = 27;

	const int EDGE_X = 10;
	const int EDGE_Y = entityNS::EDGE_MAX;
	const int EDGE_MAR_X = 4;
	const int EDGE_MAR_Y = 6;
}

class Debu :public Entity{
private:
	//入力
	Input *input;
	//オブジェクトを所持しているか
	bool hold;
	//ハンマー専用の向き
	bool direct_h;

	//はしごに捕まる
	void getLadder();
public:
	//コンストラクタ
	Debu();

	//初期化 入力も入れる
	bool initialize(Game *game, Texture *t, Input *in, int i, int j);

	//移動
	virtual void move(float frameTime);

	//上下左右の衝突判定
	virtual UCHAR touchMapDirect(int c, UCHAR t);

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//他オブジェクトへの反応
	virtual void responseObj();

	//描画する画像を変更
	virtual void changeImage();

	//setter
	void setHold(bool h) { hold = h; }
	void setHammer(bool d) { direct_h = d; }

	//getter
	bool getHold() { return hold; }
	bool getHammer() { return direct_h; }
};

#endif