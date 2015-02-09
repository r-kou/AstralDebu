#include "astraldebu.h"
using namespace astralNS;
using namespace entityNS;


//�I�u�W�F�N�g�̒ǉ�
void AstralDebu::addObject(Entity *e, Texture &t, int i, int j){
	map[i][j] = 0;
	object[objMax] = e;
	if (!e->initialize(this, &t, i, j))
		throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
	objMax++;
}

//�G�̒ǉ��@�f�u�̈ʒu������
void AstralDebu::addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j){
	map[i][j] = 0;
	object[objMax] = e;
	if (!e->initialize(this, &t, d, i, j))
		throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
	objMax++;
}

//���[�v�̒ǉ�
void AstralDebu::addWarp(int i, int j){
	object[i]->setPartnerX(object[j]->ChipCX());
	object[i]->setPartnerY(object[j]->ChipCY());
	object[j]->setPartnerX(object[i]->ChipCX());
	object[j]->setPartnerY(object[i]->ChipCY());
}

//�I�u�W�F�N�g�����Ă邩����
bool AstralDebu::canHold(Entity *e){
	switch (e->getType()){
	case entityNS::TY_WOOD_BOX:
	case entityNS::TY_STEEL_BOX:
	case entityNS::TY_LEAD_BOX:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_HIBOMB_BOX:
	case entityNS::TY_AIR_BOX:
	case entityNS::TY_FRAME_BOX:
	case entityNS::TY_BOMB:
	case entityNS::TY_HIBOMB:
	case entityNS::TY_MEAT:
	case entityNS::TY_HIMEAT:
	case entityNS::TY_HAMMER:
		return canTouch(e);
		break;
	default:
		return false;
		break;
	}
}

//�I�u�W�F�N�g����������
bool AstralDebu::canEat(Entity *e){
	switch (e->getType()){
	case entityNS::TY_MEAT:
	case entityNS::TY_HIMEAT:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//�I�u�W�F�N�g���ڐG�\������
bool AstralDebu::canTouch(Entity *e){
	if ((e->getState() == entityNS::ST_LOCK) ||
		(e->getState() == entityNS::ST_CLEAR) ||
		(e->getState() == entityNS::ST_DEAD)) return false;
	return canMove(e);
}

//�I�u�W�F�N�g��`�悷�邩����
bool AstralDebu::canMove(Entity *e){
	if ((e->getState() == entityNS::ST_EMPTY) ||
		(e->getState() == entityNS::ST_LOCK) ||
		(e->getType() == entityNS::TY_NONE)) return false;
	return true;
}

//�̗͂̑���
void AstralDebu::addLife(int i){
	life += i;
	if (life > 100) life = 100;
}

//�̗͂̌���
void AstralDebu::subLife(int i){
	if (cheat1) return;
	life -= i;
	if (life < 0) life = 0;
}

//���g�p�̃I�u�W�F�N�g���擾
int AstralDebu::getEmptyIndex(){
	int tmp = objMax;
	ALL_OBJ if ((object[i]->getType() == entityNS::TY_NONE) && (object[i]->getState() == entityNS::ST_EMPTY)) {
		tmp = i;
		break;
	}
	if (tmp == objMax) objMax++;
	return tmp;
}

//bgm�Đ�
void AstralDebu::playBgm(){
	std::string playBgm;
	if (bgm) return;

	if (stage <= 10) playBgm = audioNS::BGM1;
	else if (stage <= 20) playBgm = audioNS::BGM2;
	else if (stage <= 30) playBgm = audioNS::BGM3;
	else playBgm = audioNS::BGM4;

	if (audio->isPlaying(playBgm.c_str())) return;
	audio->playCue(playBgm.c_str());

	bgm = true;
}

//bgm��~
void AstralDebu::stopBgm(){
	std::string playBgm;
	if (!bgm) return;

	if (stage == 11) playBgm = audioNS::BGM1;
	else if (stage == 21) playBgm = audioNS::BGM2;
	else if (stage == 31) playBgm = audioNS::BGM3;
	else playBgm = audioNS::BGM4;

	if (!audio->isPlaying(playBgm.c_str())) return;
	audio->stopCue(playBgm.c_str());

	bgm = false;
}

//�I�u�W�F�N�g�f�[�^�Ƃ���������
void AstralDebu::resetObject(){
	//�Â��I�u�W�F�N�g�����ׂď���
	ALL_OBJ	SAFE_DELETE(object[i]);
	objMax = 0;
	objHolded = -1;
	warpRed = -1;
	warpGreen = -1;
	warpYellow = -1;
	FOR_2(MAP_COL, MAP_ROW){
		map[i][j] = 0;
	}
}