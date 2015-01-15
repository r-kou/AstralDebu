#include "enemy.h"

using namespace entityNS;
using namespace enemyNS;

//�R���X�g���N�^
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
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	//���s�ł͏����痎���Ȃ�
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
		//���ɂ͂Ԃ���
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
		//����������Ԃ����Ă����玀�S
		if (!(e->getState() == HOLD_HAMMER) &&
			!((t & LEFT) && (e->getVelX() > 0)) &&
			!((t & RIGHT) && (e->getVelX() < 0)) &&
			!((t & TOP) && (e->getVelY() > 0))) {
			break;
		}
		//���S���͔����Ɠ������� (break����)
	case BLAST:
		//������΂���Ď��S
		state = DEAD;
		anim_interval = 1.0f;
		if (blastLeft(e)) vel.x = -VEL_BOMB_X;
		else if (blastRight(e)) vel.x = VEL_BOMB_X;
		if (blastTop(e)) vel.y = -VEL_BOMB_Y;
		else if (blastBottom(e)) vel.y = VEL_BOMB_Y;
		//������ƕ���
		vel.y -= VEL_BOMB_M;
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
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (anim_interval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		anim_interval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//�`�悷��摜��ύX
void Enemy3::changeImage(){
	Enemy::changeImage();

	//���݂̏�Ԃ��m�F
	if (state == STAND) {
		if (anim_interval > 0.8f) setImage(IMG_ACTION);
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
	if ((state == STAND) && (!action) && (ChipCY() == debu->ChipCY()) && (anim_interval == 0)){
		if (getPosX() > debu->getPosX()) direct = true;
		else direct = false;
		anim_interval = 1.0f;
		action = true;
	}

	Enemy::move(frameTime);
}

//�`�悷��摜��ύX
void Enemy5::changeImage(){
	Enemy::changeImage();

	//���݂̏�Ԃ��m�F
	if (state == STAND) {
		if (anim_interval > 0.8f) setImage(IMG_ACTION);
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

//�`�悷��摜��ύX
void BulletE::changeImage(){
	//�����ς��邾��
	image.setFlipH(direct);
}

//�R���X�g���N�^
Bullet::Bullet(){
	type = EN_B;
	img = IMG_BULLET;
	edge_x = BULLET_X;
	edge_y = BULLET_Y;
	margin_x = BULLET_MAR_X;
	margin_y = BULLET_MAR_Y;
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
	type = EN_M;
	img = IMG_MISSILE;
	edge_x = MISSILE_X;
	edge_y = MISSILE_Y;
	margin_x = MISSILE_MAR_X;
	margin_y = MISSILE_MAR_Y;
}

//�ړ�
void Missile::move(float frameTime){
	if (state == KNOCK){
		if (direct) vel.x = -VEL_MISSILE;
		else vel.x = VEL_MISSILE;
	}

	Entity::move(frameTime);
}
