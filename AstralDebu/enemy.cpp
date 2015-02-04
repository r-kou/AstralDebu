#include "enemy.h"

using namespace entityNS;
using namespace enemyNS;

//�R���X�g���N�^
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

//������
bool Enemy::initialize(Game *game, Texture *t, Debu *d, int i, int j){
	//�f�u�̏����g��
	debu = d;
	return Entity::initialize(game, t, i, j);
}

//�ړ�
void Enemy::move(float frameTime){
	//�󒆂ɂ��邩���S���Ȃ痎��
	if (state == JUMP || state == DEAD) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_MAX) vel.y = VEL_MAX;

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
	if ((t & LEFT) && (vel.x <= 0.0f)&&(!action)) direct = false;
	if ((t & RIGHT) && (vel.x >= 0.0f)&&(!action)) direct = true;

	Entity::collideMap(t);
}

//���I�u�W�F�N�g�ւ̐ڐG
void Enemy::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

	//�G�ŗL�̏Փ˔���
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
		//���ɂ͂Ԃ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(RES_LEFT);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(RES_RIGHT);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(RES_TOP);
		if ((diffBottom(e, true) <= marginY) &&
			((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == JUMP)) ||
			((diffVelY(e) > 0) && (state == KNOCK))))) setRes(RES_BOTTOM); //�󒆂ɂ����璅�n����
		//����������Ԃ����Ă����玀�S
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			animInterval = 1.0f;
			setRes(RES_DEAD, getPosX() > e->getPosX() ? VEL_KNOCK_X : -VEL_KNOCK_X, 0);
		}
		break;
	case BLAST:
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
	case STAND:
		image.setFlipH(direct);
		//�����Ă��邩��~���Ă��邩�Ŕ��f
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

//�R���X�g���N�^
Enemy1::Enemy1(){
	type = EN_1;
}

//�ړ�
void Enemy1::move(float frameTime){
	if (state == STAND){
		if (direct) vel.x = -VEL_WALK;
		else vel.x = VEL_WALK;
	}
	else if (state != DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//�R���X�g���N�^
Enemy2::Enemy2(){
	type = EN_2;
}

//�ړ�
void Enemy2::move(float frameTime){
	if (state == STAND){
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
	else if (state != DEAD) vel.x = 0;

	Enemy::move(frameTime);
}

//�R���X�g���N�^
Enemy3::Enemy3(){
	type = EN_3;
}

//�ړ�
void Enemy3::move(float frameTime){
	//�����Ȃ�
	if (state != DEAD) vel.x = 0;

	//���������Ȃ甭�C ���������킹��
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//�`�悷��摜��ύX
void Enemy3::changeImage(){
	Enemy::changeImage();

	//���݂̏�Ԃ��m�F
	if (state == STAND) {
		if (animInterval > 0.8f) setImage(IMG_ACTION);
	}
}


//�R���X�g���N�^
Enemy5::Enemy5(){
	type = EN_5;
}

//�ړ�
void Enemy5::move(float frameTime){
	//�����Ȃ�
	if (state != DEAD) vel.x = 0;

	//���������Ȃ甭�C ���������킹��
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (animInterval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		animInterval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//�`�悷��摜��ύX
void Enemy5::changeImage(){
	Enemy::changeImage();

	//���݂̏�Ԃ��m�F
	if (state == STAND) {
		if (animInterval > 0.8f) setImage(IMG_ACTION);
	}
}

//�R���X�g���N�^
BulletE::BulletE(){
	state = KNOCK;
	size = IMG_SIZE;
	col = IMG_COL;
}

//�n�`�ւ̐ڐG
void BulletE::collideMap(UCHAR t){
	//���ł���
	if (((t & LEFT) && (vel.x < 0.0f)) ||
		((t & RIGHT) && (vel.x > 0.0f))) {
		state = DEAD;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG
void BulletE::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N(���[�v)
	Entity::collideObj(e, t);

	//�Ƃ肠�������ł���
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
	type = BULLET;
	renderOrder = RO_ENEMY;
	img = IMG_BULLET;
	edgeX = BULLET_X;
	edgeY = BULLET_Y;
	marginX = BULLET_MAR_X;
	marginY = BULLET_MAR_Y;
}

//�ړ�
void Bullet::move(float frameTime){
	if (state == KNOCK){
		if (direct) vel.x = -VEL_BULLET;
		else vel.x = VEL_BULLET;
	}

	Entity::move(frameTime);
}


//�R���X�g���N�^
Missile::Missile(){
	type = MISSILE;
	renderOrder = RO_ENEMY;
	img = IMG_MISSILE;
	edgeX = MISSILE_X;
	edgeY = MISSILE_Y;
	marginX = MISSILE_MAR_X;
	marginY = MISSILE_MAR_Y;
}

//�ړ�
void Missile::move(float frameTime){
	if (state == KNOCK){
		if (direct) vel.x = -VEL_MISSILE;
		else vel.x = VEL_MISSILE;
	}

	Entity::move(frameTime);
}
