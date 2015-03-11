#include "enemy.h"

using namespace entityNS;
using namespace enemyNS;

//�R���X�g���N�^
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

//������
bool Enemy::initialize(Game *game, Texture *t, Debu *d, int i, int j){
	//�f�u�̏����g��
	debu = d;
	return Entity::initialize(game, t, i, j);
}

//�ړ�
void Enemy::move(float frameTime){
	//���s�ł͏����痎���Ȃ�
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

//�n�`�ւ̐ڐG
void Enemy::collideMap(UCHAR t){
	//������ς���
	if ((t & LEFT) && (vel.x < 0.0f)&&(!action)) direct = false;
	if ((t & RIGHT) && (vel.x > 0.0f)&&(!action)) direct = true;

	Entity::collideMap(t);
}

//���I�u�W�F�N�g�ւ̐ڐG
void Enemy::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

	//�G�ŗL�̏Փ˔���
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
		//���ɂ͂Ԃ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(RES_LEFT);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(RES_RIGHT);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(RES_TOP);
		if ((diffBottom(e, true) <= marginY) &&
			((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK))))) setRes(RES_BOTTOM); //�󒆂ɂ����璅�n����
		//����������Ԃ����Ă����玀�S
		if ((e->getState() == ST_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			animInterval = 1.0f;
			setRes(RES_DEAD, getPosX() > e->getPosX() ? VEL_KNOCK_X : -VEL_KNOCK_X, 0);
		}
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		if (((t & LEFT) && (diffVelX(e) < 0))||
			((t & RIGHT) && (diffVelX(e) > 0))) setRes(RES_COLLIDE);
		break;
	case TY_BLAST:
		//������΂���Ď��S
		animInterval = 1.0f;
		setRes(RES_DEAD, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y));
		break;
	}
}

//�`�悷��摜��ύX
void Enemy::changeImage(){
	//���݂̏�Ԃ��m�F
	switch (state){
	case ST_STAND:
		image.setFlipH(direct);
		//�����Ă��邩��~���Ă��邩�Ŕ��f
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

//�`��
void Enemy::draw(){
	//����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}

//�R���X�g���N�^
Enemy1::Enemy1(){
	type = TY_ENEMY_1;
	margin = IMG_MAR_1;
	walk = true;
}

//�ړ�
void Enemy1::move(float frameTime){
	if (state == ST_STAND){
		if (direct) vel.x = -VEL_WALK;
		else vel.x = VEL_WALK;
	}
	else if (state != ST_DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//�R���X�g���N�^
Enemy2::Enemy2(){
	type = TY_ENEMY_2;
	margin = IMG_MAR_2;
	walk = true;
}

//�ړ�
void Enemy2::move(float frameTime){
	if (state == ST_STAND){
		//���������Ȃ�ːi
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

//�R���X�g���N�^
Enemy3::Enemy3(){
	type = TY_ENEMY_3;
	margin = IMG_MAR_3;
	walk = false;
}

//�ړ�
void Enemy3::move(float frameTime){
	//�����Ȃ�
	if (state != ST_DEAD) vel.x = 0;

	//���������Ȃ甭�C ���������킹��
	if ((state == ST_STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 0.5f;
		action = true;
	}

	Enemy::move(frameTime);
}

//�`�悷��摜��ύX
void Enemy3::changeImage(){
	Enemy::changeImage();

	//���݂̏�Ԃ��m�F
	if (state == ST_STAND) {
		if (animInterval > 0.8f) setImage(IMG_ACTION);
	}
}
//�R���X�g���N�^
Enemy4::Enemy4(){
	type = TY_ENEMY_4;
	margin = IMG_MAR_4;
	walk = true;
}

//�ړ�
void Enemy4::move(float frameTime){
	if ((state == ST_STAND)&&(animInterval == 0.0f)){
		if (direct) vel.x = -VEL_WALK;
		else vel.x = VEL_WALK;
	}
	else if (state != ST_DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//���I�u�W�F�N�g�ւ̐ڐG
void Enemy4::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Enemy::collideObj(e, t);

	//�G�ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_BOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_BOMB:
	case TY_HIBOMB:
		//�����锠�ɂԂ���Ɖ���
		if ((state != ST_DEAD) && (!action) && (animInterval == 0)&&
			(((t & LEFT) && (diffVelX(e) < 0))||
			((t & RIGHT) && (diffVelX(e) > 0)))) {
			action = true;
		}
		break;
	}
}
//�`�悷��摜��ύX
void Enemy4::changeImage(){
	Enemy::changeImage();

	//���݂̏�Ԃ��m�F
	if (state == ST_STAND) {
		if (animInterval > 0.3f) setImage(IMG_ACTION);
	}
}

//�R���X�g���N�^
Enemy5::Enemy5(){
	type = TY_ENEMY_5;
	margin = IMG_MAR_5;
	walk = false;
}

//�ړ�
void Enemy5::move(float frameTime){
	//�����Ȃ�
	if (state != ST_DEAD) vel.x = 0;

	//���������Ȃ甭�C ���������킹��
	if ((state == ST_STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 0.75f;
		action = true;
	}

	Enemy::move(frameTime);
}

//�`�悷��摜��ύX
void Enemy5::changeImage(){
	Enemy::changeImage();

	//���݂̏�Ԃ��m�F
	if (state == ST_STAND) {
		if (animInterval > 0.8f) setImage(IMG_ACTION);
	}
}

//�R���X�g���N�^
BulletE::BulletE(){
	state = ST_KNOCK;
	size = CHIP_SIZE;
	col = COL_ENEMY;
}

//�n�`�ւ̐ڐG
void BulletE::collideMap(UCHAR t){
	//���ł���
	if (((t & LEFT) && (vel.x < 0.0f)) ||
		((t & RIGHT) && (vel.x > 0.0f))) {
		state = ST_DEAD;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG
void BulletE::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N(���[�v)
	Entity::collideObj(e, t);

	//�Ƃ肠�������ł���
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
		setRes(RES_DEAD);
		break;
	}
}

//�`�悷��摜��ύX
void BulletE::changeImage(){
	//�����ς��邾��
	image.setFlipH(direct);
}

//�R���X�g���N�^
Bullet::Bullet(){
	type = TY_BULLET;
	renderOrder = RO_ENEMY;
	img = IMG_BULLET;
	edgeX = BULLET_X;
	edgeY = BULLET_Y;
	marginX = BULLET_MAR_X;
	marginY = BULLET_MAR_Y;
}

//�ړ�
void Bullet::move(float frameTime){
	if (state == ST_KNOCK){
		if (direct) vel.x = -VEL_BULLET;
		else vel.x = VEL_BULLET;
	}

	Entity::move(frameTime);
}


//�R���X�g���N�^
Missile::Missile(){
	type = TY_MISSILE;
	renderOrder = RO_ENEMY;
	img = IMG_MISSILE;
	edgeX = MISSILE_X;
	edgeY = MISSILE_Y;
	marginX = MISSILE_MAR_X;
	marginY = MISSILE_MAR_Y;
}

//�ړ�
void Missile::move(float frameTime){
	if (state == ST_KNOCK){
		if (direct) vel.x = -VEL_MISSILE;
		else vel.x = VEL_MISSILE;
	}

	Entity::move(frameTime);
}
