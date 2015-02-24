#include "astraldebu.h"

using namespace astralNS;

//�R�}���h�ǂݍ���
void AstralDebu::readCommand(){
	std::string str = input->getTextIn();

	//�n��őf��Ȃ�W�����v�ł���
	//�`�[�g���͂��ł��\
	if (in1()){
		if ((cheat1) || ((debu->getState() == entityNS::ST_STAND) && (!debu->getHold()))){
			debu->setVelY(debuNS::VEL_JUMP);
			subLife(10);
		}
	}

	//�A�C�e���擾�͒����̂Ŋ֐��ɓn��
	handleObject();

	//��蒼��
	if (input->isKeyPressed('Q')) life = 0;

	//���j���[�\��
	if (input->isKeyPressed(VK_SPACE)) {
		menu = true;
		count = 0;
		audio->playCue(audioNS::OK);
	}

	//�ȉ��`�[�g�R�}���h
	//�N���A
	if (input->isKeyPressed('W') && (cheat1)){
		debu->setState(entityNS::ST_CLEAR);
		debu->setVelX(0.0f);
		debu->setVelY(0.0f);
	}
	//��������
	if (input->isKeyPressed('E') && (cheat3)){
		Entity *tmp = object[getEmptyIndex()];
		SAFE_DELETE(tmp);
		tmp = new Blast(true);
		if (tmp->initialize(this, &bombT, debu->ChipCX(), debu->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
	}
	//�f�u���������Ȃ�
	if (str == "tabata") {
		cheat1 = !cheat1;
		input->clearTextIn();
	}
	//���肪������
	else if (str == "atari") {
		cheat2 = !cheat2;
		input->clearTextIn();
	}
	//��������
	else if (str == "tuyoi"){
		cheat3 = !cheat3;
		input->clearTextIn();
	}
}

//�I�u�W�F�N�g�̎擾����
void AstralDebu::handleObject(){
	int exist = -1;
	int cx = getCursorChipX(debu), cy = getCursorChipY(debu);

	//�����Ă鎞���n���}�[�����������L��
	if ((debu->getState() != entityNS::ST_STAND) &&
		(debu->getState() != entityNS::ST_LADDER)&&
		(debu->getState() != entityNS::ST_HAMMER)) return;

	exist = getCursorObject(cx, cy, true);

	if (in2()){
		if (debu->getHold()){
			if (exist == -1) putObject();
		}
		else{
			if (exist >= 0) holdObject(exist);
		}
	}
	if (in3()){
		if (debu->getHold()){
			if (exist == -1) throwObject();
		}
		else{
			if (exist >= 0) pushObject(exist);
		}
	}
}

//�I�u�W�F�N�g��͂�
void AstralDebu::holdObject(int exist){
	Entity *e = getObject(exist);

	if (isFood(e)){
		//����20�� ���ɂ��͑S��
		if (e->getType() == entityNS::TY_MEAT) {
			addLife(30);
			audio->playCue(audioNS::EAT_1);
		}
		else{
			life = 100;
			audio->playCue(audioNS::EAT_2);
		}
		e->setState(entityNS::ST_EMPTY);
		e->setType(entityNS::TY_NONE);
	}
	else {
		objHolded = exist;
		debu->setHold(true);

		e->setVelX(0.0f);
		e->setVelY(0.0f);
		moveHold(exist);

		if (e->getType() == entityNS::TY_HAMMER) ((Hammer*)e)->setHold(debu);
		else e->setState(entityNS::ST_LOCK);

		e->setRenderOrder(entityNS::RO_HOLD);

		switch (e->getType()){
		case entityNS::TY_WOOD_BOX:
		case entityNS::TY_BOMB_BOX:
		case entityNS::TY_AIR_BOX:
			subLife(5);
			break;
		case entityNS::TY_STEEL_BOX:
		case entityNS::TY_BOMB:
		case entityNS::TY_HIBOMB:
			subLife(10);
			break;
		case entityNS::TY_LEAD_BOX:
		case entityNS::TY_HIBOMB_BOX:
			subLife(20);
			break;
		case entityNS::TY_HAMMER:
			//�n���}�[�͎������ł̗͑͌���Ȃ�
			break;
		}
	}
}

//�I�u�W�F�N�g�����
void AstralDebu::putObject(){
	Entity *e = getObject(objHolded);

	objHolded = -1;
	debu->setHold(false);
	debu->setState(entityNS::ST_STAND);
	debu->setAnim(0.3f);

	if (e->getType() == entityNS::TY_HAMMER){
		if (e->getDirect()) e->setPosX(CHIP(debu->ChipCX() - 0.5f));
		else e->setPosX(CHIP(debu->ChipCX() + 1.5f));
	}
	else {
		e->setPosX(CHIP(getCursorChipX(debu) + 0.5f));
	}
	e->setPosY(CHIP_D(getCursorChipY(debu) + 0.5f));
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::ST_STAND);
	e->setRenderOrder(entityNS::RO_OBJECT);
	e->setEdge();
	if ((e->ChipCY() == MAP_ROW - 1) || (map[e->ChipCX()][e->ChipCY() + 1]) != 0) e->playPut();

}

//�I�u�W�F�N�g������
void AstralDebu::pushObject(int exist){
	int cx = 0, cy = getCursorChipY(debu);
	Entity *e = getObject(exist);

	//�n���}�[�Ƃɂ��͉����Ȃ�
	if ((e->getType() == entityNS::TY_HAMMER) || (isFood(e))) return;

	if (debu->getDirect()) {
		if (debu->ChipCX() > 1) cx = debu->ChipCX() - 2;
		else return;
	}
	else {
		if (debu->ChipCX() < MAP_COL - 2) cx = debu->ChipCX() + 2;
		else return;
	}
	if (getCursorObject(cx, cy, false) != -1) return;
	debu->setAnim(0.3f);

	e->setPosX(CHIP(cx + 0.5f));
	e->setPosY(CHIP_D(cy + 0.5f));
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::ST_STAND);
	e->setEdge();
	e->playPut();

	//�̗͌���
	switch (e->getType()){
	case entityNS::TY_WOOD_BOX:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_AIR_BOX:
		//�ؔ��̗͑͌����Ȃ�
		break;
	case entityNS::TY_STEEL_BOX:
	case entityNS::TY_BOMB:
	case entityNS::TY_HIBOMB:
		subLife(5);
		break;
	case entityNS::TY_LEAD_BOX:
	case entityNS::TY_HIBOMB_BOX:
		subLife(10);
		break;
	}
}

