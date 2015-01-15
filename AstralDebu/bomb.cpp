#include "bomb.h"

using namespace entityNS;
using namespace bombNS;

//�R���X�g���N�^
BombE::BombE(){
	state = STAND;
	size = IMG_SIZE;
	col = IMG_COL;
	edge_x = EDGE_X;
	edge_y = EDGE_Y;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
}

//�ړ�
void BombE::move(float frameTime){
	//�󒆂ɂ���Ȃ痎��
	if (state == JUMP) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	Entity::move(frameTime);
}

//�n�`�ւ̐ڐG
void BombE::collideMap(UCHAR t){
	//�Ƃ肠������������
	if ((t & LEFT) && (vel.x < 0.0f)) {
		setLeft(false);
		vel.x = 0.0f;
		state = DEAD;
	}
	if ((t & RIGHT) && (vel.x > 0.0f)) {
		setRight(false);
		vel.x = 0.0f;
		state = DEAD;
	}
	if ((t & TOP) && (vel.y < 0.0f)) {
		setTop(false);
		vel.y = 0.0f;
		state = DEAD;
	}
	if (t & BOTTOM) {
		//�󒆂ɂ����璅�n����
		if ((vel.y > 0.0f) && ((state == KNOCK) || (state == JUMP))){
			state = DEAD;
		}
	}
	else if ((state != KNOCK) && (state != DEAD)) {
		//���ɉ����Ȃ��Ȃ痎��
		state = JUMP;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG
void BombE::collideObj(Entity *e, UCHAR t){
	//���e�͂Ƃ肠������������
	switch (e->getType()){
	case BOX_W:
	case BOX_S:
	case BOX_L:
	case BOX_B:
	case BOX_H:
	case BOX_A:
	case BOX_F:
	case ROCK:
		//�Î~���Ă���~�܂�
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == JUMP)) setRes(4);
	case BOMB:
	case HIBOMB:
	case HAMMER:
		//�ړ����Ă��甚������
		if (((t & LEFT) && (diffVelX(e) < 0)) ||
			((t & RIGHT) && (diffVelX(e) > 0)) ||
			((t & TOP) && (diffVelY(e) < 0)) ||
			((t & BOTTOM) && (diffVelY(e) > 0) && (state == JUMP || state == KNOCK))) setRes(6);
		break;
	case EN_1:
	case EN_2:
	case EN_3:
	case EN_4:
	case EN_5:
	case EN_B:
		//���E�ɂ͂������������Ă�����A�㉺�͖ⓚ���p�Ŕ�������
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			(t & TOP) || (t & BOTTOM)) setRes(6);
		break;
	case BLAST:
		//�����͑��x�Ƃ��֌W�Ȃ�
		setRes(6);
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

}

//�R���X�g���N�^
Bomb::Bomb(){
	type = BOMB;
	img = IMG_B;
}

//�R���X�g���N�^
Hibomb::Hibomb(){
	type = HIBOMB;
	img = IMG_H;
}