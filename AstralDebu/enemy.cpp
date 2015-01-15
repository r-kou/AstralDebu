#include "enemy.h"

using namespace entityNS;
using namespace enemyNS;

//コンストラクタ
Enemy::Enemy(){
	state = STAND;
	size = IMG_SIZE;
	col = IMG_COL;
	img = 1;
	edge_x = EDGE_X;
	edge_y = EDGE_Y;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
	debu = NULL;
}

//初期化
bool Enemy::initialize(Game *game, Texture *t, Debu *d, int i, int j){
	//デブの情報を使う
	debu = d;
	return Entity::initialize(game, t, i, j);
}

//移動
void Enemy::move(float frameTime){
	//空中にいるか死亡時なら落下
	if (state == JUMP || state == DEAD) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	//歩行では床から落ちない
	if (bottom_object[0] && !bottom_object[1]) {
		setRight(true);
		if (!action) direct = true;
	}
	if (!bottom_object[0] && bottom_object[1]) {
		setLeft(true);
		if (!action) direct = false;
	}

	Entity::move(frameTime);
}

//地形への接触
void Enemy::collideMap(UCHAR t){
	//向きを変える
	if ((t & LEFT) && (vel.x <= 0.0f)&&(!action)) direct = false;
	if ((t & RIGHT) && (vel.x >= 0.0f)&&(!action)) direct = true;

	Entity::collideMap(t);
}

//他オブジェクトへの接触
void Enemy::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//敵固有の衝突判定
	switch (e->getType()){
	case BOX_W:
	case BOX_S:
	case BOX_L:
	case BOX_B:
	case BOX_H:
	case BOX_A:
	case BOX_F:
	case BOMB:
	case HIBOMB:
	case HAMMER:
		//箱にはぶつかる
		if ((t & LEFT) && (diffVelX(e) < 0)) {
			setLeft(false);
			vel.x = 0.0f;
			if (!action) direct = false;
			if (state == KNOCK) state = STAND;
		}
		if ((t & RIGHT) && (diffVelX(e) > 0)) {
			setRight(false);
			vel.x = 0.0f;
			if (!action) direct = true;
			if (state == KNOCK) state = STAND;
		}
		if ((t & TOP) && (diffVelY(e) < 0)) {
			setTop(false);
			vel.y = 0.0f;
			if (state == KNOCK) state = STAND;
		}
		if (t & BOTTOM) {
			if ((diffBottom(e, true) <= margin_y) &&
				(((diffVelY(e) >= 0) && (state == JUMP || (state == LADDER))) ||
				((diffVelY(e) > 0) && (state == KNOCK)))){
				state = STAND;
				setBottom(false);
				vel.y = 0.0f;
			}
		}
		//向こうからぶつかってきたら死亡
		if (!(e->getState() == HOLD_HAMMER) &&
			!((t & LEFT) && (e->getVelX() > 0)) &&
			!((t & RIGHT) && (e->getVelX() < 0)) &&
			!((t & TOP) && (e->getVelY() > 0))) {
			break;
		}
		//死亡時は爆風と同じ挙動 (break無し)
	case BLAST:
		//吹っ飛ばされて死亡
		state = DEAD;
		anim_interval = 1.0f;
		if (blastLeft(e)) vel.x = -VEL_BOMB_X;
		else if (blastRight(e)) vel.x = VEL_BOMB_X;
		if (blastTop(e)) vel.y = -VEL_BOMB_Y;
		else if (blastBottom(e)) vel.y = VEL_BOMB_Y;
		//ちょっと浮く
		vel.y -= VEL_BOMB_M;
		break;
	}
}

//描画する画像を変更
void Enemy::changeImage(){
	//現在の状態を確認
	switch (state){
	case STAND:
		image.setFlipH(direct);
		//動いているか停止しているかで判断
		if (vel.x == 0.0f) setImage(IMG_STAND);
		else setImage(IMG_WALK_S, IMG_WALK_E, true);
		break;
	case JUMP:
		setImage(IMG_STAND);
		break;
	case DEAD:
		if (vel.x > 0.0) setImage(IMG_DEAD_F);
		else setImage(IMG_DEAD_B);
		break;
	}
}