//�I�u�W�F�N�g�𓊂���
void AstralDebu::throwObject(){
	Entity *e = getObject(objHolded);

	objHolded = -1;
	debu->setHold(false);
	debu->setState(entityNS::ST_STAND);
	debu->setAnim(0.3f);

	e->setPosX(CHIP(getCursorChipX(debu) + 0.5f));
	e->setPosY(CHIP_D(getCursorChipY(debu) + 0.5f));
	if (debu->getDirect()) e->setVelX(-VEL_THROW);
	else e->setVelX(VEL_THROW);
	e->setVelY(0.0f);
	e->setState(entityNS::ST_KNOCK);
	e->setEdge();
	e->setRenderOrder(entityNS::RO_OBJECT);

	//�̗͌���
	switch (e->getType()){
	case entityNS::TY_WOOD_BOX:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_AIR_BOX:
		subLife(5);
		audio->playCue(audioNS::THROW_WOOD);
		break;
	case entityNS::TY_STEEL_BOX:
	case entityNS::TY_BOMB:
	case entityNS::TY_HIBOMB:
		subLife(10);
		audio->playCue(audioNS::THROW_STEEL);
		break;
	case entityNS::TY_LEAD_BOX:
	case entityNS::TY_HIBOMB_BOX:
		subLife(20);
		audio->playCue(audioNS::THROW_LEAD);
		break;
	case entityNS::TY_HAMMER:
		subLife(40);
		audio->playCue(audioNS::THROW_HAMMER);
		break;
	}
}

