#include "astraldebu.h"

using namespace astralNS;

//�R���X�g���N�^
AstralDebu::AstralDebu(){
	stage = 0;
	count = 0;
	life = 0;
	vitalLife = 0;
	read = false;
	clear = false;
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
}

//�f�X�g���N�^
AstralDebu::~AstralDebu(){
	deleteAll();
}

//������
void AstralDebu::initialize(HWND hwnd){
	Game::initialize(hwnd);

	//�摜�e�N�X�`����ݒ�
	initTexture(debuT, IMG_FILE_DEBU);
	initTexture(chipT, IMG_FILE_CHIP);
	initTexture(bombT, IMG_FILE_BLAST);
	initTexture(enemyT, IMG_FILE_ENEMY);
	initTexture(titleT, IMG_FILE_TITLE);

	//�摜��ݒ�
	if (!title.initialize(graphics, &titleT, 0, 0, 0))
		throw(GameError(gameErrorNS::FATAL, "Error initializing image."));
	if (!chip.initialize(graphics, &chipT, CHIP_SIZE, CHIP_SIZE, IMG_COL_CHIP))
		throw(GameError(gameErrorNS::FATAL, "Error initializing image."));

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
	if (!t.initialize(graphics, (IMG_FILE_DIR + "\\" + file).c_str()))
		throw(GameError(gameErrorNS::FATAL, "Error initializing texture."));
}

//�t�H���g�̏�����
void AstralDebu::initFont(Text &t, int point){
	if (t.initialize(graphics, point, true, false, FONT) == false)
		throw(GameError(gameErrorNS::FATAL, "Error initializing font."));
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
}

//�l�p�̕`��
void AstralDebu::setVertex(float l, float t, float r, float b, ARGB c){
	//����
	vtx[0].x = l;
	vtx[0].y = t;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//�E��
	vtx[1].x = l + r;
	vtx[1].y = t;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//�E��
	vtx[2].x = l + r;
	vtx[2].y = t + b;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//����
	vtx[3].x = l;
	vtx[3].y = t + b;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//�l�p��`��
void AstralDebu::drawQuad(float l, float t, float r, float b, ARGB c){
	//�Z�a��ݒ�
	setVertex(l, t, r, b, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphics�ɔC����
	graphics->drawQuad(vertex);
}

//�O�p��vertex��`��
void AstralDebu::setVertexTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//��
	vtx[0].x = x1;
	vtx[0].y = y1;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//�E
	vtx[1].x = x2;
	vtx[1].y = y2;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//��
	vtx[2].x = x3;
	vtx[2].y = y3;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//��ɋA���Ă���
	vtx[3].x = x1;
	vtx[3].y = y1;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//�O�p��`��
void AstralDebu::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//�Z�a��ݒ�
	setVertexTriangle(x1, y1, x2, y2, x3, y3, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphcs�ɔC����
	graphics->drawQuad(vertex);
}

//�J�[�\����X�ʒu��Ԃ�
int AstralDebu::getCursorChipX(){
	if (debu->getDirect()) {
		if (debu->ChipCX() > 0) return debu->ChipCX() - 1;
		return 0;
	}
	else {
		if (debu->ChipCX() < MAP_COL - 1) return debu->ChipCX() + 1;
		return MAP_COL - 1;

	}
}

//�J�[�\����Y�ʒu��Ԃ�
int AstralDebu::getCursorChipY(){
	return debu->ChipCY();
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