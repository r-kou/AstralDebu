#include "bomb.h"

using namespace entityNS;
using namespace bombNS;

//コンストラクタ
BombE::BombE(){
	state = STAND;
	size = IMG_SIZE;
	col = IMG_COL;
	edge_x = EDGE_X;
	edge_y = EDGE_Y;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
}

//移動
void BombE::move(float frameTime){
	//空中にいるなら落下
	if (state == JUMP) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	Entity::move(frameTime);
}

//地形への接触
void BombE::collideMap(UCHAR t){
	//とりあえず爆発する
	if ((t & LEFT) && (vel.x < 0.0f)) {
		setLeft(false);
		vel.x = 0.0f;
		state = DEAD;
	}
	if ((t & RIGHT) && (vel.x > 0.0f)) {
		setRight(false);
		vel.x = 0.0f;
		state = DEAD;
	}
	if ((t & TOP) && (vel.y < 0.0f)) {
		setTop(false);
		vel.y = 0.0f;
		state = DEAD;
	}
	if (t & BOTTOM) {
		//空中にいたら着地判定
		if ((vel.y > 0.0f) && ((state == KNOCK) || (state == JUMP))){
			state = DEAD;
		}
	}
	else if ((state != KNOCK) && (state != DEAD)) {
		//下に何もないなら落下
		state = JUMP;
	}
}

//他オブジェクトへの接触
void BombE::collideObj(Entity *e, UCHAR t){
	//爆弾はとりあえず爆発する
	switch (e->getType()){
	case BOX_W:
	case BOX_S:
	case BOX_L:
	case BOX_B:
	case BOX_H:
	case BOX_A:
	case BOX_F:
	case ROCK:
		//静止してたら止まる
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == JUMP)) setRes(4);
	case BOMB:
	case HIBOMB:
	case HAMMER:
		//移動してたら爆発する
		if (((t & LEFT) && (diffVelX(e) < 0)) ||
			((t & RIGHT) && (diffVelX(e) > 0)) ||
			((t & TOP) && (diffVelY(e) < 0)) ||
			((t & BOTTOM) && (diffVelY(e) > 0) && (state == JUMP || state == KNOCK))) setRes(6);
		break;
	case EN_1:
	case EN_2:
	case EN_3:
	case EN_4:
	case EN_5:
	case EN_B:
		//左右にはこっちが動いていたら、上下は問答無用で爆発する
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			(t & TOP) || (t & BOTTOM)) setRes(6);
		break;
	case BLAST:
		//爆風は速度とか関係ない
		setRes(6);
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

}

//コンストラクタ
Bomb::Bomb(){
	type = BOMB;
	img = IMG_B;
}

//コンストラクタ
Hibomb::Hibomb(){
	type = HIBOMB;
	img = IMG_H;
}
