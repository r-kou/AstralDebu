#include "debu.h"

using namespace entityNS;
using namespace debuNS;

//�R���X�g���N�^
Debu::Debu(){
	state = ST_STAND;
	type = TY_DEBU;
	renderOrder = RO_DEBU;
	size = CHIP_SIZE;
	col = COL_DEBU;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	putSound = audioNS::PUT_DEBU;
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
			case ST_STAND:
				//�؂�Ԃ��͑���
				if (vel.x <= 0) vel.x -= VEL_RATE * frameTime;
				else vel.x -= VEL_DUMP * frameTime;
				setDirect(true);
				break;
			case ST_JUMP:
				//�󒆐���͂��ɂ₩
				if (vel.x > -VEL_MAX_WALK)
					vel.x -= VEL_RATE_JUMP * frameTime;
				break;
			case ST_LADDER:
				//�͂����ł̑��x�͌Œ�
				vel.x = -VEL_LADDER;
				setDirect(true);
				break;
			case ST_HAMMER:
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
			case ST_STAND:
				if (vel.x >= 0) vel.x += VEL_RATE * frameTime;
				else vel.x += VEL_DUMP * frameTime;
				setDirect(false);
				break;
			case ST_JUMP:
				if (vel.x < VEL_MAX_WALK)
					vel.x += VEL_RATE_JUMP * frameTime;
				break;
			case ST_LADDER:
				vel.x = VEL_LADDER;
				setDirect(false);
				break;
			case ST_HAMMER:
				//�ړ��s��
				setDirect(false);
				break;
			}
		}
		else {
			//�L�[���͖���
			switch (state){
			case ST_STAND:
				//�u���[�L��������
				vel.x *= 0.9f;
				if (fabs(vel.x) < 10) vel.x = 0.0f;
				break;
			case ST_LADDER:
				//��q�͂����Ɏ~�܂�
				vel.x = 0.0f;
				break;
			}
		}
	}

	//��q�͏㉺�ړ��\
	if (state == ST_LADDER){
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
	if (state == ST_JUMP || state == ST_KNOCK || state == ST_DEAD) vel.y += GRAVITY_RATE * frameTime;

	//�n��ł͑��x����
	if (state == ST_STAND){
		if (vel.x > VEL_MAX_WALK) vel.x = VEL_MAX_WALK;
		else if (vel.x < -VEL_MAX_WALK) vel.x = -VEL_MAX_WALK;
	}
	if (vel.y > VEL_MAX) vel.y = VEL_MAX;

	//�N���A���͓����Ȃ�
	if (state == ST_CLEAR) {
		vel.x = 0;
		vel.y = 0;
	}

	Entity::move(frameTime);
}

//���I�u�W�F�N�g�ւ̐ڐG
void Debu::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);

	//�f�u�ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
		//���͏�ɏ��� ���ɂ͂��蔲���� �씠�͓�����Ȃ�
		if ((!input->isKeyDown(VK_DOWN))&&
			(t & BOTTOM) && ((diffBottom(e, true) <= 0) &&
			(((diffVelY(e) >= 0) && (state == ST_JUMP || (state == ST_LADDER))) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK))))) setRes(RES_BOTTOM);
		break;
	case TY_LADDER:
		setRes(RES_LADDER);
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
	case TY_BULLET:
		//������΂����
		if (knockInterval == 0.0f){
			setRes(RES_KNOCK, getPosX() > e->getPosX() ? VEL_KNOCK_X : -VEL_KNOCK_X, -VEL_KNOCK_JUMP);
			knockInterval = 0.3f;
		}
		break;
	case TY_MISSILE:
	case TY_BLAST:
		//������΂����
		setRes(RES_JUMP, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y));
		break;
	case TY_GOAL:
		//�N���A����
		setRes(RES_CLEAR);
		break;
	}
}

//�`�悷��摜��ύX
void Debu::changeImage(){
	//���݂̏�Ԃ��m�F
	switch (state){
	case ST_STAND:
		image.setFlipH(direct);
		//�����Ă��邩��~���Ă��邩�Ŕ��f
		if (animInterval > 0.0f) setImage(IMG_THROW);
		else if (vel.x == 0.0f){
			//�I�u�W�F�N�g��ێ����Ă��邩�Ŕ��f
			if (hold) setImage(IMG_HOLD);
			else setImage(IMG_STAND);
		}
		else {
			if (hold) setImage(IMG_HOLD_WALK_START, IMG_HOLD_WALK_END, true);
			else setImage(IMG_WALK_START, IMG_WALK_END, true);
		}
		break;
	case ST_JUMP:
		//���~��
		if (vel.y > 0){
			//�O��ړ��Ŕ��f
			if (vel.x*(direct?-1:1) >= 0) {
				if (hold) setImage(IMG_FALL_HOLD_FRONT);
				else setImage(IMG_FALL_FRONT);
			}
			else {
				if (hold) setImage(IMG_FALL_HOLD_BACK);
				else setImage(IMG_FALL_BACK);
			}
		}
		//�㏸��
		else {
			//�O��ړ��Ŕ��f
			if (vel.x*(direct ? -1 : 1) >= 0)	{
				if (hold) setImage(IMG_JUMP_HOLD_FRONT);
				else setImage(IMG_JUMP_FRONT);
			}
			else {
				if (hold) setImage(IMG_JUMP_HOLD_BACK);
				else setImage(IMG_JUMP_BACK);
			}
		}
		break;
	case ST_LADDER:
		//�����Ă��邩��~���Ă��邩�Ŕ��f �����̋�ʂ͖���
		if (vel.x == 0.0f && vel.y == 0.0f) setImage(IMG_LADDER);
		else setImage(IMG_LADDER_START, IMG_LADDER_END, true);
		break;
	case ST_HAMMER:
		image.setFlipH(directHammer);
		setImage(IMG_HOLD);
		break;
	case ST_CLEAR:
		//�A�j�����I��������N���A
		image.setDelay(0.33f);
		setImage(IMG_CLEAR_START, IMG_CLEAR_END, false);
		break;
	case ST_DEAD:
		//�A�j�����I���������蒼��
		image.setDelay(0.25f);
		setImage(IMG_DEAD_START, IMG_DEAD_END, false);
		break;
	}


}

//���I�u�W�F�N�g�ւ̔���
void Debu::responseObj(){
	Entity::responseObj();
	if (getRes(RES_LADDER)){
		if (input->isKeyDown('Z') || hold || state == ST_KNOCK || state == ST_DEAD || state == ST_CLEAR) return;
		state = ST_LADDER;
	}
}

//�`��
void Debu::draw(){
	//�������u���b�N���A��낪��q�Ƃ��Ȃ甼����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	trans = false;
}


