#include "astraldebu.h"

using namespace astralNS;

//�^�C�g����ʂ̍X�V
void AstralDebu::updateTitle(){
	count = count++ % 30;

	if (input->isKeyPressed('Z')){
		state = S_STAGE;
		stage = 17;
		read = false;
	}
}

//�X�e�[�W�\����ʂ̍X�V
void AstralDebu::updateStage(){
	if (!read) loadStage();

	if (input->isKeyPressed('Z') && read){
		clear = false;
		life = 100;
		life_v = 100;
		state = S_MAIN;
	}
}

//���C���̍X�V
void AstralDebu::updateMain(){
	if (life_v > life) life_v--;
	else if (life_v < life) life_v++;

	if (life == 0) debu->setState(entityNS::DEAD);

	if ((debu->getState() == entityNS::DEAD) && (debu->getComplete())) {
		state = S_STAGE;
		read = false;
	}

	if ((debu->getState() == entityNS::CLEAR) && (debu->getComplete())) clear = true;

	if (clear) {
		clear = false;
		stage++;
		if (stage > 21){
			state = S_CLEAR;
		}
		else {
			state = S_STAGE;
			read = false;
		}
	}

	readCommand();

	//�ړ�
	debu->move(frameTime);
	ALL_OBJ if (canMove(object[i]))
		object[i]->move(frameTime);

	//�����I�u�W�F�N�g�̓f�u�̑O�Ɉړ�
	if (obj_hold >= 0) moveHold(obj_hold);

	ALL_OBJ if (object[i]->getAction())
		actionObject(i);

	//�n�`�ւ̐ڐG
	if (canMove(debu))
		debu->touchMap(map);
	ALL_OBJ if (canMove(object[i]))
		object[i]->touchMap(map);

	debu->resetResponse();
	ALL_OBJ object[i]->resetResponse();

	//���I�u�W�F�N�g�ւ̐ڐG
	if (canTouch(debu))
		ALL_OBJ if (canTouch(object[i]))
		debu->touchObj(object[i]);
	ALL_OBJ_D if (canTouch(object[i]) && canTouch(object[j]))
		object[i]->touchObj(object[j]);

	//�I�u�W�F�N�g�ւ̔������s
	if (canTouch(debu)) debu->responseObj();
	ALL_OBJ if (canTouch(object[i]))
		object[i]->responseObj();

	//�I�u�W�F�N�g���ŏ���
	ALL_OBJ if (object[i]->isDead())
		deadObject(i);

	//�`�悷��摜��ύX
	debu->changeImage();
	ALL_OBJ object[i]->changeImage();

}

//�N���A��ʂ̍X�V
void AstralDebu::updateClear(){

}

void AstralDebu::loadStage(){
	char buf[256];
	std::ifstream file(MAP_DIR + "\\" + MAP_NAME(stage) + MAP_EXT);

	//�Â��I�u�W�F�N�g�����ׂď���
	ALL_OBJ	SAFE_DELETE(object[i]);
	obj_num = 0;
	obj_hold = -1;
	warp_r = -1;
	warp_g = -1;
	warp_y = -1;


	if (file.fail())
		throw(GameError(gameErrorNS::FATAL, "Error reading map file"));

	for (int j = 0; j < MAP_ROW; j++){
		file >> buf;
		for (int i = 0; i < MAP_COL; i++){
			loadChip(i, j, buf[i]);
		}
	}
	read = true;
}

