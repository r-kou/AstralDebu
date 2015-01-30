#include "enemy.h"

using namespace entityNS;
using namespace enemyNS;

//コンストラクタ
Enemy::Enemy(){
	state = STAND;
	renderOrder = RO_ENEMY;
	size = IMG_SIZE;
	col = IMG_COL;
	img = 1;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
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
	if (bottomObj[0] && !bottomObj[1]) {
		setRight(true);
		if (!action) direct = true;
	}
	if (!bottomObj[0] && bottomObj[1]) {
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
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
	case FRAME_BOX:
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
			if ((diffBottom(e, true) <= marginY) &&
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
		animInterval = 1.0f;
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
		else setImage(IMG_WALK_START, IMG_WALK_END, true);
		break;
	case JUMP:
		setImage(IMG_STAND);
		break;
	case DEAD:
		if (vel.x*(direct ? -1 : 1) > 0.0) setImage(IMG_DEAD_FRONT);
		else setImage(IMG_DEAD_BACK);
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
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//描画する画像を変更
void Enemy3::changeImage(){
	Enemy::changeImage();

	//現在の状態を確認
	if (state == STAND) {
		if (animInterval > 0.8f) setImage(IMG_ACTION);
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
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//描画する画像を変更
void Enemy5::changeImage(){
	Enemy::changeImage();

	//現在の状態を確認
	if (state == STAND) {
		if (animInterval > 0.8f) setImage(IMG_ACTION);
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
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
	case FRAME_BOX:
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
	type = BULLET;
	renderOrder = RO_ENEMY;
	img = IMG_BULLET;
	edgeX = BULLET_X;
	edgeY = BULLET_Y;
	marginX = BULLET_MAR_X;
	marginY = BULLET_MAR_Y;
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
	type = MISSILE;
	renderOrder = RO_ENEMY;
	img = IMG_MISSILE;
	edgeX = MISSILE_X;
	edgeY = MISSILE_Y;
	marginX = MISSILE_MAR_X;
	marginY = MISSILE_MAR_Y;
}

//移動
void Missile::move(float frameTime){
	if (state == KNOCK){
		if (direct) vel.x = -VEL_MISSILE;
		else vel.x = VEL_MISSILE;
	}

	Entity::move(frameTime);
}
