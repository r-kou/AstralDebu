#include "enemy.h"

using namespace entityNS;
using namespace enemyNS;

//コンストラクタ
Enemy::Enemy(){
	state = ST_STAND;
	renderOrder = RO_ENEMY;
	size = CHIP_SIZE;
	col = COL_ENEMY;
	img = 1;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	fall = true;
	deadSound = audioNS::BRAKE_ENEMY;
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
	if ((t & LEFT) && (vel.x < 0.0f)&&(!action)) direct = false;
	if ((t & RIGHT) && (vel.x > 0.0f)&&(!action)) direct = true;

	Entity::collideMap(t);
}

//他オブジェクトへの接触
void Enemy::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Entity::collideObj(e, t);

	//敵固有の衝突判定
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_HAMMER:
		//箱にはぶつかる
		if ((t & LEFT) && (diffVelX(e) < 0)) setResponse(RES_LEFT);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setResponse(RES_RIGHT);
		if ((t & TOP) && (diffVelY(e) < 0)) setResponse(RES_TOP);
		if ((diffBottom(e, true) <= marginY) &&
			((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK))))) setResponse(RES_BOTTOM); //空中にいたら着地判定
		//向こうからぶつかってきたら死亡
		if ((e->getState() == ST_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			animInterval = 1.0f;
			setResponse(RES_DEAD, getPosX() > e->getPosX() ? VEL_KNOCK_X : -VEL_KNOCK_X, 0);
		}
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		if (((t & LEFT) && (diffVelX(e) < 0))||
			((t & RIGHT) && (diffVelX(e) > 0))) setResponse(RES_COLLIDE);
		break;
	case TY_BLAST:
		//吹っ飛ばされて死亡
		animInterval = 1.0f;
		setResponse(RES_DEAD, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y));
		break;
	}
}

//描画する画像を変更
void Enemy::changeImage(){
	//現在の状態を確認
	switch (state){
	case ST_STAND:
		image.setFlipH(direct);
		//動いているか停止しているかで判断
		if (vel.x == 0.0f) {
			if (animInterval > 0.0f) setImage(IMG_ACTION + margin);
			else setImage(IMG_STAND + margin);
		}
		else setImage(IMG_WALK_START+margin, IMG_WALK_END+margin, true);
		break;
	case ST_JUMP:
		setImage(IMG_STAND+margin);
		break;
	case ST_DEAD:
		if (vel.x*(direct ? -1 : 1) > 0.0) setImage(IMG_DEAD_FRONT+margin-(walk?0:IMG_MAR_WALK));
		else setImage(IMG_DEAD_BACK + margin - (walk ? 0 : IMG_MAR_WALK));
		break;
	}
}

//描画
void Enemy::draw(){
	//書く
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}

//ワープの是非を判定
void Enemy::collideWarp(Entity *e){
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
		resetResponse(RES_WARP);
		break;
	}
}

//コンストラクタ
Enemy1::Enemy1(){
	type = TY_ENEMY_1;
	margin = IMG_MAR_1;
	walk = true;
}

//移動
void Enemy1::move(float frameTime){
	if (state == ST_STAND){
		if (direct) vel.x = -VEL_WALK;
		else vel.x = VEL_WALK;
	}
	else if (state != ST_DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//コンストラクタ
Enemy2::Enemy2(){
	type = TY_ENEMY_2;
	margin = IMG_MAR_2;
	walk = true;
}

//移動
void Enemy2::move(float frameTime){
	if (state == ST_STAND){
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
	else if (state != ST_DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//コンストラクタ
Enemy3::Enemy3(){
	type = TY_ENEMY_3;
	margin = IMG_MAR_3;
	walk = false;
}

//移動
void Enemy3::move(float frameTime){
	//動かない
	if (state != ST_DEAD) vel.x = 0;

	//同じ高さなら発砲 向きも合わせる
	if ((state == ST_STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 0.75f;
		action = true;
	}

	Enemy::move(frameTime);
}

//コンストラクタ
Enemy4::Enemy4(){
	type = TY_ENEMY_4;
	margin = IMG_MAR_4;
	walk = true;
}

//移動
void Enemy4::move(float frameTime){
	if ((state == ST_STAND)&&(animInterval == 0.0f)){
		if (direct) vel.x = -VEL_WALK;
		else vel.x = VEL_WALK;
	}
	else if (state != ST_DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//他オブジェクトへの接触
void Enemy4::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック
	Enemy::collideObj(e, t);

	//敵固有の衝突判定
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_BOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_BOMB:
	case TY_HIBOMB:
		//押せる箱にぶつかると押す
		if ((state != ST_DEAD) && (!action) && (animInterval == 0)&&
			(((t & LEFT) && (diffVelX(e) < 0))||
			((t & RIGHT) && (diffVelX(e) > 0)))) {
			action = true;
		}
		break;
	}
}
//描画する画像を変更
void Enemy4::changeImage(){
	Enemy::changeImage();

	//現在の状態を確認
	if (state == ST_STAND) {
		if (animInterval > 0.3f) setImage(IMG_ACTION);
	}
}

//コンストラクタ
Enemy5::Enemy5(){
	type = TY_ENEMY_5;
	margin = IMG_MAR_5;
	walk = false;
}

//移動
void Enemy5::move(float frameTime){
	//動かない
	if (state != ST_DEAD) vel.x = 0;

	//同じ高さなら発砲 向きも合わせる
	if ((state == ST_STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 1.5f;
		action = true;
	}

	Enemy::move(frameTime);
}

//他オブジェクトへの反応
void Enemy5::responseObj(){
	if (getResponse(RES_DEAD)){
		action = true;
	}

	Entity::responseObj();
}

//コンストラクタ
BulletE::BulletE(){
	state = ST_KNOCK;
	size = CHIP_SIZE;
	col = COL_ENEMY;
}

//地形への接触
void BulletE::collideMap(UCHAR t){
	//消滅する
	if (((t & LEFT) && (vel.x < 0.0f)) ||
		((t & RIGHT) && (vel.x > 0.0f))) {
		state = ST_DEAD;
	}
}

//他オブジェクトへの接触
void BulletE::collideObj(Entity *e, UCHAR t){
	//全オブジェクトの衝突判定をチェック(ワープ)
	Entity::collideObj(e, t);

	//とりあえず消滅する
	switch (e->getType()){
	case TY_DEBU:
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_ROCK:
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_HAMMER:
	case TY_BLAST:
		setResponse(RES_DEAD);
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
	type = TY_BULLET;
	renderOrder = RO_ENEMY;
	img = IMG_BULLET;
	edgeX = BULLET_X;
	edgeY = BULLET_Y;
	marginX = BULLET_MAR_X;
	marginY = BULLET_MAR_Y;
}

//移動
void Bullet::move(float frameTime){
	if (state == ST_KNOCK){
		if (direct) vel.x = -VEL_BULLET;
		else vel.x = VEL_BULLET;
	}

	Entity::move(frameTime);
}


//コンストラクタ
Missile::Missile(){
	type = TY_MISSILE;
	renderOrder = RO_ENEMY;
	img = IMG_MISSILE;
	edgeX = MISSILE_X;
	edgeY = MISSILE_Y;
	marginX = MISSILE_MAR_X;
	marginY = MISSILE_MAR_Y;
}

//移動
void Missile::move(float frameTime){
	if (state == ST_KNOCK){
		if (direct) vel.x = -VEL_MISSILE;
		else vel.x = VEL_MISSILE;
	}

	Entity::move(frameTime);
}
