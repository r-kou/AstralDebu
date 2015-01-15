#include "debu.h"

using namespace entityNS;
using namespace debuNS;

//�R���X�g���N�^
Debu::Debu(){
	state = STAND;
	type = DEBU;
	size = IMG_SIZE;
	col = IMG_COL;
	edge_x = EDGE_X;
	edge_y = EDGE_Y;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
}

//������
bool Debu::initialize(Game *game, Texture *t, Input *in, int i, int j){
	//���͂��󂯕t����
	input = in;
	hold = false;
	return Entity::initialize(game, t, i, j);
}

//�ړ�
void Debu::move(float frameTime){
	if (input->isKeyDown(VK_LEFT)) {
		if (!input->isKeyDown(VK_RIGHT)){
			//���L�[�݂̂̏��
			switch (state){
			case STAND:
				//�؂�Ԃ��͑���
				if (vel.x <= 0) vel.x -= VEL_RATE * frameTime;
				else vel.x -= VEL_DUMP * frameTime;
				setDirect(true);
				break;
			case JUMP:
				//�󒆐���͂��ɂ₩
				if (vel.x > -VEL_MAX)
					vel.x -= VEL_RATE_JUMP * frameTime;
				break;
			case LADDER:
				//�͂����ł̑��x�͌Œ�
				vel.x = -VEL_LADDER;
				setDirect(true);
				break;
			case HOLD_HAMMER:
				//�ړ��s��
				setDirect(true);
				break;
			}
		}
	}
	else {
		if (input->isKeyDown(VK_RIGHT)){
			//�E�L�[�݂̂̏��
			switch (state){
			case STAND:
				if (vel.x >= 0) vel.x += VEL_RATE * frameTime;
				else vel.x += VEL_DUMP * frameTime;
				setDirect(false);
				break;
			case JUMP:
				if (vel.x < VEL_MAX)
					vel.x += VEL_RATE_JUMP * frameTime;
				break;
			case LADDER:
				vel.x = VEL_LADDER;
				setDirect(false);
				break;
			case HOLD_HAMMER:
				//�ړ��s��
				setDirect(false);
				break;
			}
		}
		else {
			//�L�[���͖���
			switch (state){
			case STAND:
				//�u���[�L��������
				vel.x *= 0.9f;
				if (fabs(vel.x) < 10) vel.x = 0.0f;
				break;
			case LADDER:
				//��q�͂����Ɏ~�܂�
				vel.x = 0.0f;
				break;
			}
		}
	}

	//��q�͏㉺�ړ��\
	if (state == LADDER){
		if (input->isKeyDown(VK_UP)){
			if (!input->isKeyDown(VK_DOWN)) vel.y = -VEL_LADDER;
		}
		else{
			if (input->isKeyDown(VK_DOWN)) vel.y = VEL_LADDER;
			//�����Ƃ����Ɏ~�܂�
			else vel.y = 0.0f;
		}
	}

	//�󒆂ɂ��邩���S���Ȃ痎��
	if (state == JUMP || state == DEAD) vel.y += GRAVITY_RATE * frameTime;

	//�n��ł͑��x����
	if (state == STAND){
		if (vel.x > VEL_MAX) vel.x = VEL_MAX;
		else if (vel.x < -VEL_MAX) vel.x = -VEL_MAX;
	}
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	//�N���A���͓����Ȃ�
	if (state == CLEAR) {
		vel.x = 0;
		vel.y = 0;
	}

	Entity::move(frameTime);
}

//�㉺���E�̏Փ˔���
UCHAR Debu::touchMapDirect(int c, UCHAR t){
	//��q�ɕ߂܂锻�肠��
	if (c == 0) return 0;
	else if (c == CHIP_LADDER) {
		if (t & (TOP | BOTTOM)) return GETLADDER;
		else return 0;
	}
	else return t;
}

//�n�`�ւ̐ڐG
void Debu::collideMap(UCHAR t){
	Entity::collideMap(t);

	if (t & GETLADDER){
		//�͂����ɕ߂܂�
		getLadder();
	}
}

