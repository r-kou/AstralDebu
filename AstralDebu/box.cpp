#include "box.h"

using namespace entityNS;
using namespace boxNS;

//�R���X�g���N�^
Box::Box(){
	state = ST_STAND;
	renderOrder = RO_BOX;
	size = CHIP_SIZE;
	col = COL_CHIP;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}

//�ړ�
void Box::move(float frameTime){
	//�󒆂ɂ���Ȃ痎��
	if (state == ST_JUMP) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_MAX) vel.y = VEL_MAX;

	Entity::move(frameTime);
}

//�n�`�ւ̐ڐG
void Box::collideMap(UCHAR t){
	Entity::collideMap(t);
	if ((t & BOTTOM) && (state == ST_STAND)) {
		//�n�ʂł҂�����~�܂�
		setCX();
		vel.x = 0.0f;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG
void Box::collideObj(Entity *e, UCHAR t){
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
		//�����m�͂Ԃ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(RES_LEFT);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(RES_RIGHT);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(RES_TOP);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK)))) {
			//�󒆂ɂ����璅�n����
			setRes(RES_BOTTOM_CHIP);
		}
		break;
	}
}

//�R���X�g���N�^
WoodBox::WoodBox(){
	type = TY_WOOD_BOX;
	img = IMG_WOOD_BOX;
	putSound = audioNS::PUT_WOOD;
	deadSound = audioNS::BRAKE_WOOD;
}

//���I�u�W�F�N�g�ւ̐ڐG
void WoodBox::collideObj(Entity *e, UCHAR t){
	//�ؔ��ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_BLAST:
		//�����ŉ���
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
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		//�G�ɂԂ���Ɖ���
		if (((t & LEFT) && (vel.x < 0.0f)) ||
			((t & RIGHT) && (vel.x > 0.0f)) ||
			((t & TOP) && (vel.y < 0.0f)) ||
			((t & BOTTOM) && (vel.y > 0.0f))){
			setRes(RES_DEAD);
		}
		break;

	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�R���X�g���N�^
SteelBox::SteelBox(){
	type = TY_STEEL_BOX;
	img = IMG_STEEL_BOX;
	putSound = audioNS::PUT_STEEL;
	deadSound = audioNS::BRAKE_STEEL;
}

//���I�u�W�F�N�g�ւ̐ڐG
void SteelBox::collideObj(Entity *e, UCHAR t){
	//���ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_BLAST:
		//�����Ŕ��ł���
		setRes(RES_JUMP, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y));
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
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		//�G�ɂԂ�������~�܂�
		if (((t & LEFT) && (vel.x < 0.0f)) ||
			((t & RIGHT) && (vel.x > 0.0f)) ||
			((t & TOP) && (vel.y < 0.0f)) ||
			((t & BOTTOM) && (vel.y > 0.0f))){
			setRes(RES_STOP);
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�R���X�g���N�^
LeadBox::LeadBox(){
	type = TY_LEAD_BOX;
	img = IMG_LEAD_BOX;
	putSound = audioNS::PUT_LEAD;
}

//���I�u�W�F�N�g�ւ̐ڐG
void LeadBox::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);

	//�����ŗL�̏Փ˔���͂Ȃ� �ŋ�
}


//�R���X�g���N�^
BombBox::BombBox(){
	type = TY_BOMB_BOX;
	img = IMG_BOMB_BOX;
	putSound = audioNS::PUT_WOOD;
}

