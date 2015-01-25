#include "astraldebu.h"

using namespace astralNS;

//�^�C�g����ʂ̍X�V
void AstralDebu::updateTitle(){
	if (state_num == 0) loadData();
	else if (state_num == 1){
		count = count++ % 30;
		if (input->isKeyPressed('Z')){
			if(stage_max == 0)count = 0;
			else count = 1;
			state_num = 2;
		}
	} else if (state_num == 2){
		if (stage_max != 0){
			if (input->isKeyPressed(VK_UP)) count -= (count != 0) ? 1 : -3;
			if (input->isKeyPressed(VK_DOWN)) count += (count != 3) ? 1 : -3;
		}
		if (input->isKeyPressed('Z')){
			switch (count){
			case 0:
				state = S_STAGE;
				stage = 1;
				read = false;
				break;
			case 1:
				state = S_STAGE;
				stage = stage_max+1;
				read = false;
				break;
			case 2:
				state_num = 3;
				stage = stage_max;
				break;
			case 3:
				break;
			}
		}
	}
	else if (state_num == 3) {
		if (input->isKeyPressed(VK_UP)) stage += (stage != stage_max)  ? 1 : -stage_max + 1;
		if (input->isKeyPressed(VK_DOWN)) stage -= (stage != 1) ? 1 : -stage_max + 1;
		if (input->isKeyPressed('Z')) {
			state = S_STAGE;
			read = false;
		}
		if (input->isKeyPressed('X')) state_num = 2;
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
		stage_start = timeGetTime();
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
		updateClearTime();
		if (stage>stage_max) stage_max = stage;
		saveData();
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

//�Z�[�u�f�[�^�̓ǂݍ���
void AstralDebu::loadData(){
	std::ifstream save(SAV_NAME, std::ios::in | std::ios::binary);
	int buf_stage;
	double buf_time;
	if (save){
		try{
			//�N���A�����X�e�[�W��ǂݍ���
			save.read((char *)&buf_stage, sizeof(int));
			stage_max = buf_stage;
			//�N���A���Ԃ�ǂݍ���
			FOR(STG_SIZE) {
				save.read((char *)&buf_time, sizeof(double));
				clear_time[i] = buf_time;
			}
			save.close();
		}
		catch (...){
			save.close();
			throw(GameError(gameErrorNS::FATAL, "Error loading data."));
		}
	}
	else {
		//�N���A�����X�e�[�W��������
		stage_max = 0;
		//�N���A���Ԃ�������
		FOR(STG_SIZE) clear_time[i] = 0;
	}
	state_num = 1;
}

//�Z�[�u�f�[�^�̏�������
void AstralDebu::saveData(){
	std::ofstream save(SAV_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
	int buf_stage = stage_max;
	if (save){
		//�N���A�����X�e�[�W��ǂݍ���
		save.write((char *)&buf_stage, sizeof(int));
		//�N���A���Ԃ�ǂݍ���
		FOR(STG_SIZE) {
			save.write((char *)&clear_time[i], sizeof(double));
		}
		save.close();
	}
	else throw(GameError(gameErrorNS::FATAL, "Error saving data."));
}


//�X�e�[�W�̓ǂݍ���
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

void AstralDebu::updateClearTime(){
	//�`�[�g���͋L�^���Ȃ�
	if (cheat) return;
	//�I���������擾
	stage_end = timeGetTime();
	double total_time = (double)(stage_end - stage_start) / 1000;
	//���L�^�Ȃ�X�V
	if (clear_time[stage - 1] == 0) clear_time[stage - 1] = total_time;
	//�ȑO�̋L�^��葁���Ȃ�X�V
	else if (total_time < clear_time[stage - 1]) clear_time[stage - 1] = total_time;
}