//���I�u�W�F�N�g�ւ̐ڐG
void Debu::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

	//�f�u�ŗL�̏Փ˔���
	switch (e->getType()){
	case BOX_W:
	case BOX_S:
	case BOX_L:
	case BOX_B:
	case BOX_H:
	case BOX_A:
		//���͏�ɏ��� ���ɂ͂��蔲����
		if ((t & BOTTOM) && ((diffBottom(e, true) <= margin_y) &&
			(((diffVelY(e) >= 0) && (state == JUMP || (state == LADDER))) ||
			((diffVelY(e) > 0) && (state == KNOCK))))) setRes(3);
		break;
	case EN_1:
	case EN_2:
	case EN_3:
	case EN_4:
	case EN_5:
	case EN_B:
		//������΂����
		setRes(7, getPosX() > e->getPosX() ? VEL_ENEMY_X : -VEL_ENEMY_X, VEL_ENEMY_Y);
		break;
	case EN_M:
	case BLAST:
		//������΂����
		setRes(8, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y, VEL_BOMB_M));
		break;
	case GOAL:
		//�N���A����
		setRes(9);
		break;
	}
}

//���I�u�W�F�N�g�ւ̔���
void Debu::responseObj(){
	Entity::responseObj();

	if (getRes(7)){
		state = JUMP;
		vel.x = getResX(7);
		vel.y = getResY(7);
	}
	if (getRes(8)){
		state = JUMP;
		vel.x = getResX(8);
		vel.y = getResY(8);
	}
	if (getRes(9)){
		state = CLEAR;
		vel.x = 0.0f;
		vel.y = 0.0f;
	}
}
//�`�悷��摜��ύX
void Debu::changeImage(){
	//���݂̏�Ԃ��m�F
	switch (state){
	case STAND:
		image.setFlipH(direct);
		//�����Ă��邩��~���Ă��邩�Ŕ��f
		if (vel.x == 0.0f){
			//�I�u�W�F�N�g��ێ����Ă��邩�Ŕ��f
			if (hold) setImage(IMG_HOLD);
			else setImage(IMG_STAND);
		}
		else {
			if (hold) setImage(IMG_HOLD_WALK_S, IMG_HOLD_WALK_E, true);
			else setImage(IMG_WALK_S, IMG_WALK_E, true);
		}
		break;
	case JUMP:
		//���~��
		if (vel.y > 0){
			//�O��ړ��Ŕ��f
			if (vel.x >= 0) {
				if (hold) setImage(IMG_FALL_HOLD_F);
				else setImage(IMG_FALL_F);
			}
			else {
				if (hold) setImage(IMG_FALL_HOLD_B);
				else setImage(IMG_FALL_B);
			}
		}
		//�㏸��
		else {
			//�O��ړ��Ŕ��f
			if (vel.x >= 0)	{
				if (hold) setImage(IMG_JUMP_HOLD_F);
				else setImage(IMG_JUMP_F);
			}
			else {
				if (hold) setImage(IMG_JUMP_HOLD_B);
				else setImage(IMG_JUMP_B);
			}
		}
		break;
	case LADDER:
		//�����Ă��邩��~���Ă��邩�Ŕ��f �����̋�ʂ͖���
		if (vel.x == 0.0f && vel.y == 0.0f) setImage(IMG_LADDER);
		else setImage(IMG_LADDER_S, IMG_LADDER_E, true);
		break;
	case HOLD_HAMMER:
		image.setFlipH(direct_h);
		setImage(IMG_HOLD);
		break;
	case CLEAR:
		//�A�j�����I��������N���A
		image.setDelay(0.33f);
		setImage(IMG_CLEAR_S, IMG_CLEAR_E, false);
		break;
	case DEAD:
		//�A�j�����I���������蒼��
		image.setDelay(0.25f);
		setImage(IMG_DEAD_S, IMG_DEAD_E, false);
		break;
	}


}

//�͂����ɕ߂܂�@�f�u��p
void Debu::getLadder(){
	//�W�����v�����莝���Ă�ƕ߂܂�Ȃ�
	if (input->isKeyDown('Z') || hold || state == DEAD || state == CLEAR) return;
	state = LADDER;
}