void AstralDebu::loadChip(int i, int j, char c){
	switch (c){
	case '0': //��
		map[i][j] = 0;
		break;
	case '1': //��
		map[i][j] = 1;
		break;
	case '2': //��
		map[i][j] = 2;
		break;
	case '3': //��
		map[i][j] = 3;
		break;
	case '4': //���{��
		map[i][j] = 4;
		break;
	case '5': //����
		map[i][j] = 5;
		break;
	case '6': //����
		map[i][j] = 6;
		break;
	case '7': //����
		map[i][j] = 7;
		break;
	case '8': //�����{��
		map[i][j] = 8;
		break;
	case '9': //��
		addObject(new Rock(stage), chipT, i, j);
		break;
	case 'A': //�ؔ�
		addObject(new WoodBox(), chipT, i, j);
		break;
	case 'B': //�S��
		addObject(new SteelBox(), chipT, i, j);
		break;
	case 'C': //����
		addObject(new LeadBox(), chipT, i, j);
		break;
	case 'D': //���e��
		addObject(new BombBox(), chipT, i, j);
		break;
	case 'E': //�����e��
		addObject(new HibombBox(), chipT, i, j);
		break;
	case 'F': //���e
		addObject(new Bomb(), chipT, i, j);
		break;
	case 'G': //�����e
		addObject(new Hibomb(), chipT, i, j);
		break;
	case 'H': //�ɂ�
		addObject(new Meat(), chipT, i, j);
		break;
	case 'I': //���ɂ�
		addObject(new Himeat(), chipT, i, j);
		break;
	case 'J': //�͂���
		map[i][j] = CHIP_LADDER;
		break;
	case 'K': //�G(��������)
		addEnemy(new Enemy1(), enemyT, debu, i, j);
		break;
	case 'L': //�G(�������Ɍ������Ă���)
		addEnemy(new Enemy2(), enemyT, debu, i, j);
		break;
	case 'M': //�G(�������ɒe������)
		addEnemy(new Enemy3(), enemyT, debu, i, j);
		break;
	case 'N': //�G(��������Ɖ���)
		//addEnemy(new Enemy4(), enemyT, debu, i, j);
		break;
	//case 'O':�[���Ƌ�ʂ���̂Ō���
	case 'P': //�G(�������Ƀ~�T�C��)
		addEnemy(new Enemy5(), enemyT, debu, i, j);
		break;
	case 'Q': //�S��
		addObject(new Hammer(), chipT, i, j);
		break;
	case 'R': //�ԃ��[�v
		addObject(new Warp(entityNS::WARP_R), chipT, i, j);
		if (warp_r == -1) warp_r = obj_num - 1;
		else addWarp(warp_r, obj_num - 1);
		break;
	case 'S': //�΃��[�v
		addObject(new Warp(entityNS::WARP_G), chipT, i, j);
		if (warp_g == -1) warp_g = obj_num - 1;
		else addWarp(warp_g, obj_num - 1);
		break;
	case 'T': //�����[�v
		addObject(new Warp(entityNS::WARP_Y), chipT, i, j);
		if (warp_y == -1) warp_y = obj_num - 1;
		else addWarp(warp_y, obj_num - 1);
		break;
	case 'U': //��C��
		addObject(new AirBox(), chipT, i, j);
		break;
	case 'V': //�t���[����
		addObject(new FrameBox(), chipT, i, j);
		break;
	case 'X': //�S�[��
		addObject(new Goal(), chipT, i, j);
		break;
	case 'Z': //�f�u
		map[i][j] = 0;
		if (!debu->initialize(this, &debuT, input, i, j))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	default:
		map[i][j] = 0;
		break;
	}
}

//�R�}���h�ǂݍ���
void AstralDebu::readCommand(){
	std::string str = input->getTextIn();

	//�n��őf��Ȃ�W�����v�ł���
	//�`�[�g���͂��ł��\
	if (input->isKeyPressed('Z')){
		if ((cheat1) || ((debu->getState() == entityNS::STAND) && (!debu->getHold()))){
			debu->setVelY(debuNS::VEL_JUMP);
			subLife(10);
		}
	}

	//�A�C�e���擾�͒����̂Ŋ֐��ɓn��
	handleObject();

	//��蒼��
	if (input->isKeyPressed('Q')) life = 0;

	//�ȉ��`�[�g�R�}���h
	//�N���A
	if (input->isKeyPressed('W') && (cheat1)){
		debu->setState(entityNS::CLEAR);
		debu->setVelX(0.0f);
		debu->setVelY(0.0f);
	}
	//�S��
	if (input->isKeyPressed('E') && (cheat1)){
		life = 100;
	}
	//�f�u���������Ȃ�
	if (str == "tabata") {
		cheat1 = !cheat1;
		input->clearTextIn();
	}
	//���肪������
	else if (str == "mieru") {
		cheat2 = !cheat2;
		input->clearTextIn();
	}
}

//�I�u�W�F�N�g�̒ǉ�
void AstralDebu::addObject(Entity *e, Texture &t, int i, int j){
	map[i][j] = 0;
	object[obj_num] = e;
	if (!e->initialize(this, &t, i, j))
		throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
	obj_num++;
}

