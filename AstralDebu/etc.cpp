#include "etc.h"

using namespace entityNS;
using namespace etcNS;

//�R���X�g���N�^
Rock::Rock(int stage){
	state = STAND;
	type = ROCK;
	size = IMG_SIZE;
	col = IMG_COL;
	edge_x = EDGE_RX;
	edge_y = EDGE_RY;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
	//�X�e�[�W�ɉ����ĉ摜�ύX
	if (stage < 10) img = IMG_R1;
	else if (stage < 20) img = IMG_R2;
	else img = IMG_R3;
}

//���I�u�W�F�N�g�ւ̐ڐG
void Rock::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

	//��ŗL�̏Փ˔���
	switch (e->getType()){
	case BLAST:
	case HAMMER:
		//��������
		setRes(6);
		break;
	}
}

//�R���X�g���N�^
Meat::Meat(){
	state = STAND;
	type = MEAT;
	size = IMG_SIZE;
	col = IMG_COL;
	img = IMG_M;
}

//�R���X�g���N�^
Himeat::Himeat(){
	state = STAND;
	type = HIMEAT;
	size = IMG_SIZE;
	col = IMG_COL;
	img = IMG_H;
}


//�R���X�g���N�^
Hammer::Hammer(){
	state = STAND;
	type = HAMMER;
	size = IMG_SIZE;
	col = IMG_COL;
	img = IMG_HAM;
	edge_x = EDGE_HX;
	edge_y = EDGE_HY;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
}


//�ړ�
void Hammer::move(float frameTime){
	if (state == HOLD_HAMMER){
		if (pos.x >= getRight(false)) vel.x = -VEL_HAM;
		else if (pos.x <= getLeft(false)) vel.x = VEL_HAM;

		//���x�������ړ�
		pos.x += (vel.x * frameTime);

		//����̈ʒu�Ƃ��͓���
	}
	else {
		//�󒆂ɂ���Ȃ痎��
		if (state == JUMP) vel.y += GRAVITY_RATE * frameTime;
		if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

		Entity::move(frameTime);
	}
}


//�n�`�ւ̐ڐG����
void Hammer::touchMap(int map[MAP_COL][MAP_ROW]){
	//������ԂȂ瓖����Ȃ�
	if (state == HOLD_HAMMER) return;

	Entity::touchMap(map);
}

//�n�`�ւ̐ڐG
void Hammer::collideMap(UCHAR t){
	Entity::collideMap(t);

	if ((t & BOTTOM) && (state == STAND)) {
		//�n�ʂł҂�����~�܂�
		setCX();
		vel.x = 0.0f;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG����
void Hammer::touchObj(Entity *e){
	//������ԂȂ瓖����Ȃ�
	if (state == HOLD_HAMMER) return;

	Entity::touchObj(e);
}

//���I�u�W�F�N�g�ւ̐ڐG
void Hammer::collideObj(Entity *e, UCHAR t){
	//������ԂȂ�}�b�v�ɂ͓�����Ȃ�
	if (state == HOLD_HAMMER) return;

	//�S���ތŗL�̏Փ˔���@�قƂ�ǂ̃I�u�W�F�N�g���ђʂ���
	switch (e->getType()){
	case BOX_L:
	case BOX_H:
		//��~����͉̂��������@����ȊO�͑S�ĕ��ӂ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(0);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(1);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(2);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == JUMP || state == LADDER)) ||
			((diffVelY(e) > 0) && (state == KNOCK)))) setRes(4); //�󒆂ɂ����璅�n����
		break;
	case WARP_R:
	case WARP_G:
	case WARP_Y:
		//���[�v�͉\
		if (warp_interval == 0.0f) setRes(5, (e->getPartnerX() + 0.5f) * CHIP_SIZE, (e->getPartnerY() + 0.5f) * CHIP_SIZE + DATA_LEN);
		break;
	}
}

//������
void Hammer::setHold(Debu *d){
	d->setState(HOLD_HAMMER);
	d->setVelX(0);
	d->setVelY(0);

	direct = d->getDirect();
	state = HOLD_HAMMER;
	if (direct) setPosX(d->getPosX() - HAMMER_MAR_X);
	else setPosX(d->getPosX() + HAMMER_MAR_X);
	setPosY(d->getPosY() - HAMMER_MAR_Y);
	setVelX(VEL_HAM);
	setVelY(0);

	SETRECT(edge, (long)d->getPosX() - EDGE_HS, (long)d->getPosY() - EDGE_HY, EDGE_HS * 2, EDGE_HY * 2);
}