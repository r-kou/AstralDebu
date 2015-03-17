#include "astraldebu.h"

using namespace astralNS;

//�R���X�g���N�^
AstralDebu::AstralDebu(){
	stage = 0;
	count = 0;
	fCount = 0;
	life = 0;
	vitalLife = 0;
	bgmVolume = 0.0;
	soundVolume = 0.0;
	timeout = 0.0f;
	read = false;
	clear = false;
	bgm = false;
	initializeFont = false;
	menu = false;
	state = S_TITLE;
	stateNumber = 0;
	clearTimeStart = 0;
	clearTimeEnd = 0;
	clearedStage = 0;
	//�N���A���Ԃ�������
	FOR(STG_SIZE) clearTime[i] = 0;
	//�}�b�v��������
	FOR_2(MAP_COL, MAP_ROW){
		map[i][j] = 0;
	}
	//�I�u�W�F�N�g��������
	debu = NULL;
	ALL_OBJ object[i] = NULL;
	objMax = 0;
	objHolded = -1;
	vertex = NULL;
	cheat = false;
	cheat1 = false;
	cheat2 = false;
	cheat3 = false;
	cheat4 = false;
}

//�f�X�g���N�^
AstralDebu::~AstralDebu(){
	deleteAll();

	//�t�H���g�폜
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (!RemoveFontResourceEx(DATA(FON_FILE), FR_PRIVATE, NULL))
		throw(GameError(gameErrorNS::FATAL, "�t�H���g�f�[�^�̏������Ɏ��s���܂���"));
#endif
}

//������
void AstralDebu::initialize(HWND hwnd){
	Game::initialize(hwnd);

	//�t�H���g�ǉ�
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (AddFontResourceEx(DATA(FON_FILE), FR_PRIVATE, NULL)) initializeFont = true;
#endif

	//�摜�e�N�X�`����ݒ�
	initTexture(debuT, IMG_FILE_DEBU);
	initTexture(chipT, IMG_FILE_CHIP);
	initTexture(bombT, IMG_FILE_BLAST);
	initTexture(enemyT, IMG_FILE_ENEMY);
	initTexture(titleT, IMG_FILE_TITLE);

	//�摜��ݒ�
	if (!title.initialize(graphics, &titleT, 0, 0, 0))
		throw(GameError(gameErrorNS::FATAL, "�摜�f�[�^�̏������Ɏ��s���܂���"));
	if (!chip.initialize(graphics, &chipT, CHIP_SIZE, CHIP_SIZE, IMG_COL_CHIP))
		throw(GameError(gameErrorNS::FATAL, "�摜�f�[�^�̏������Ɏ��s���܂���"));

	//�t�H���g��ݒ�
	initFont(stageF, 80);
	initFont(numberF, 48);
	initFont(bigF, 64);
	initFont(middleF, 32);
	initFont(smallF, 16);

	//��l����ݒ�
	debu = new Debu();
}

//�e�N�X�`���̏�����
void AstralDebu::initTexture(Texture &t, std::string file){
	if (!t.initialize(graphics, IMG(file)))
		throw(GameError(gameErrorNS::FATAL, "�摜�f�[�^�̏������Ɏ��s���܂���"));
}

//�t�H���g�̏�����
void AstralDebu::initFont(Text &t, int point){
	if (t.initialize(graphics, point, false, false, (initializeFont?GEN:MEI)) == false)
		throw(GameError(gameErrorNS::FATAL, "�t�H���g�̏������Ɏ��s���܂���"));
	t.setFontColor(WHITE);
}

//�����̍X�V
void AstralDebu::update(){
	//FPS�̕\��
	if (input->isKeyPressed('F')) fpsOn = !fpsOn;

	//�Q�[����Ԃɉ����ē��e�ύX
	switch (state){
	case S_TITLE:
		updateTitle();
		break;
	case S_STAGE:
		updateStage();
		break;
	case S_MAIN:
		updateMain();
		break;
	case S_CLEAR:
		updateClear();
		break;
	}
}

//�`��
void AstralDebu::render(){
	//�Q�[����Ԃɉ����ē��e�ύX
	switch (state){
	case S_TITLE:
		renderTitle();
		break;
	case S_STAGE:
		renderStage();
		break;
	case S_MAIN:
		renderMain();
		break;
	case S_CLEAR:
		renderClear();
		break;
	}
	graphics->spriteBegin();
	if (cheat1) text->print("�X�S�C�f�u", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	if (cheat2) text->print("�n���e�C", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y * 2, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	if (cheat3) text->print("�W�o�N", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y * 3, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	if (cheat4) text->print("�\�N�h", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y * 4, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	graphics->spriteEnd();
}

//�|�C���^�̉��
void AstralDebu::releaseAll(){
	//�t�H���g��j��
	stageF.onLostDevice();
	numberF.onLostDevice();
	bigF.onLostDevice();
	middleF.onLostDevice();
	smallF.onLostDevice();

	//�e�N�X�`����j��
	debuT.onLostDevice();
	chipT.onLostDevice();
	bombT.onLostDevice();
	enemyT.onLostDevice();
	titleT.onLostDevice();

	//�Z�a��j��
	SAFE_RELEASE(vertex);

	Game::releaseAll();
	return;
}

//�|�C���^�̉��
void AstralDebu::resetAll(){
	//�t�H���g�����Z�b�g
	stageF.onResetDevice();
	numberF.onResetDevice();
	bigF.onResetDevice();
	middleF.onResetDevice();
	smallF.onResetDevice();

	//�e�N�X�`�������Z�b�g
	debuT.onResetDevice();
	chipT.onResetDevice();
	bombT.onResetDevice();
	enemyT.onResetDevice();
	titleT.onResetDevice();

	//�Z�a�����Z�b�g
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);

	Game::resetAll();
	return;
}


//�|�C���^�̉��
void AstralDebu::deleteAll(){
	//�����[�X�����s
	releaseAll();

	//�I�u�W�F�N�g�f�[�^��j��
	SAFE_DELETE(debu);
	FOR(OBJ_SIZE) SAFE_DELETE(object[i]);

	Game::deleteAll();
	return;
}