//�����I�u�W�F�N�g���f�u�̑O�Ɉړ�
void AstralDebu::moveHold(int i){
	Entity *e = getObject(i);

	if (e->getType() == entityNS::TY_HAMMER) {
		debu->setHammer(debu->getPosX() >= e->getPosX());
	}
	else {
		if (debu->getDirect()) e->setPosX(debu->getPosX() - HOLD_MAR_X);
		else e->setPosX(debu->getPosX() + HOLD_MAR_X);
		e->setPosY(debu->getPosY() - HOLD_MAR_Y);
	}

}

//�I�u�W�F�N�g�̏��ŏ���
void AstralDebu::deadObject(int i){
	Entity *e = getObject(i);
	int cx = e->ChipCX(), cy = e->ChipCY();

	switch (e->getType()){
	case entityNS::TY_BOMB:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_MISSILE:
	case entityNS::TY_MINE:
		//�������Ă��甚���𐶐�����
		SAFE_DELETE(e);
		e = new Blast(false);
		if (!e->initialize(this, &bombT, cx, cy))
			throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
		setObject(i,e);
		audio->playCue(audioNS::BLAST1);
		break;
	case entityNS::TY_HIBOMB:
	case entityNS::TY_HIBOMB_BOX:
		//�������Ă��炷���������𐶐�����
		SAFE_DELETE(e);
		e = new Blast(true);
		if (!e->initialize(this, &bombT, cx, cy))
			throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
		setObject(i, e);
		audio->playCue(audioNS::BLAST2);
		break;
	default:
		//���� �z�񂩂�̍폜�͂��Ȃ�
		e->setState(entityNS::ST_EMPTY);
		e->setType(entityNS::TY_NONE);
		break;
	}
}

//�I�u�W�F�N�g�̓���s��
void AstralDebu::actionObject(int i){
	int n;
	int cx,cy;
	Entity *e = getObject(i), *ne;

	switch (e->getType()){
	case entityNS::TY_ENEMY_3:
		//�e�e����
		e->setAction(false);
		n = getEmptyIndex();
		ne = object[n];
		SAFE_DELETE(ne);
		ne = new Bullet();
		if (!ne->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
		ne->setDirect(e->getDirect());
		ne->setState(entityNS::ST_KNOCK);
		setObject(n, ne);
		break;
	case entityNS::TY_ENEMY_4:
		cx = getCursorChipX(e), cy = getCursorChipY(e);
		n = getCursorObject(cx, cy, true);
		e->setAction(false);
		if (n < 0) {
			e->setDirect(!e->getDirect());
			return;
		}

		if (e->getDirect()) cx--;
		else cx++;
		if ((cx < 0) || (cx >= MAP_COL)) {
			e->setDirect(!e->getDirect());
			return;
		}

		e->setAnim(0.3f);

		if (getCursorObject(cx, cy, false) != -1) {
			e->setDirect(!e->getDirect());
			return;
		}

		ne = getObject(n);
		ne->setPosX(CHIP(cx + 0.5f));
		ne->setPosY(CHIP_D(cy + 0.5f));
		ne->setVelX(0.0f);
		ne->setVelY(0.0f);
		ne->setState(entityNS::ST_STAND);
		ne->setEdge();
		ne->playPut();
		break;
	case entityNS::TY_ENEMY_5:
		//�~�T�C������
		e->setAction(false);
		n = getEmptyIndex();
		ne = object[n];
		SAFE_DELETE(ne);
		ne = new Missile();
		if (!ne->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "�I�u�W�F�N�g�̏������Ɏ��s���܂���"));
		ne->setDirect(e->getDirect());
		ne->setState(entityNS::ST_KNOCK);
		setObject(n, ne);
		break;
	}
}
