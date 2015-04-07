#include "bomb.h"

using namespace entityNS;
using namespace bombNS;

//�R���X�g���N�^
BombE::BombE(){
	state = ST_STAND;
	renderOrder = RO_OBJECT;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	fall = true;
	putSound = audioNS::PUT_STEEL;
}

//�n�`�ւ̐ڐG
void BombE::collideMap(UCHAR t){
	//�Ƃ肠������������
	if ((t & LEFT) && (vel.x < 0.0f)) {
		setLeft(false);
		vel.x = 0.0f;
		state = ST_DEAD;
	}
	if ((t & RIGHT) && (vel.x > 0.0f)) {
		setRight(false);
		vel.x = 0.0f;
		state = ST_DEAD;
	}
	if ((t & TOP) && (vel.y < 0.0f)) {
		setTop(false);
		vel.y = 0.0f;
		state = ST_DEAD;
	}
	if (t & BOTTOM) {
		//�󒆂ɂ����璅�n����
		if ((vel.y > 0.0f) && ((state == ST_KNOCK) || (state == ST_JUMP))){
			state = ST_DEAD;
		}
	}
	else if ((state != ST_KNOCK) && (state != ST_DEAD)) {
		//���ɉ����Ȃ��Ȃ痎��
		state = ST_JUMP;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG
void BombE::collideObj(Entity *e, UCHAR t){
	//���e�͂Ƃ肠������������
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_GOAST_BOX:
	case TY_ROCK:
		//�Î~���Ă���~�܂�
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == ST_JUMP)) setResponse(RES_BOTTOM);
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_HAMMER:
		//�ړ����Ă��甚������
		if (((t & LEFT) && (diffVelX(e) < 0)) ||
			((t & RIGHT) && (diffVelX(e) > 0)) ||
			((t & TOP) && (diffVelY(e) < 0)) ||
			((t & BOTTOM) && (diffVelY(e) > 0) && (state == ST_JUMP || state == ST_KNOCK))) setResponse(RES_DEAD);
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		//�������������Ă����甚������
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			((t & TOP) && (vel.y < 0)) ||
			((t & BOTTOM) && (vel.y > 0))) setResponse(RES_DEAD);
		break;
	case TY_BULLET:
	case TY_MISSILE:
	case TY_BLAST:
		//�����ƒe�ۂ͑��x�Ƃ��֌W�Ȃ�
		setResponse(RES_DEAD);
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

}

//���[�v�̐���𔻒�
void BombE::collideWarp(Entity *e){
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
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		resetResponse(RES_WARP);
		break;
	}
}

//�R���X�g���N�^
Bomb::Bomb(){
	type = TY_BOMB;
	img = IMG_BOMB;
}

//�R���X�g���N�^
Hibomb::Hibomb(){
	type = TY_HIBOMB;
	img = IMG_HIBOMB;
}

//�R���X�g���N�^
Mine::Mine(){
	state = ST_STAND;
	type = TY_MINE;
	renderOrder = RO_OBJECT;
	img = IMG_MINE;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}

//���I�u�W�F�N�g�ւ̐ڐG
void Mine::collideObj(Entity *e, UCHAR t){
	//�@���͖{���ɂȂ�ł���������
	setResponse(RES_DEAD);
}