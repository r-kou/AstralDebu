#include "bomb.h"

using namespace entityNS;
using namespace bombNS;

//コンストラクタ
BombE::BombE(){
	state = STAND;
	renderOrder = RO_OBJECT;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}

//移動
void BombE::move(float frameTime){
	//空中にいるなら落下
	if (state == JUMP) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_MAX) vel.y = VEL_MAX;

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
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
	case FRAME_BOX:
	case GOAST_BOX:
	case ROCK:
		//静止してたら止まる
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == JUMP)) setRes(RES_BOTTOM);
	case BOMB:
	case HIBOMB:
	case HAMMER:
		//移動してたら爆発する
		if (((t & LEFT) && (diffVelX(e) < 0)) ||
			((t & RIGHT) && (diffVelX(e) > 0)) ||
			((t & TOP) && (diffVelY(e) < 0)) ||
			((t & BOTTOM) && (diffVelY(e) > 0) && (state == JUMP || state == KNOCK))) setRes(RES_DEAD);
		break;
	case EN_1:
	case EN_2:
	case EN_3:
	case EN_4:
	case EN_5:
	case BULLET:
	case MISSILE:
		//左右にはこっちが動いていたら、上下は問答無用で爆発する
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			(t & TOP) || (t & BOTTOM)) setRes(RES_DEAD);
		break;
	case BLAST:
		//爆風は速度とか関係ない
		setRes(RES_BOTTOM);
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

}

//コンストラクタ
Bomb::Bomb(){
	type = BOMB;
	img = IMG_BOMB;
}

//コンストラクタ
Hibomb::Hibomb(){
	type = HIBOMB;
	img = IMG_HIBOMB;
}

//コンストラクタ
Mine::Mine(){
	state = STAND;
	type = MINE;
	renderOrder = RO_OBJECT;
	img = 33;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}

//他オブジェクトへの接触
void Mine::collideObj(Entity *e, UCHAR t){
	//機雷は本当になんでも爆発する
	setRes(RES_BOTTOM);
}