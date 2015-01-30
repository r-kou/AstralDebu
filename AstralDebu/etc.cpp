#include "etc.h"

using namespace entityNS;
using namespace etcNS;

//コンストラクタ
Rock::Rock(int stage){
	state = STAND;
	type = ROCK;
	renderOrder = RO_BASE;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_ROCK_X;
	edgeY = EDGE_ROCK_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	//ステージに応じて画像変更
	if (stage < 10) img = IMG_ROCK_AREA1;
	else if (stage < 20) img = IMG_ROCK_AREA2;
	else img = IMG_ROCK_AREA3;
}

//他オブジェクトへの接触
void Rock::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//岩固有の衝突判定
	switch (e->getType()){
	case BLAST:
	case HAMMER:
		//爆発する
		setRes(6);
		break;
	}
}

//コンストラクタ
Meat::Meat(){
	state = STAND;
	type = MEAT;
	renderOrder = RO_OBJECT;
	size = IMG_SIZE;
	col = IMG_COL;
	img = IMG_MEAT;
}

//コンストラクタ
Himeat::Himeat(){
	state = STAND;
	type = HIMEAT;
	renderOrder = RO_OBJECT;
	size = IMG_SIZE;
	col = IMG_COL;
	img = IMG_HIMEAT;
}


//コンストラクタ
Hammer::Hammer(){
	state = STAND;
	type = HAMMER;
	renderOrder = RO_OBJECT;
	size = IMG_SIZE;
	col = IMG_COL;
	img = IMG_HAMMER;
	edgeX = EDGE_HAMMER_X;
	edgeY = EDGE_HAMMER_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}


//移動
void Hammer::move(float frameTime){
	if (state == HOLD_HAMMER){
		if (pos.x >= getRight(false)) {
			vel.x = -VEL_HAM;
			renderOrder = RO_HAMMER;
		}
		else if (pos.x <= getLeft(false)) {
			vel.x = VEL_HAM;
			renderOrder = RO_HOLD;
		}

		//速度分だけ移動
		pos.x += (vel.x * frameTime);

		//判定の位置とかは同じ
	}
	else {
		//空中にいるなら落下
		if (state == JUMP) vel.y += GRAVITY_RATE * frameTime;
		if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

		Entity::move(frameTime);
	}
}


//地形への接触判定
void Hammer::touchMap(int map[MAP_COL][MAP_ROW]){
	//所持状態なら当たらない
	if (state == HOLD_HAMMER) return;

	Entity::touchMap(map);
}

//地形への接触
void Hammer::collideMap(UCHAR t){
	Entity::collideMap(t);

	if ((t & BOTTOM) && (state == STAND)) {
		//地面でぴったり止まる
		setCX();
		vel.x = 0.0f;
	}
}

//他オブジェクトへの接触判定
void Hammer::touchObj(Entity *e){
	//所持状態なら当たらない
	if (state == HOLD_HAMMER) return;

	Entity::touchObj(e);
}

//他オブジェクトへの接触
void Hammer::collideObj(Entity *e, UCHAR t){
	//所持状態ならマップには当たらない
	if (state == HOLD_HAMMER) return;

	//鉄球類固有の衝突判定　ほとんどのオブジェクトを貫通する
	switch (e->getType()){
	case LEAD_BOX:
	case HIBOMB_BOX:
	case GOAST_BOX:
		//停止するのは鉛箱と霊箱だけ　それ以外は全て粉砕する
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(0);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(1);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(2);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == JUMP || state == LADDER)) ||
			((diffVelY(e) > 0) && (state == KNOCK)))) setRes(4); //空中にいたら着地判定
		break;
	case RED_WARP:
	case GREEN_WARP:
	case YELLOW_WARP:
		//ワープは可能
		if (warpInterval == 0.0f) setRes(5, (e->getPartnerX() + 0.5f) * CHIP_SIZE, (e->getPartnerY() + 0.5f) * CHIP_SIZE + DATA_LEN);
		break;
	}
}

//持つ処理
void Hammer::setHold(Debu *d){
	d->setState(HOLD_HAMMER);
	d->setVelX(0);
	d->setVelY(0);

	direct = d->getDirect();
	state = HOLD_HAMMER;
	if (direct) setPosX(d->getPosX() - HAMMER_MAR_X);
	else setPosX(d->getPosX() + HAMMER_MAR_X);
	setPosY(d->getPosY() - HAMMER_MAR_Y);
	setVelX(VEL_HAM);
	setVelY(0);

	SETRECT(edge, (long)d->getPosX() - EDGE_HAMMER_HOLDED_X, (long)d->getPosY() - EDGE_HAMMER_Y, EDGE_HAMMER_HOLDED_X * 2, EDGE_HAMMER_Y * 2);
}