//コンストラクタ
Enemy1::Enemy1(){
	type = EN_1;
}

//移動
void Enemy1::move(float frameTime){
	if (state == STAND){
		if (direct) vel.x = -VEL_WALK;
		else vel.x = VEL_WALK;
	}
	else if (state != DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//コンストラクタ
Enemy2::Enemy2(){
	type = EN_2;
}

//移動
void Enemy2::move(float frameTime){
	if (state == STAND){
		//同じ高さなら突進
		if (ChipCY() == debu->ChipCY()){
			action = true;
			if (getPosX() > debu->getPosX()) {
				vel.x = -VEL_DASH;
				direct = true;
			}
			else {
				vel.x = VEL_DASH;
				direct = false;
			}
		}
		else {
			action = false;
			if (direct) vel.x = -VEL_WALK;
			else vel.x = VEL_WALK;
		}
	}
	else if (state != DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//コンストラクタ
Enemy3::Enemy3(){
	type = EN_3;
}

//移動
void Enemy3::move(float frameTime){
	//動かない
	if (state != DEAD) vel.x = 0;

	//同じ高さなら発砲 向きも合わせる
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (anim_interval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		anim_interval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//描画する画像を変更
void Enemy3::changeImage(){
	Enemy::changeImage();

	//現在の状態を確認
	if (state == STAND) {
		if (anim_interval > 0.8f) setImage(IMG_ACTION);
	}
}


//コンストラクタ
Enemy5::Enemy5(){
	type = EN_5;
}

//移動
void Enemy5::move(float frameTime){
	//動かない
	if (state != DEAD) vel.x = 0;

	//同じ高さなら発砲 向きも合わせる
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (anim_interval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		anim_interval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//描画する画像を変更
void Enemy5::changeImage(){
	Enemy::changeImage();

	//現在の状態を確認
	if (state == STAND) {
		if (anim_interval > 0.8f) setImage(IMG_ACTION);
	}
}

//コンストラクタ
BulletE::BulletE(){
	state = KNOCK;
	size = IMG_SIZE;
	col = IMG_COL;
}

//地形への接触
void BulletE::collideMap(UCHAR t){
	//消滅する
	if (((t & LEFT) && (vel.x < 0.0f)) ||
		((t & RIGHT) && (vel.x > 0.0f))) {
		state = DEAD;
	}
}

//他オブジェクトへの接触
void BulletE::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック(ワープ)
	Entity::collideObj(e, t);

	//とりあえず消滅する
	switch (e->getType()){
	case DEBU:
	case BOX_W:
	case BOX_S:
	case BOX_L:
	case BOX_B:
	case BOX_H:
	case BOX_A:
	case BOX_F:
	case ROCK:
	case BOMB:
	case HIBOMB:
	case HAMMER:
	case BLAST:
		state = DEAD;
		break;
	}
}

//描画する画像を変更
void BulletE::changeImage(){
	//向き変えるだけ
	image.setFlipH(direct);
}

//コンストラクタ
Bullet::Bullet(){
	type = EN_B;
	img = IMG_BULLET;
	edge_x = BULLET_X;
	edge_y = BULLET_Y;
	margin_x = BULLET_MAR_X;
	margin_y = BULLET_MAR_Y;
}

//移動
void Bullet::move(float frameTime){
	if (state == KNOCK){
		if (direct) vel.x = -VEL_BULLET;
		else vel.x = VEL_BULLET;
	}

	Entity::move(frameTime);
}


//コンストラクタ
Missile::Missile(){
	type = EN_M;
	img = IMG_MISSILE;
	edge_x = MISSILE_X;
	edge_y = MISSILE_Y;
	margin_x = MISSILE_MAR_X;
	margin_y = MISSILE_MAR_Y;
}

//移動
void Missile::move(float frameTime){
	if (state == KNOCK){
		if (direct) vel.x = -VEL_MISSILE;
		else vel.x = VEL_MISSILE;
	}

	Entity::move(frameTime);
}
