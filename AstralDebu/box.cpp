#include "box.h"

using namespace entityNS;
using namespace boxNS;

//�R���X�g���N�^
Box::Box(){
	state = STAND;
	renderOrder = RO_BOX;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}

//�ړ�
void Box::move(float frameTime){
	//�󒆂ɂ���Ȃ痎��
	if (state == JUMP) vel.y += GRAVITY_RATE * frameTime;
	if (vel.y > VEL_FALL_MAX) vel.y = VEL_FALL_MAX;

	Entity::move(frameTime);
}

//�n�`�ւ̐ڐG
void Box::collideMap(UCHAR t){
	Entity::collideMap(t);
	if ((t & BOTTOM) && (state == STAND)) {
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
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
	case FRAME_BOX:
	case GOAST_BOX:
	case HAMMER:
		//�����m�͂Ԃ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(0);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(1);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(2);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == JUMP || state == LADDER)) ||
			((diffVelY(e) > 0) && (state == KNOCK)))) setRes(4); //�󒆂ɂ����璅�n����
		break;
	}
}

//�R���X�g���N�^
WoodBox::WoodBox(){
	type = WOOD_BOX;
	img = IMG_WOOD_BOX;
}

//���I�u�W�F�N�g�ւ̐ڐG
void WoodBox::collideObj(Entity *e, UCHAR t){
	//�ؔ��ŗL�̏Փ˔���
	switch (e->getType()){
	case BLAST:
		//�����ŉ���
		setRes(6);
		break;
	case HAMMER:
		//�S������Ԃ����Ă��������
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�R���X�g���N�^
SteelBox::SteelBox(){
	type = STEEL_BOX;
	img = IMG_STEEL_BOX;
}

//���I�u�W�F�N�g�ւ̐ڐG
void SteelBox::collideObj(Entity *e, UCHAR t){
	//���ŗL�̏Փ˔���
	switch (e->getType()){
	case BLAST:
		//�����Ŕ��ł���
		setRes(7, blastX(e, VEL_BOMB_X), blastY(e, VEL_BOMB_Y, VEL_BOMB_M));
		break;
	case HAMMER:
		//�S������Ԃ����Ă��������
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//���I�u�W�F�N�g�ւ̔���
void SteelBox::responseObj(){
	Entity::responseObj();

	if (getRes(7)){
		state = JUMP;
		vel.x = getResX(7);
		vel.y = getResY(7);
	}
}

//�R���X�g���N�^
LeadBox::LeadBox(){
	type = LEAD_BOX;
	img = IMG_LEAD_BOX;
}

//���I�u�W�F�N�g�ւ̐ڐG
void LeadBox::collideObj(Entity *e, UCHAR t){
	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);

	//�����ŗL�̏Փ˔���͂Ȃ� �ŋ�
}


//�R���X�g���N�^
BombBox::BombBox(){
	type = BOMB_BOX;
	img = IMG_BOMB_BOX;
}

//���I�u�W�F�N�g�ւ̐ڐG
void BombBox::collideObj(Entity *e, UCHAR t){
	//���e���ŗL�̏Փ˔���
	switch (e->getType()){
	case BLAST:
		//�����ŉ���@�Ă��U��
		setRes(6);
	case HAMMER:
		//�S������Ԃ����Ă��������
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�R���X�g���N�^
HibombBox::HibombBox(){
	type = HIBOMB_BOX;
	img = IMG_HIBOMB_BOX;
}

//���I�u�W�F�N�g�ւ̐ڐG
void HibombBox::collideObj(Entity *e, UCHAR t){
	//�����e���ŗL�̏Փ˔���
	switch (e->getType()){
	case BLAST:
		//�����ŉ��� �S���ł͉��Ȃ�
		setRes(6);
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//�R���X�g���N�^
AirBox::AirBox(){
	type = AIR_BOX;
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
	case BLAST:
		//�����Ŕ��ł���
		setRes(7, blastX(e,VEL_BOMB_X), blastY(e,VEL_BOMB_A,0));
		break;
	case HAMMER:
		//�S������Ԃ����Ă��������
		if ((e->getState() == HOLD_HAMMER) ||
			((t & LEFT) && (e->getVelX() > 0)) ||
			((t & RIGHT) && (e->getVelX() < 0)) ||
			((t & TOP) && (e->getVelY() > 0))) {
			setRes(6);
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Box::collideObj(e, t);
}

//���I�u�W�F�N�g�ւ̔���
void AirBox::responseObj(){
	Entity::responseObj();

	if (getRes(7)){
		state = KNOCK;
		vel.x = getResX(7);
		vel.y = getResY(7);
	}
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
	type = FRAME_BOX;
	img = IMG_FRAME_BOX;
}

//�n�`�ւ̐ڐG
void FrameBox::collideMap(UCHAR t){
	//�Ƃ肠��������
	if (((t & LEFT) && (vel.x < 0.0f))||
		((t & RIGHT) && (vel.x > 0.0f))||
		((t & TOP) && (vel.y < 0.0f))){
		vel.x = 0.0f;
		vel.y = 0.0f;
		state = DEAD;
	}
	if (t & BOTTOM) {
		//�󒆂ɂ����璅�n����@����
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
void FrameBox::collideObj(Entity *e, UCHAR t){
	//�t���[���͂Ƃ肠��������
	switch (e->getType()){
	case WOOD_BOX:
	case STEEL_BOX:
	case LEAD_BOX:
	case BOMB_BOX:
	case HIBOMB_BOX:
	case AIR_BOX:
	case FRAME_BOX:
	case ROCK:
		if ((t & BOTTOM) && (diffVelY(e) <= 0) && (state == JUMP)) setRes(4);
	case BOMB:
	case HIBOMB:
	case HAMMER:
		//�ړ����Ă������
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
	case BULLET:
		//���E�ɂ͂������������Ă�����A�㉺�͖ⓚ���p�ŉ���
		if (((t & LEFT) && (vel.x < 0)) ||
			((t & RIGHT) && (vel.x > 0)) ||
			(t & TOP) || (t & BOTTOM)) setRes(6);
		break;
	case BLAST:
		//�����͑��x�Ƃ��֌W�Ȃ�
		setRes(6);
		break;
	case DEBU:
		//�f�u������Ă�����
		if (t & TOP) {
			if ((diffTop(e, true) >= -marginY) &&
				((diffVelY(e) < 0) && (e->getState() == JUMP || e->getState() == LADDER || e->getState() == KNOCK))){
				setRes(6);
			}
		}
		break;
	}

	//�S�I�u�W�F�N�g�̏Փ˔�����`�F�b�N
	Entity::collideObj(e, t);
}

//�R���X�g���N�^
GoastBox::GoastBox(){
	type = GOAST_BOX;
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
	case BLAST:
		//�����ł̂݉���
		setRes(6);
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