//���I�u�W�F�N�g�ւ̐ڐG
void BombBox::collideObj(Entity *e, UCHAR t){
	//���e���ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_BLAST:
		//�����ŉ���@�Ă��U��
		setRes(RES_DEAD);
	case TY_HAMMER:
		//�S������Ԃ����Ă��������
		if ((e->getState() == ST_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(RES_DEAD);
		}
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		//�G�ɂԂ����������
		if (((t & LEFT) && (vel.x < 0.0f)) ||
			((t & RIGHT) && (vel.x > 0.0f)) ||
			((t & TOP) && (vel.y < 0.0f)) ||
			((t & BOTTOM) && (vel.y > 0.0f))){
			setRes(RES_DEAD);
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�R���X�g���N�^
HibombBox::HibombBox(){
	type = TY_HIBOMB_BOX;
	img = IMG_HIBOMB_BOX;
	putSound = audioNS::PUT_WOOD;
}

//���I�u�W�F�N�g�ւ̐ڐG
void HibombBox::collideObj(Entity *e, UCHAR t){
	//�����e���ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_BLAST:
		//�����ŉ��� �S���ł͉��Ȃ�
		setRes(RES_DEAD);
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�R���X�g���N�^
AirBox::AirBox(){
	type = TY_AIR_BOX;
	img = IMG_AIR_BOX;
}


//�ړ�
void AirBox::move(float frameTime){
	//�����Ȃ�
	Entity::move(frameTime);
}

//���I�u�W�F�N�g�ւ̐ڐG
void AirBox::collideObj(Entity *e, UCHAR t){
	//���ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_BLAST:
		//�����Ŕ��ł���
		setRes(RES_KNOCK, blastX(e,VEL_BOMB_X), blastY(e,VEL_BOMB_A));
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
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
		//�G�ɂԂ�������~�܂�
		if (((t & LEFT) && (vel.x < 0.0f)) ||
			((t & RIGHT) && (vel.x > 0.0f)) ||
			((t & TOP) && (vel.y < 0.0f)) ||
			((t & BOTTOM) && (vel.y > 0.0f))){
			setRes(RES_STOP);
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�`��
void AirBox::draw(){
	//��C�Ȃ̂Ŕ�����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw(D3DCOLOR_ARGB(128, 255, 255, 255));
}

//�R���X�g���N�^
FrameBox::FrameBox(){
	type = TY_FRAME_BOX;
	img = IMG_FRAME_BOX;
	putSound = audioNS::PUT_STEEL;
	deadSound = audioNS::BRAKE_STEEL;
}

//�n�`�ւ̐ڐG
void FrameBox::collideMap(UCHAR t){
	//�Ƃ肠��������
	if (((t & LEFT) && (vel.x < 0.0f))||
		((t & RIGHT) && (vel.x > 0.0f))||
		((t & TOP) && (vel.y < 0.0f))){
		vel.x = 0.0f;
		vel.y = 0.0f;
		state = ST_DEAD;
	}
	if (t & BOTTOM) {
		//�󒆂ɂ����璅�n����@����
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
void FrameBox::collideObj(Entity *e, UCHAR t){
	//�t���[���͂Ƃ肠��������
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_ROCK:
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == ST_JUMP)) setRes(RES_BOTTOM_CHIP);
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_HAMMER:
		//�ړ����Ă������
		if (((t & LEFT) && (diffVelX(e) < 0)) ||
			((t & RIGHT) && (diffVelX(e) > 0)) ||
			((t & TOP) && (diffVelY(e) < 0)) ||
			((t & BOTTOM) && (diffVelY(e) > 0) && (state == ST_JUMP || state == ST_KNOCK))) setRes(RES_DEAD);
		break;
	case TY_ENEMY_1:
	case TY_ENEMY_2:
	case TY_ENEMY_3:
	case TY_ENEMY_4:
	case TY_ENEMY_5:
	case TY_BULLET:
		//���E�ɂ͂������������Ă�����A�㉺�͖ⓚ���p�ŉ���
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			(t & TOP) || (t & BOTTOM)) setRes(RES_DEAD);
		break;
	case TY_BLAST:
		//�����͑��x�Ƃ��֌W�Ȃ�
		setRes(RES_DEAD);
		break;
	case TY_DEBU:
		//�f�u������Ă�����
		if (t & TOP) {
			if ((diffTop(e, true) >= -marginY) &&
				((diffVelY(e) < 0) && (e->getState() == ST_JUMP || e->getState() == ST_LADDER || e->getState() == ST_KNOCK))){
				setRes(RES_DEAD);
			}
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);
}

//�R���X�g���N�^
GoastBox::GoastBox(){
	type = TY_GOAST_BOX;
	img = IMG_GOAST_BOX;
}

//�ړ�
void GoastBox::move(float frameTime){
	//�����Ȃ�
	Entity::move(frameTime);
}

//���I�u�W�F�N�g�ւ̐ڐG
void GoastBox::collideObj(Entity *e, UCHAR t){
	//�씠�ŗL�̏Փ˔���
	switch (e->getType()){
	case TY_BLAST:
		//�����ł̂݉���
		setRes(RES_DEAD);
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�`��
void GoastBox::draw(){
	//��Ȃ̂Ŕ�����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw(D3DCOLOR_ARGB(128, 255, 255, 255));
}