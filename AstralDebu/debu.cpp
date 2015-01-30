#include "debu.h"

using namespace entityNS;
using namespace debuNS;

//コンストラクタ
Debu::Debu(){
	state = STAND;
	type = DEBU;
	renderOrder = RO_DEBU;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
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
			case STAND:
				//切り返しは早い
				if (vel.x <= 0) vel.x -= VEL_RATE * frameTime;
				else vel.x -= VEL_DUMP * frameTime;
				setDirect(true);
				break;
			case JUMP:
				//空中制御はやや緩やか
				if (vel.x > -VEL_MAX)
					vel.x -= VEL_RATE_JUMP * frameTime;
				break;
			case LADDER:
				//はしごでの速度は固定
				vel.x = -VEL_LADDER;
				setDirect(true);
				break;
			case HOLD_HAMMER:
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
			case STAND:
				if (vel.x >= 0) vel.x += VEL_RATE * frameTime;
				else vel.x += VEL_DUMP * frameTime;
				setDirect(false);
				break;
			case JUMP:
				if (vel.x < VEL_MAX)
					vel.x += VEL_RATE_JUMP * frameTime;
				break;
			case LADDER:
				vel.x = VEL_LADDER;
				setDirect(false);
				break;
			case HOLD_HAMMER:
				//移動不可
				setDirect(false);
				break;
			}
		}
		else {
			//キー入力無し
			switch (state){
			case STAND:
				//ブレーキをかける
				vel.x *= 0.9f;
				if (fabs(vel.x) < 10) vel.x = 0.0f;
				break;
			case LADDER:
				//梯子はすぐに止まる
				vel.x = 0.0f;
				break;
			}
		}
	}

	//梯子は上下移動可能
	if (state == LADDER){
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
	if (state == JUMP || state == DEAD) vel.y += GRAVITY_RATE * frameTime;

	//地上では速度制限
	if (state == STAND){
		if (vel.x > VEL_MAX) vel.x = VEL_MAX;
		else if (vel.x < -VEL_MAX) vel.x = -VEL_MAX;
	}
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	//クリア時は動かない
	if (state == CLEAR) {
		vel.x = 0;
		vel.y = 0;
	}

	Entity::move(frameTime);
}

//上下左右の衝突判定
UCHAR Debu::touchMapDirect(int c, UCHAR t){
	//梯子に捕まる判定あり
	if (c == 0) return 0;
	else if (c == CHIP_LADDER) {
		if (t & (TOP | BOTTOM)) return GETLADDER;
		else return 0;
	}
	else return t;
}

//地形への接触
void Debu::collideMap(UCHAR t){
	Entity::collideMap(t);

	if (t & GETLADDER){
		//はしごに捕まる
		getLadder();
	}
}

//他オブジェクトへの接触
void Debu::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//デブ固有の衝突判定
	switch (e->getType()){
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
		//箱は上に乗れる 横にはすり抜ける 霊箱は当たらない
		if ((t & BOTTOM) && ((diffBottom(e, true) <= marginY) &&
			(((diffVelY(e) >= 0) && (state == JUMP || (state == LADDER))) ||
			((diffVelY(e) > 0) && (state == KNOCK))))) setRes(3);
		break;
	case EN_1:
	case EN_2:
	case EN_3:
	case EN_4:
	case EN_5:
	case BULLET:
		//吹っ飛ばされる
		setRes(7, getPosX() > e->getPosX() ? VEL_ENEMY_X : -VEL_ENEMY_X, VEL_ENEMY_Y);
		break;
	case MISSILE:
	case BLAST:
		//吹っ飛ばされる
		setRes(8, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y, VEL_BOMB_M));
		break;
	case GOAL:
		//クリアする
		setRes(9);
		break;
	}
}

//他オブジェクトへの反応
void Debu::responseObj(){
	Entity::responseObj();

	if (getRes(7)){
		state = JUMP;
		vel.x = getResX(7);
		vel.y = getResY(7);
	}
	if (getRes(8)){
		state = JUMP;
		vel.x = getResX(8);
		vel.y = getResY(8);
	}
	if (getRes(9)){
		state = CLEAR;
		vel.x = 0.0f;
		vel.y = 0.0f;
	}
}
//描画する画像を変更
void Debu::changeImage(){
	//現在の状態を確認
	switch (state){
	case STAND:
		image.setFlipH(direct);
		//動いているか停止しているかで判断
		if (vel.x == 0.0f){
			//オブジェクトを保持しているかで判断
			if (hold) setImage(IMG_HOLD);
			else setImage(IMG_STAND);
		}
		else {
			if (hold) setImage(IMG_HOLD_WALK_START, IMG_HOLD_WALK_END, true);
			else setImage(IMG_WALK_START, IMG_WALK_END, true);
		}
		break;
	case JUMP:
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
	case LADDER:
		//動いているか停止しているかで判断 向きの区別は無し
		if (vel.x == 0.0f && vel.y == 0.0f) setImage(IMG_LADDER);
		else setImage(IMG_LADDER_START, IMG_LADDER_END, true);
		break;
	case HOLD_HAMMER:
		image.setFlipH(directHammer);
		setImage(IMG_HOLD);
		break;
	case CLEAR:
		//アニメが終了したらクリア
		image.setDelay(0.33f);
		setImage(IMG_CLEAR_START, IMG_CLEAR_END, false);
		break;
	case DEAD:
		//アニメが終了したらやり直し
		image.setDelay(0.25f);
		setImage(IMG_DEAD_START, IMG_DEAD_END, false);
		break;
	}


}

//はしごに捕まる　デブ専用
void Debu::getLadder(){
	//ジャンプしたり持ってると捕まれない
	if (input->isKeyDown('Z') || hold || state == DEAD || state == CLEAR) return;
	state = LADDER;
}