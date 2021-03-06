#ifndef _DEBU_H
#define _DEBU_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace debuNS{
	const float VEL_RATE = 900;
	const float VEL_MAX_WALK = 145;
	const float VEL_JUMP = -295;
	const float VEL_DUMP = 3000;
	const float VEL_BREAK = 2000;
	const float VEL_RATE_JUMP = 700;
	const float VEL_LADDER = 100;
	const float VEL_BOMB_X = 350;
	const float VEL_BOMB_Y = 250;
	const float VEL_KNOCK_X = 250;

	const int IMG_STAND = 0;
	const int IMG_HOLD = 1;
	const int IMG_WALK_START = 2;
	const int IMG_WALK_END = 4;
	const int IMG_HOLD_WALK_START = 5;
	const int IMG_HOLD_WALK_END = 7;
	const int IMG_THROW = 8;
	const int IMG_FALL_FRONT = 9;
	const int IMG_FALL_BACK = 10;
	const int IMG_JUMP_FRONT = 11;
	const int IMG_JUMP_BACK = 12;
	const int IMG_FALL_HOLD_FRONT = 13;
	const int IMG_FALL_HOLD_BACK = 14;
	const int IMG_JUMP_HOLD_FRONT = 15;
	const int IMG_JUMP_HOLD_BACK = 16;
	const int IMG_DEAD_START = 17;
	const int IMG_DEAD_END = 21;
	const int IMG_LADDER = 22;
	const int IMG_LADDER_START = 23;
	const int IMG_LADDER_END = 24;
	const int IMG_CLEAR_START = 25;
	const int IMG_CLEAR_END = 27;

	const int EDGE_X = 11;
	const int EDGE_Y = entityNS::EDGE_MAX;
	const int EDGE_MAR_X = 5;
	const int EDGE_MAR_Y = 4;
}

class Debu :public Entity{
private:
	//入力
	Input *input;
	//オブジェクトを所持しているか
	bool hold;
	//ハンマー専用の向き
	bool directHammer;
	//ハンマーを所持しているか
	bool hammer;
public:
	//コンストラクタ
	Debu();

	//初期化 入力も入れる
	bool initialize(Game *game, Texture *t, Input *in, int i, int j);

	//移動
	virtual void move(float frameTime);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//他オブジェクトへの反応
	virtual void responseObj();

	//描画する画像を変更
	virtual void changeImage();

	//描画
	virtual void draw();

	//立つ
	virtual void setStand() { state = (hammer ? entityNS::ST_HAMMER : entityNS::ST_STAND); }

	//setter
	void setHold(bool h) { hold = h; }
	void setHammerHold(bool d) { hammer = d; }
	void setHammerDirect(bool d) { directHammer = d; }

	//getter
	bool getHold() { return hold; }
	bool getHammerHold() { return hammer; }
	bool getHammerDirect() { return directHammer; }
};

#endif