#include "etc.h"

using namespace entityNS;
using namespace etcNS;

//コンストラクタ
Rock::Rock(int stage){
	state = ST_STAND;
	type = TY_ROCK;
	renderOrder = RO_BASE;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_ROCK_X;
	edgeY = EDGE_ROCK_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	deadSound = audioNS::BRAKE_ROCK;
	chipTop = false;
	chipBottom = false;
	//ステージに応じて画像変更
	if (stage < 10) img = IMG_ROCK_AREA1;
	else if (stage < 20) img = IMG_ROCK_AREA2;
	else if (stage <= 30) img = IMG_ROCK_AREA3;
	else img = IMG_ROCK_AREA4;
}

//他オブジェクトへの接触
void Rock::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//岩固有の衝突判定
	switch (e->getType()){
	case TY_BLAST:
		//爆発する
		setResponse(RES_DEAD);
		break;
	case TY_HAMMER:
		//鉄球からぶつかってきたら壊れる
		if ((e->getState() == ST_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setResponse(RES_DEAD);
		}
		break;
	}
}

//描画
void Rock::draw(){
	//書く
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}


//地形をチェックして岩の画像を決める
void Rock::checkMap(int map[MAP_COL][MAP_ROW]){
	int cx = ChipCX();
	int cy = ChipCY();

	if (cy == 0) chipTop = true;
	else chipTop = TOBOOL(map[cx][cy - 1]);

	if (cy == MAP_ROW) chipBottom = true;
	else chipBottom = TOBOOL(map[cx][cy + 1]);
}

//他の岩をチェックして岩の画像を決める
void Rock::checkObj(Entity *e){
	//形は上下の岩でのみ決まる
	if ((e->getType() != TY_ROCK)||
		(ChipCX()!=e->ChipCX())) return;
	if (ChipCY() + 1 == e->ChipCY()) chipBottom = true;
	else if(ChipCY() - 1 == e->ChipCY()) chipTop = true;
}

//描画する画像を決める
void Rock::setImage(){
	if (chipBottom){
		if (chipTop) Entity::setImage(img);
		else Entity::setImage(img + 1);
	}
	else {
		if (chipTop) Entity::setImage(img + 2);
		else Entity::setImage(img + 3);
	}
}

//コンストラクタ
Ladder::Ladder(){
	state = ST_STAND;
	type = TY_LADDER;
	renderOrder = RO_BASE;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_LADDER_X;
	edgeY = EDGE_LADDER_Y;
	img = IMG_LADDER;
}

//描画
void Ladder::draw(){
	//書く
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}

//コンストラクタ
MeatE::MeatE(){
	state = ST_STAND;
	renderOrder = RO_OBJECT;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_MEAT_X;
	edgeY = EDGE_MEAT_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	fall = true;
}

//地形への接触
void MeatE::collideMap(UCHAR t){
	Entity::collideMap(t);
	if ((t & BOTTOM) && (state == ST_STAND)) {
		//地面でぴったり止まる
		setCX();
		vel.x = 0.0f;
	}
}

//他オブジェクトへの接触
void MeatE::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//箱類固有の衝突判定
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_GOAST_BOX:
	case TY_HAMMER:
		//箱の上に乗る
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK)))) {
			//空中にいたら着地判定
			setResponse(RES_BOTTOM_CHIP);
		}
		break;
	}
}

//コンストラクタ
Meat::Meat(){
	type = TY_MEAT;
	img = IMG_MEAT;
}

//コンストラクタ
Himeat::Himeat(){
	type = TY_HIMEAT;
	img = IMG_HIMEAT;
}

//コンストラクタ
Hammer::Hammer(){
	state = ST_STAND;
	type = TY_HAMMER;
	renderOrder = RO_OBJECT;
	size = CHIP_SIZE;
	col = COL_CHIP;
	img = IMG_HAMMER;
	edgeX = EDGE_HAMMER_X;
	edgeY = EDGE_HAMMER_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	fall = true;
	basePos.x = 0.0f;
	basePos.y = 0.0f;
}


//移動
void Hammer::move(float frameTime){
	if (state == ST_HAMMER){
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
		Entity::move(frameTime);
	}
}

//地形への接触判定
void Hammer::touchMap(int map[MAP_COL][MAP_ROW]){
	//所持状態なら当たらない
	if (state == ST_HAMMER) return;

	Entity::touchMap(map);
}

//地形への接触
void Hammer::collideMap(UCHAR t){
	Entity::collideMap(t);

	if ((t & BOTTOM) && (state == ST_STAND)) {
		//地面でぴったり止まる
		setCX();
		vel.x = 0.0f;
	}
}

//他オブジェクトへの接触
void Hammer::collideObj(Entity *e, UCHAR t){
	//所持状態なら当たらない
	if (state == ST_HAMMER) return;

	//鉄球類固有の衝突判定　ほとんどのオブジェクトを貫通する
	switch (e->getType()){
	case TY_LEAD_BOX:
	case TY_HIBOMB_BOX:
	case TY_GOAST_BOX:
		//停止するのは鉛箱と霊箱だけ　それ以外は全て粉砕する
		if ((t & LEFT) && (diffVelX(e) < 0)) setResponse(RES_LEFT);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setResponse(RES_RIGHT);
		if ((t & TOP) && (diffVelY(e) < 0)) setResponse(RES_TOP);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP || state == ST_LADDER)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK)))) setResponse(RES_BOTTOM_CHIP); //空中にいたら着地判定
		break;
	case TY_RED_WARP:
	case TY_GREEN_WARP:
	case TY_YELLOW_WARP:
		//ワープは可能
		if (warpInterval == 0.0f) setResponse(RES_WARP, CHIP(e->getPartnerX() + 0.5f), CHIP_D(e->getPartnerY() + 0.5f));
	case TY_LADDER:
	case TY_GOAL:
		//半透明になる
		trans = true;
		break;
	}
}

//ワープの是非を判定
void Hammer::collideWarp(Entity *e){
	Entity::collideWarp(e);

	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_GOAST_BOX:
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_HAMMER:
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		resetResponse(RES_WARP);
		break;
	}
}

//持つ処理
void Hammer::setHold(Debu *d){
	d->setState(ST_HAMMER);
	d->setHammerHold(true);
	d->setVelX(0);
	d->setVelY(0);

	direct = d->getDirect();
	state = ST_HAMMER;
	setBasePos(pos);
	if (direct) setPosX(d->getPosX() - HAMMER_MAR_X);
	else setPosX(d->getPosX() + HAMMER_MAR_X);
	setPosY(d->getPosY() - HAMMER_MAR_Y);
	setVelX(VEL_HAM);
	setVelY(0);

	SETRECT(edge, (long)d->getPosX() - EDGE_HAMMER_HOLDED_X, (long)d->getPosY() - EDGE_HAMMER_HOLDED_Y, EDGE_HAMMER_HOLDED_X * 2, EDGE_HAMMER_HOLDED_Y * 2);
}