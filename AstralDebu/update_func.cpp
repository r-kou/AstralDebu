#include "astraldebu.h"
using namespace astralNS;
using namespace entityNS;


//�I�u�W�F�N�g�̒ǉ�
void AstralDebu::addObject(Entity *e, Texture &t, int i, int j){
	if (objMax >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING,"�I�u�W�F�N�g�̐�������ɒB���܂���"));
	map[i][j] = 0;
	setObject(objMax, e);
	if (!e->initialize(this, &t, i, j))
		throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
	objMax++;
}

//�G�̒ǉ��@�f�u�̈ʒu������
void AstralDebu::addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j){
	if (objMax >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "�I�u�W�F�N�g�̐�������ɒB���܂���"));
	map[i][j] = 0;
	setObject(objMax, e);
	if (!e->initialize(this, &t, d, i, j))
		throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
	objMax++;
}

//���[�v�̒ǉ�
void AstralDebu::addWarp(int i, int j){
	Entity *ei = getObject(i),*ej = getObject(j);
	ei->setpartner(ej);
	ej->setpartner(ei);
}

//�I�u�W�F�N�g���w�i�^�C�v������
bool AstralDebu::isBase(Entity *e){
	switch (e->getType()){
	case TY_GOAL:
	case TY_RED_WARP:
	case TY_GREEN_WARP:
	case TY_YELLOW_WARP:
	case TY_LADDER:
	case TY_BULLET:
	case TY_MISSILE:
		return true;
	}
	return false;
}

//�I�u�W�F�N�g�����Ă邩����
bool AstralDebu::isHoldable(Entity *e){
	//��C���͋󒆂ł����߂�
	if (e->getType() == TY_AIR_BOX) return isTouchable(e);
	if (e->getState() != ST_STAND) return false;
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_FRAME_BOX:
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_MEAT:
	case TY_HIMEAT:
	case TY_HAMMER:
		return isTouchable(e);
		break;
	default:
		return false;
		break;
	}
}

//�I�u�W�F�N�g����������
bool AstralDebu::isFood(Entity *e){
	switch (e->getType()){
	case TY_MEAT:
	case TY_HIMEAT:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//�I�u�W�F�N�g���ڐG�\������
bool AstralDebu::isTouchable(Entity *e){
	if ((e->getState() == ST_LOCK) ||
		(e->getState() == ST_CLEAR) ||
		(e->getState() == ST_DEAD)) return false;
	return isMovable(e);
}

//�I�u�W�F�N�g��`�悷�邩����
bool AstralDebu::isMovable(Entity *e){
	if ((e->getState() == ST_EMPTY) ||
		(e->getState() == ST_LOCK) ||
		(e->getType() == TY_NONE)) return false;
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
	int empty = objMax;
	if (objMax > OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "�I�u�W�F�N�g�̐�������ɒB���܂���"));
	ALL_OBJ {
		Entity *tmp = getObject(i);
		if ((tmp->getType() == TY_NONE) && (tmp->getState() == ST_EMPTY)) {
			empty = i;
			break;
		}
	}
	if (empty == objMax) objMax++;
	return empty;
}

//bgm�Đ�
void AstralDebu::playBgm(){
	std::string playBgm;
	if (bgm) return;

	if (stage == 0) playBgm = audioNS::BGM_TITLE;
	else if (stage <= 10) playBgm = audioNS::BGM1;
	else if (stage <= 20) playBgm = audioNS::BGM2;
	else if (stage <= 30) playBgm = audioNS::BGM3;
	else playBgm = audioNS::BGM4;

	if (audio->isPlaying(playBgm.c_str())) return;
	audio->playCue(playBgm.c_str());

	bgm = true;
}

//bgm��~
void AstralDebu::stopBgm(){
	if (!bgm) return;

	if (audio->isPlaying(audioNS::BGM_TITLE)) audio->stopCue(audioNS::BGM_TITLE);
	if (audio->isPlaying(audioNS::BGM1)) audio->stopCue(audioNS::BGM1);
	if (audio->isPlaying(audioNS::BGM2)) audio->stopCue(audioNS::BGM2);
	if (audio->isPlaying(audioNS::BGM3)) audio->stopCue(audioNS::BGM3);
	if (audio->isPlaying(audioNS::BGM4)) audio->stopCue(audioNS::BGM4);

	bgm = false;
}

//bgm�ύX
void AstralDebu::changeBgm(int m){
	switch (m){
	case 0:
		if (audio->isPlaying(audioNS::BGM_TITLE)) break;
		stopBgm();
		playBgm();
		break;
	case 1:
		if (audio->isPlaying(audioNS::BGM1)) break;
		stopBgm();
		playBgm();
		break;
	case 2:
		if (audio->isPlaying(audioNS::BGM2)) break;
		stopBgm();
		playBgm();
		break;
	case 3:
		if (audio->isPlaying(audioNS::BGM3)) break;
		stopBgm();
		playBgm();
		break;
	case 4:
		if (audio->isPlaying(audioNS::BGM4)) break;
		stopBgm();
		playBgm();
		break;
	}
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

//double�𐮐��ɂ��ĕ�����
std::string AstralDebu::doubleToString(double f){
	int i = (int)(f * 100);
	return std::to_string(i);
}

//�}�b�v�`�b�v�̃f�R�[�h
short AstralDebu::decodeChip(short c, int i, int j) {
	short first, second, third;
	first = ((i*(j + 1)) << 4);
	second = ((i + 1) * 11);
	third = c - first;
	if ((third % second)!=0) throw(GameError(gameErrorNS::FATAL, "�}�b�v�f�[�^�̓ǂݍ��݂Ɏ��s���܂���\n�}�b�v�f�[�^�����Ă���\��������܂�"));
	return third / second;
}


//�J�[�\����X�ʒu��Ԃ�
int AstralDebu::getCursorChipX(Entity *e){
	if (e->getDirect()) {
		if (e->ChipCX() > 0) return e->ChipCX() - 1;
		return 0;
	}
	else {
		if (e->ChipCX() < MAP_COL - 1) return e->ChipCX() + 1;
		return MAP_COL - 1;

	}
}


//�J�[�\����Y�ʒu��Ԃ�
int AstralDebu::getCursorChipY(Entity *e){
	return e->ChipCY();
}


//���݂��Ă���I�u�W�F�N�g��Ԃ�
int AstralDebu::getCursorObject(int cx,int cy,bool b){
	//�n�`�ɂ͉����ł��Ȃ�
	if (map[cx][cy])  return -2;

	//���݂��Ă���I�u�W�F�N�g������
	ALL_OBJ{
		if (i == objHolded) continue;
		Entity *tmp = getObject(i);
		if (!isBase(tmp) &&
			isTouchable(tmp) &&
			(tmp->inChip(cx, cy))) {
				//true�Ȃ玝�Ă���̂̂݁Afalse�Ȃ���i��Ԃ�
				if ((!b)||(isHoldable(tmp))) return i;
				else return -2;
		}
	}

	return -1;
}

//�I�u�W�F�N�g��Ԃ�
Entity* AstralDebu::getObject(int i){
	if (i >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "�I�u�W�F�N�g�̐�������ɒB���܂���"));
	Entity *obj = object[i];
	if (obj == NULL)
		throw(GameError(gameErrorNS::WARNING, "���݂��Ȃ��I�u�W�F�N�g���Q�Ƃ���܂���"));
	return obj;
}

//�I�u�W�F�N�g��ۑ�����
void AstralDebu::setObject(int i,Entity *e){
	if (i >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "�I�u�W�F�N�g�̐�������ɒB���܂���"));
	object[i] = e;
}