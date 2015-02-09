#include "bomb.h"

using namespace entityNS;
using namespace bombNS;

//コンストラクタ
BombE::BombE(){
	state = ST_STAND;
	renderOrder = RO_OBJECT;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	putSound = audioNS::PUT_STEEL;
}

//移動
void BombE::move(float frameTime){
	//空中にいるなら落下
	if (state == ST_JUMP) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_MAX) vel.y = VEL_MAX;

	Entity::move(frameTime);
}

//地形への接触
void BombE::collideMap(UCHAR t){
	//とりあえず爆発する
	if ((t & LEFT) && (vel.x < 0.0f)) {
		setLeft(false);
		vel.x = 0.0f;
		state = ST_DEAD;
	}
	if ((t & RIGHT) && (vel.x > 0.0f)) {
		setRight(false);
		vel.x = 0.0f;
		state = ST_DEAD;
	}
	if ((t & TOP) && (vel.y < 0.0f)) {
		setTop(false);
		vel.y = 0.0f;
		state = ST_DEAD;
	}
	if (t & BOTTOM) {
		//空中にいたら着地判定
		if ((vel.y > 0.0f) && ((state == ST_KNOCK) || (state == ST_JUMP))){
			state = ST_DEAD;
		}
	}
	else if ((state != ST_KNOCK) && (state != ST_DEAD)) {
		//下に何もないなら落下
		state = ST_JUMP;
	}
}

//他オブジェクトへの接触
void BombE::collideObj(Entity *e, UCHAR t){
	//爆弾はとりあえず爆発する
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_GOAST_BOX:
	case TY_ROCK:
		//静止してたら止まる
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == ST_JUMP)) setRes(RES_BOTTOM);
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_HAMMER:
		//移動してたら爆発する
		if (((t & LEFT) && (diffVelX(e) < 0)) ||
			((t & RIGHT) && (diffVelX(e) > 0)) ||
			((t & TOP) && (diffVelY(e) < 0)) ||
			((t & BOTTOM) && (diffVelY(e) > 0) && (state == ST_JUMP || state == ST_KNOCK))) setRes(RES_DEAD);
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
	case TY_BULLET:
	case TY_MISSILE:
		//左右にはこっちが動いていたら、上下は問答無用で爆発する
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			(t & TOP) || (t & BOTTOM)) setRes(RES_DEAD);
		break;
	case TY_BLAST:
		//爆風は速度とか関係ない
		setRes(RES_DEAD);
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

}

//コンストラクタ
Bomb::Bomb(){
	type = TY_BOMB;
	img = IMG_BOMB;
}

//コンストラクタ
Hibomb::Hibomb(){
	type = TY_HIBOMB;
	img = IMG_HIBOMB;
}

//コンストラクタ
Mine::Mine(){
	state = ST_STAND;
	type = TY_MINE;
	renderOrder = RO_OBJECT;
	img = IMG_MINE;
	size = CHIP_SIZE;
	col = COL_CHIP;
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