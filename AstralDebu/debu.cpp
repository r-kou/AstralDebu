#include "debu.h"

using namespace entityNS;
using namespace debuNS;

//コンストラクタ
Debu::Debu(){
	state = ST_STAND;
	type = TY_DEBU;
	renderOrder = RO_DEBU;
	size = CHIP_SIZE;
	col = COL_DEBU;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	putSound = audioNS::PUT_DEBU;
}

//初期化
bool Debu::initialize(Game *game, Texture *t, Input *in, int i, int j){
	//入力を受け付ける
	input = in;
	hold = false;
	return Entity::initialize(game, t, i, j);
}

//移動
void Debu::move(float frameTime){
	if (input->isKeyDown(VK_LEFT)) {
		if (!input->isKeyDown(VK_RIGHT)){
			//左キーのみの状態
			switch (state){
			case ST_STAND:
				//切り返しは早い
				if (vel.x <= 0) vel.x -= VEL_RATE * frameTime;
				else vel.x -= VEL_DUMP * frameTime;
				setDirect(true);
				break;
			case ST_JUMP:
				//空中制御はやや緩やか
				if (vel.x > -VEL_MAX_WALK)
					vel.x -= VEL_RATE_JUMP * frameTime;
				break;
			case ST_LADDER:
				//はしごでの速度は固定
				vel.x = -VEL_LADDER;
				setDirect(true);
				break;
			case ST_HAMMER:
				//移動不可
				setDirect(true);
				break;
			}
		}
	}
	else {
		if (input->isKeyDown(VK_RIGHT)){
			//右キーのみの状態
			switch (state){
			case ST_STAND:
				if (vel.x >= 0) vel.x += VEL_RATE * frameTime;
				else vel.x += VEL_DUMP * frameTime;
				setDirect(false);
				break;
			case ST_JUMP:
				if (vel.x < VEL_MAX_WALK)
					vel.x += VEL_RATE_JUMP * frameTime;
				break;
			case ST_LADDER:
				vel.x = VEL_LADDER;
				setDirect(false);
				break;
			case ST_HAMMER:
				//移動不可
				setDirect(false);
				break;
			}
		}
		else {
			//キー入力無し
			switch (state){
			case ST_STAND:
				//ブレーキをかける
				vel.x *= 0.9f;
				if (fabs(vel.x) < 10) vel.x = 0.0f;
				break;
			case ST_LADDER:
				//梯子はすぐに止まる
				vel.x = 0.0f;
				break;
			}
		}
	}

	//梯子は上下移動可能
	if (state == ST_LADDER){
		if (input->isKeyDown(VK_UP)){
			if (!input->isKeyDown(VK_DOWN)) vel.y = -VEL_LADDER;
		}
		else{
			if (input->isKeyDown(VK_DOWN)) vel.y = VEL_LADDER;
			//離すとすぐに止まる
			else vel.y = 0.0f;
		}
	}

	//空中にいるか死亡時なら落下
	if (state == ST_JUMP || state == ST_KNOCK || state == ST_DEAD) vel.y += GRAVITY_RATE * frameTime;

	//地上では速度制限
	if (state == ST_STAND){
		if (vel.x > VEL_MAX_WALK) vel.x = VEL_MAX_WALK;
		else if (vel.x < -VEL_MAX_WALK) vel.x = -VEL_MAX_WALK;
	}
	if (vel.y > VEL_MAX) vel.y = VEL_MAX;

	//クリア時は動かない
	if (state == ST_CLEAR) {
		vel.x = 0;
		vel.y = 0;
	}

	Entity::move(frameTime);
}

//他オブジェクトへの接触
void Debu::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//デブ固有の衝突判定
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
		//箱は上に乗れる 横にはすり抜ける 霊箱は当たらない
		if ((!input->isKeyDown(VK_DOWN))&&
			(t & BOTTOM) && ((diffBottom(e, true) <= 0) &&
			(((diffVelY(e) >= 0) && (state == ST_JUMP || (state == ST_LADDER))) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK))))) setRes(RES_BOTTOM);
		break;
	case TY_LADDER:
		setRes(RES_LADDER);
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
	case TY_BULLET:
		//吹っ飛ばされる
		if (knockInterval == 0.0f){
			setRes(RES_KNOCK, getPosX() > e->getPosX() ? VEL_KNOCK_X : -VEL_KNOCK_X, -VEL_KNOCK_JUMP);
			knockInterval = 0.3f;
		}
		break;
	case TY_MISSILE:
	case TY_BLAST:
		//吹っ飛ばされる
		setRes(RES_JUMP, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y));
		break;
	case TY_GOAL:
		//クリアする
		setRes(RES_CLEAR);
		break;
	}
}

//描画する画像を変更
void Debu::changeImage(){
	//現在の状態を確認
	switch (state){
	case ST_STAND:
		image.setFlipH(direct);
		//動いているか停止しているかで判断
		if (animInterval > 0.0f) setImage(IMG_THROW);
		else if (vel.x == 0.0f){
			//オブジェクトを保持しているかで判断
			if (hold) setImage(IMG_HOLD);
			else setImage(IMG_STAND);
		}
		else {
			if (hold) setImage(IMG_HOLD_WALK_START, IMG_HOLD_WALK_END, true);
			else setImage(IMG_WALK_START, IMG_WALK_END, true);
		}
		break;
	case ST_JUMP:
		//下降中
		if (vel.y > 0){
			//前後移動で判断
			if (vel.x*(direct?-1:1) >= 0) {
				if (hold) setImage(IMG_FALL_HOLD_FRONT);
				else setImage(IMG_FALL_FRONT);
			}
			else {
				if (hold) setImage(IMG_FALL_HOLD_BACK);
				else setImage(IMG_FALL_BACK);
			}
		}
		//上昇中
		else {
			//前後移動で判断
			if (vel.x*(direct ? -1 : 1) >= 0)	{
				if (hold) setImage(IMG_JUMP_HOLD_FRONT);
				else setImage(IMG_JUMP_FRONT);
			}
			else {
				if (hold) setImage(IMG_JUMP_HOLD_BACK);
				else setImage(IMG_JUMP_BACK);
			}
		}
		break;
	case ST_LADDER:
		//動いているか停止しているかで判断 向きの区別は無し
		if (vel.x == 0.0f && vel.y == 0.0f) setImage(IMG_LADDER);
		else setImage(IMG_LADDER_START, IMG_LADDER_END, true);
		break;
	case ST_HAMMER:
		image.setFlipH(directHammer);
		setImage(IMG_HOLD);
		break;
	case ST_CLEAR:
		//アニメが終了したらクリア
		image.setDelay(0.33f);
		setImage(IMG_CLEAR_START, IMG_CLEAR_END, false);
		break;
	case ST_DEAD:
		//アニメが終了したらやり直し
		image.setDelay(0.25f);
		setImage(IMG_DEAD_START, IMG_DEAD_END, false);
		break;
	}


}

//他オブジェクトへの反応
void Debu::responseObj(){
	Entity::responseObj();
	if (getRes(RES_LADDER)){
		if (input->isKeyDown('Z') || hold || state == ST_KNOCK || state == ST_DEAD || state == ST_CLEAR) return;
		state = ST_LADDER;
	}
}

//描画
void Debu::draw(){
	//半透明ブロックか、後ろが梯子とかなら半透明
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}


