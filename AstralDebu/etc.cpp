#include "etc.h"

using namespace entityNS;
using namespace etcNS;

//�R���X�g���N�^
Rock::Rock(int stage){
	state = ST_STAND;
	type = TY_ROCK;
	renderOrder = RO_BASE;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_ROCK_X;
	edgeY = EDGE_ROCK_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	deadSound = audioNS::BRAKE_ROCK;
	chipTop = false;
	chipBottom = false;
	//�X�e�[�W�ɉ����ĉ摜�ύX
	if (stage < 10) img = IMG_ROCK_AREA1;
	else if (stage < 20) img = IMG_ROCK_AREA2;
	else if (stage < 30) img = IMG_ROCK_AREA3;
	else img = IMG_ROCK_AREA4;
}


//�n�`�ւ̐ڐG����
void Rock::touchMap(int map[MAP_COL][MAP_ROW]){
	int cx = ChipCX();
	int cy = ChipCY();

	if (cy == 0) chipTop = true;
	else chipTop = TOBOOL(map[cx][cy - 1]);

	if (cy == MAP_ROW) chipBottom = true;
	else chipBottom = TOBOOL(map[cx][cy + 1]);
}

//���I�u�W�F�N�g�ւ̐ڐG
void Rock::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

	//��ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_ROCK:
		//�㉺�̊���m�F
		if (t & TOP) chipTop = true;
		if (t & BOTTOM) chipBottom = true;
		break;
	case TY_BLAST:
		//��������
		setRes(RES_DEAD);
		break;
	case TY_HAMMER:
		//�S������Ԃ����Ă��������
		if ((e->getState() == ST_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(RES_DEAD);
		}
		break;
	}
}


//�`�悷��摜��ύX
void Rock::changeImage(){
	if (chipBottom){
		if (chipTop) setImage(img);
		else setImage(img+1);
	}
	else {
		if (chipTop) setImage(img+2);
		else setImage(img + 3);
	}
}

//�`��
void Rock::draw(){
	//����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}


//�R���X�g���N�^
Ladder::Ladder(){
	state = ST_STAND;
	type = TY_LADDER;
	renderOrder = RO_BASE;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_LADDER_X;
	edgeY = EDGE_LADDER_Y;
	img = IMG_LADDER;
}

//�`��
void Ladder::draw(){
	//����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}

//�R���X�g���N�^
MeatE::MeatE(){
	state = ST_STAND;
	renderOrder = RO_OBJECT;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_MEAT_X;
	edgeY = EDGE_MEAT_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	fall = true;
}

//�n�`�ւ̐ڐG
void MeatE::collideMap(UCHAR t){
	Entity::collideMap(t);
	if ((t & BOTTOM) && (state == ST_STAND)) {
		//�n�ʂł҂�����~�܂�
		setCX();
		vel.x = 0.0f;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG
void MeatE::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

	//���ތŗL�̏Փ˔���
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_GOAST_BOX:
	case TY_HAMMER:
		//���̏�ɏ��
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK)))) {
			//�󒆂ɂ����璅�n����
			setRes(RES_BOTTOM_CHIP);
		}
		break;
	}
}

//�R���X�g���N�^
Meat::Meat(){
	type = TY_MEAT;
	img = IMG_MEAT;
}

//�R���X�g���N�^
Himeat::Himeat(){
	type = TY_HIMEAT;
	img = IMG_HIMEAT;
}

//�R���X�g���N�^
Hammer::Hammer(){
	state = ST_STAND;
	type = TY_HAMMER;
	renderOrder = RO_OBJECT;
	size = CHIP_SIZE;
	col = COL_CHIP;
	img = IMG_HAMMER;
	edgeX = EDGE_HAMMER_X;
	edgeY = EDGE_HAMMER_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	fall = true;
}


//�ړ�
void Hammer::move(float frameTime){
	if (state == ST_HAMMER){
		if (pos.x >= getRight(false)) {
			vel.x = -VEL_HAM;
			renderOrder = RO_HAMMER;
		}
		else if (pos.x <= getLeft(false)) {
			vel.x = VEL_HAM;
			renderOrder = RO_HOLD;
		}

		//���x�������ړ�
		pos.x += (vel.x * frameTime);

		//����̈ʒu�Ƃ��͓���
	}
	else {
		Entity::move(frameTime);
	}
}

//�n�`�ւ̐ڐG����
void Hammer::touchMap(int map[MAP_COL][MAP_ROW]){
	//������ԂȂ瓖����Ȃ�
	if (state == ST_HAMMER) return;

	Entity::touchMap(map);
}

//�n�`�ւ̐ڐG
void Hammer::collideMap(UCHAR t){
	Entity::collideMap(t);

	if ((t & BOTTOM) && (state == ST_STAND)) {
		//�n�ʂł҂�����~�܂�
		setCX();
		vel.x = 0.0f;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG����
void Hammer::touchObj(Entity *e){
	//������ԂȂ瓖����Ȃ�
	if (state == ST_HAMMER) return;

	Entity::touchObj(e);
}

//���I�u�W�F�N�g�ւ̐ڐG
void Hammer::collideObj(Entity *e, UCHAR t){
	//������ԂȂ�}�b�v�ɂ͓�����Ȃ�
	if (state == ST_HAMMER) return;

	//�S���ތŗL�̏Փ˔���@�قƂ�ǂ̃I�u�W�F�N�g���ђʂ���
	switch (e->getType()){
	case TY_LEAD_BOX:
	case TY_HIBOMB_BOX:
	case TY_GOAST_BOX:
		//��~����͉̂����Ɨ씠�����@����ȊO�͑S�ĕ��ӂ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(RES_LEFT);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(RES_RIGHT);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(RES_TOP);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP || state == ST_LADDER)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK)))) setRes(RES_BOTTOM_CHIP); //�󒆂ɂ����璅�n����
		break;
	case TY_RED_WARP:
	case TY_GREEN_WARP:
	case TY_YELLOW_WARP:
		//���[�v�͉\
		if (warpInterval == 0.0f) setRes(RES_WARP, CHIP(e->getPartnerX() + 0.5f), CHIP_D(e->getPartnerY() + 0.5f));
	case TY_LADDER:
	case TY_GOAL:
		//�������ɂȂ�
		trans = true;
		break;
	}
}

//������
void Hammer::setHold(Debu *d){
	d->setState(ST_HAMMER);
	d->setVelX(0);
	d->setVelY(0);

	direct = d->getDirect();
	state = ST_HAMMER;
	if (direct) setPosX(d->getPosX() - HAMMER_MAR_X);
	else setPosX(d->getPosX() + HAMMER_MAR_X);
	setPosY(d->getPosY() - HAMMER_MAR_Y);
	setVelX(VEL_HAM);
	setVelY(0);

	SETRECT(edge, (long)d->getPosX() - EDGE_HAMMER_HOLDED_X, (long)d->getPosY() - EDGE_HAMMER_Y, EDGE_HAMMER_HOLDED_X * 2, EDGE_HAMMER_Y * 2);
}