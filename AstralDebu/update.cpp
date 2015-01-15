#include "astraldebu.h"

using namespace astralNS;

//�^�C�g����ʂ̍X�V
void AstralDebu::updateTitle(){
	count = count++ % 30;

	if (input->isKeyPressed('Z')){
		state = S_STAGE;
		stage = 1;
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