//�G�̒ǉ��@�f�u�̈ʒu������
void AstralDebu::addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j){
	map[i][j] = 0;
	object[obj_num] = e;
	if (!e->initialize(this, &t, d, i, j))
		throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
	obj_num++;
}

//���[�v�̒ǉ�
void AstralDebu::addWarp(int i, int j){
	object[i]->setPartnerX(object[j]->ChipCX());
	object[i]->setPartnerY(object[j]->ChipCY());
	object[j]->setPartnerX(object[i]->ChipCX());
	object[j]->setPartnerY(object[i]->ChipCY());
}

//�I�u�W�F�N�g�̎擾����
void AstralDebu::handleObject(){
	int exist = -1;

	//�����Ă鎞���n���}�[�����������L��
	if ((debu->getState() != entityNS::STAND) && (debu->getState() != entityNS::HOLD_HAMMER)) return;
	//�n�`�ɂ͉����ł��Ȃ�
	if ((map[cursorX()][cursorY()] != 0) && (map[cursorX()][cursorY()] != CHIP_LADDER))  return;

	//���݂��Ă���I�u�W�F�N�g������
	ALL_OBJ{
		if ((i != obj_hold) &&
		(object[i]->ChipCX() == cursorX()) &&
		(object[i]->ChipCY() == cursorY()) &&
		canTouch(object[i])) {
			if (canHold(object[i])) exist = i;
			else return;
		}
	}

		if (input->isKeyPressed('X')){
		if (debu->getHold()){
			if (exist < 0) putObject();
		}
		else{
			if (exist >= 0) holdObject(exist);
		}
		}
	if (input->isKeyPressed('C')){
		if (debu->getHold()){
			if (exist < 0) throwObject();
		}
		else{
			if (exist >= 0) pushObject(exist);
		}
	}
}

//�I�u�W�F�N�g��͂�
void AstralDebu::holdObject(int i){
	Entity *e = object[i];

	if (canEat(e)){
		//����20�� ���ɂ��͑S��
		if (e->getType() == entityNS::MEAT) addLife(30);
		else life = 100;
		e->setState(entityNS::EMPTY);
		e->setType(entityNS::NONE);
	}
	else {
		obj_hold = i;
		debu->setHold(true);

		e->setVelX(0.0f);
		e->setVelY(0.0f);
		moveHold(i);

		if (e->getType() == entityNS::HAMMER) ((Hammer*)e)->setHold(debu);
		else e->setState(entityNS::LOCK);

		switch (e->getType()){
		case entityNS::BOX_W:
		case entityNS::BOX_B:
		case entityNS::BOX_A:
			subLife(5);
			break;
		case entityNS::BOX_S:
		case entityNS::BOMB:
		case entityNS::HIBOMB:
			subLife(10);
			break;
		case entityNS::BOX_L:
		case entityNS::BOX_H:
			subLife(20);
			break;
		case entityNS::HAMMER:
			//�n���}�[�͎������ł̗͑͌���Ȃ�
			break;
		}
	}
}

