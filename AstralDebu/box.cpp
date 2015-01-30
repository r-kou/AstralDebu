#include "box.h"

using namespace entityNS;
using namespace boxNS;

//コンストラクタ
Box::Box(){
	state = STAND;
	renderOrder = RO_BOX;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}

//移動
void Box::move(float frameTime){
	//空中にいるなら落下
	if (state == JUMP) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	Entity::move(frameTime);
}

//地形への接触
void Box::collideMap(UCHAR t){
	Entity::collideMap(t);
	if ((t & BOTTOM) && (state == STAND)) {
		//地面でぴったり止まる
		setCX();
		vel.x = 0.0f;
	}
}

//他オブジェクトへの接触
void Box::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//箱類固有の衝突判定
	switch (e->getType()){
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
	case FRAME_BOX:
	case GOAST_BOX:
	case HAMMER:
		//箱同士はぶつかる
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(0);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(1);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(2);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == JUMP || state == LADDER)) ||
			((diffVelY(e) > 0) && (state == KNOCK)))) setRes(4); //空中にいたら着地判定
		break;
	}
}

//コンストラクタ
WoodBox::WoodBox(){
	type = WOOD_BOX;
	img = IMG_WOOD_BOX;
}

//他オブジェクトへの接触
void WoodBox::collideObj(Entity *e, UCHAR t){
	//木箱固有の衝突判定
	switch (e->getType()){
	case BLAST:
		//爆発で壊れる
		setRes(6);
		break;
	case HAMMER:
		//鉄球からぶつかってきたら壊れる
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Box::collideObj(e, t);
}

//コンストラクタ
SteelBox::SteelBox(){
	type = STEEL_BOX;
	img = IMG_STEEL_BOX;
}

//他オブジェクトへの接触
void SteelBox::collideObj(Entity *e, UCHAR t){
	//箱固有の衝突判定
	switch (e->getType()){
	case BLAST:
		//爆発で飛んでいく
		setRes(7, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y, VEL_BOMB_M));
		break;
	case HAMMER:
		//鉄球からぶつかってきたら壊れる
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Box::collideObj(e, t);
}

//他オブジェクトへの反応
void SteelBox::responseObj(){
	Entity::responseObj();

	if (getRes(7)){
		state = JUMP;
		vel.x = getResX(7);
		vel.y = getResY(7);
	}
}

//コンストラクタ
LeadBox::LeadBox(){
	type = LEAD_BOX;
	img = IMG_LEAD_BOX;
}

//他オブジェクトへの接触
void LeadBox::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Box::collideObj(e, t);

	//鉛箱固有の衝突判定はなし 最強
}


//コンストラクタ
BombBox::BombBox(){
	type = BOMB_BOX;
	img = IMG_BOMB_BOX;
}

//他オブジェクトへの接触
void BombBox::collideObj(Entity *e, UCHAR t){
	//爆弾箱固有の衝突判定
	switch (e->getType()){
	case BLAST:
		//爆発で壊れる　てか誘爆
		setRes(6);
	case HAMMER:
		//鉄球からぶつかってきたら壊れる
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Box::collideObj(e, t);
}

//コンストラクタ
HibombBox::HibombBox(){
	type = HIBOMB_BOX;
	img = IMG_HIBOMB_BOX;
}

//他オブジェクトへの接触
void HibombBox::collideObj(Entity *e, UCHAR t){
	//超爆弾箱固有の衝突判定
	switch (e->getType()){
	case BLAST:
		//爆発で壊れる 鉄球では壊れない
		setRes(6);
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Box::collideObj(e, t);
}

//コンストラクタ
AirBox::AirBox(){
	type = AIR_BOX;
	img = IMG_AIR_BOX;
}


//移動
void AirBox::move(float frameTime){
	//落ちない
	Entity::move(frameTime);
}

//他オブジェクトへの接触
void AirBox::collideObj(Entity *e, UCHAR t){
	//箱固有の衝突判定
	switch (e->getType()){
	case BLAST:
		//爆発で飛んでいく
		setRes(7, blastX(e,VEL_BOMB_X), blastY(e,VEL_BOMB_A,0));
		break;
	case HAMMER:
		//鉄球からぶつかってきたら壊れる
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Box::collideObj(e, t);
}

//他オブジェクトへの反応
void AirBox::responseObj(){
	Entity::responseObj();

	if (getRes(7)){
		state = KNOCK;
		vel.x = getResX(7);
		vel.y = getResY(7);
	}
}

//描画
void AirBox::draw(){
	//空気なので半透明
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw(D3DCOLOR_ARGB(128, 255, 255, 255));
}

//コンストラクタ
FrameBox::FrameBox(){
	type = FRAME_BOX;
	img = IMG_FRAME_BOX;
}

//地形への接触
void FrameBox::collideMap(UCHAR t){
	//とりあえず壊れる
	if (((t & LEFT) && (vel.x < 0.0f))||
		((t & RIGHT) && (vel.x > 0.0f))||
		((t & TOP) && (vel.y < 0.0f))){
		vel.x = 0.0f;
		vel.y = 0.0f;
		state = DEAD;
	}
	if (t & BOTTOM) {
		//空中にいたら着地判定　壊れる
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
void FrameBox::collideObj(Entity *e, UCHAR t){
	//フレームはとりあえず壊れる
	switch (e->getType()){
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
	case FRAME_BOX:
	case ROCK:
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == JUMP)) setRes(4);
	case BOMB:
	case HIBOMB:
	case HAMMER:
		//移動してたら壊れる
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
	case BULLET:
		//左右にはこっちが動いていたら、上下は問答無用で壊れる
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			(t & TOP) || (t & BOTTOM)) setRes(6);
		break;
	case BLAST:
		//爆風は速度とか関係ない
		setRes(6);
		break;
	case DEBU:
		//デブが乗っても壊れる
		if (t & TOP) {
			if ((diffTop(e, true) >= -marginY) &&
				((diffVelY(e) < 0) && (e->getState() == JUMP || e->getState() == LADDER || e->getState() == KNOCK))){
				setRes(6);
			}
		}
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);
}

//コンストラクタ
GoastBox::GoastBox(){
	type = GOAST_BOX;
	img = IMG_GOAST_BOX;
}

//移動
void GoastBox::move(float frameTime){
	//落ちない
	Entity::move(frameTime);
}

//他オブジェクトへの接触
void GoastBox::collideObj(Entity *e, UCHAR t){
	//霊箱固有の衝突判定
	switch (e->getType()){
	case BLAST:
		//爆発でのみ壊れる
		setRes(6);
		break;
	}

	//全オブジェクトの衝突判定をチェック
	Box::collideObj(e, t);
}

//描画
void GoastBox::draw(){
	//霊なので半透明
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw(D3DCOLOR_ARGB(128, 255, 255, 255));
}