//�I�u�W�F�N�g�����
void AstralDebu::putObject(){
	Entity *e = object[obj_hold];

	obj_hold = -1;
	debu->setHold(false);
	debu->setState(entityNS::STAND);

	if (e->getType() == entityNS::HAMMER){
		if (e->getDirect()) e->setPosX((debu->ChipCX() - 0.5f) * CHIP_SIZE);
		else e->setPosX((debu->ChipCX() + 1.5f) * CHIP_SIZE);
	}
	else {
		e->setPosX((cursorX() + 0.5f) * CHIP_SIZE);
	}
	e->setPosY((cursorY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::STAND);
	e->setEdge();

}

//�I�u�W�F�N�g������
void AstralDebu::pushObject(int exist){
	int cursorNX = 0;
	Entity *e = object[exist];

	//�n���}�[�Ƃɂ��͉����Ȃ�
	if ((e->getType() == entityNS::HAMMER) || (canEat(e))) return;

	if (debu->getDirect()) {
		if (debu->ChipCX() > 1) cursorNX = debu->ChipCX() - 2;
		else return;
	}
	else {
		if (debu->ChipCX() < MAP_COL - 2) cursorNX = debu->ChipCX() + 2;
		else return;
	}

	//������ɉ�����������s��
	if ((map[cursorNX][cursorY()] != 0) && (map[cursorNX][cursorY()] != CHIP_LADDER)) return;
	ALL_OBJ{
		if ((object[i]->ChipCX() == cursorNX) &&
		(object[i]->ChipCY() == cursorY()) &&
		canTouch(object[i])) return;
	}

	e->setPosX((cursorNX + 0.5f) * CHIP_SIZE);
	e->setPosY((cursorY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::STAND);
	e->setEdge();

	//�̗͌���
	switch (e->getType()){
	case entityNS::BOX_W:
	case entityNS::BOX_B:
	case entityNS::BOX_A:
		//�ؔ��̗͑͌����Ȃ�
		break;
	case entityNS::BOX_S:
	case entityNS::BOMB:
	case entityNS::HIBOMB:
		subLife(5);
		break;
	case entityNS::BOX_L:
	case entityNS::BOX_H:
		subLife(10);
		break;
	}
}

//�I�u�W�F�N�g�𓊂���
void AstralDebu::throwObject(){
	Entity *e = object[obj_hold];

	obj_hold = -1;
	debu->setHold(false);
	debu->setState(entityNS::STAND);

	e->setPosX((cursorX() + 0.5f) * CHIP_SIZE);
	e->setPosY((cursorY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	if (debu->getDirect()) e->setVelX(-VEL_THROW);
	else e->setVelX(VEL_THROW);
	e->setVelY(0.0f);
	e->setState(entityNS::KNOCK);
	e->setEdge();

	//�̗͌���
	switch (e->getType()){
	case entityNS::BOX_W:
	case entityNS::BOX_B:
	case entityNS::BOX_A:
		subLife(5);
		break;
	case entityNS::BOX_S:
	case entityNS::BOMB:
	case entityNS::HIBOMB:
		subLife(10);
		break;
	case entityNS::BOX_L:
	case entityNS::BOX_H:
		subLife(20);
		break;
	case entityNS::HAMMER:
		subLife(40);
		break;
	}
}

//�����I�u�W�F�N�g���f�u�̑O�Ɉړ�
void AstralDebu::moveHold(int i){
	Entity *e = object[i];

	if (e->getType() == entityNS::HAMMER) {
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
	Entity *e = object[i];
	int x = e->ChipCX(), y = e->ChipCY();

	switch (e->getType()){
	case entityNS::BOMB:
	case entityNS::BOX_B:
	case entityNS::EN_M:
		//�������Ă��甚���𐶐�����
		SAFE_DELETE(e);
		object[i] = new Blast(false);
		if (!object[i]->initialize(this, &bombT, x, y))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	case entityNS::HIBOMB:
	case entityNS::BOX_H:
		//�������Ă��炷���������𐶐�����
		SAFE_DELETE(e);
		object[i] = new Blast(true);
		if (!object[i]->initialize(this, &bombT, x, y))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	default:
		//���� �z�񂩂�̍폜�͂��Ȃ�
		e->setState(entityNS::EMPTY);
		e->setType(entityNS::NONE);
		break;
	}
}

//�I�u�W�F�N�g�̓���s��
void AstralDebu::actionObject(int i){
	Entity *e = object[i];
	int tmp = obj_num;
	switch (e->getType()){
	case entityNS::EN_3:
		//�e�e����
		e->setAction(false);
		ALL_OBJ if ((object[i]->getType() == entityNS::NONE) && (object[i]->getState() == entityNS::EMPTY)) {
			tmp = i;
			break;
		}
		if (tmp == obj_num) obj_num++;
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Bullet();
		if (!object[tmp]->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		object[tmp]->setDirect(e->getDirect());
		object[tmp]->setState(entityNS::KNOCK);
		break;
	case entityNS::EN_4:
		break;
	case entityNS::EN_5:
		//�~�T�C������
		e->setAction(false);
		ALL_OBJ if ((object[i]->getType() == entityNS::NONE) && (object[i]->getState() == entityNS::EMPTY)) {
			tmp = i;
			break;
		}
		if (tmp == obj_num) obj_num++;
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Missile();
		if (!object[tmp]->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		object[tmp]->setDirect(e->getDirect());
		object[tmp]->setState(entityNS::KNOCK);
		break;
	}
}
