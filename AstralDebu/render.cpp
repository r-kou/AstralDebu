#include "astraldebu.h"
using namespace astralNS;

//�^�C�g����ʂ̕`��
void AstralDebu::renderTitle(){
	//�^�C�g����`��
	renderTitleBack();
	//�N���b�N�O
	if (stateNumber <= 1) renderTitleStart();
	if (stateNumber >= 2) renderTitleMenu();
	if (stateNumber == 3) renderTitleSelect();
	if (stateNumber >= 4) renderTitleVolume();
}

//�X�e�[�W�\����ʂ̕`��
void AstralDebu::renderStage(){
	graphics->spriteBegin();

	drawQuad(0, 0, (float)WINDOW_W, (float)WINDOW_H, BLACK);

	bigF.print("STAGE" + std::to_string(stage),
		0, (WINDOW_H - 60) / 2,
		WINDOW_W, WINDOW_H,
		WHITE, DT_CT);

	graphics->spriteEnd();
}

//���C���̕`��
void AstralDebu::renderMain(){
	//�w�i�h��Ԃ�
	renderBack();

	//�}�b�v�`�b�v�h��Ԃ�
	renderChip();

	//�q���g�Ƃ��`��
	renderHint();

	//�I�u�W�F�N�g�`��
	renderObject();

	//����\���i�`�[�g���j
	if (cheat2) renderEdge();

	//���x�\���i�`�[�g���j
	if (cheat4) renderVelocity();

	//���j���[�\���i���j���[���j
	if (menu) renderMenu();
}

//�N���A��ʂ̕`��
void AstralDebu::renderClear(){
	graphics->spriteBegin();

	graphics->spriteEnd();
}

//���j���[��ʂ̕`��
void AstralDebu::renderMenu(){
	using namespace menuNS;
	using namespace menuNS::mainMenuNS;
	graphics->spriteBegin();

	drawFrame(REC_FRAME(true));

	numberF.printShadow("�y�F�W�����v", REC_TEX(0, 0), 1.0f,BLACK,WHITE, DT_LC);
	numberF.printShadow("�w�F�A�C�e���������グ��", REC_TEX(0, 1),1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("�@�@�����グ���͒u��", REC_TEX(0, 2), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("�b�F�A�C�e��������", REC_TEX(0, 3), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("�@�@�����グ���͓�����", REC_TEX(0, 4), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("�p�F��蒼��", REC_TEX(0, 5), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("�Q�[���𑱂���", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, 1.0f, BLACK, WHITE, DT_CC);
	numberF.printShadow("�^�C�g���ɖ߂�", TEX_MAR_X + TEX_SEP_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, 1.0f, BLACK, WHITE, DT_CC);

	//���`��
	drawArrowHorizontal(REC_ARR(count,0),false,RED);
	graphics->spriteEnd();
}

//�^�C�g���Ɣw�i��`��
void AstralDebu::renderTitleBack(){
	graphics->spriteBegin();
	title.draw();

	bigF.print("�A�X�g�����E�f�u",
		2, 122, WINDOW_W, WINDOW_H,
		WHITE, DT_CT);

	bigF.print("�A�X�g�����E�f�u",
		0, 120, WINDOW_W, WINDOW_H,
		RED, DT_CT);

	middleF.print("�|�|���̎��f�u�͐��ɂȂ�|�|",
		0, 260, WINDOW_W, WINDOW_H,
		WHITE, DT_CT);

	graphics->spriteEnd();
}

//�X�^�[�g��`��
void AstralDebu::renderTitleStart(){
	graphics->spriteBegin();

	if (count < 20){
		bigF.print("�u�y�v�L�[�Ł@�X�^�[�g",
			0, 400, WINDOW_W, WINDOW_H,
			RED, DT_CT);
	}
	graphics->spriteEnd();
}

//���j���[��`��
void AstralDebu::renderTitleMenu(){
	using namespace menuNS;
	using namespace menuNS::titleMenuNS;
	graphics->spriteBegin();
	//���j���[��`��
	drawFrame(REC_FRAME(false));

	middleF.printShadow("�͂��߂���", REC_TEX(0, 0),1.0f, TEXT_SHOW,TEXT_HIDE, DT_CC);
	middleF.printShadow("�Â�����", REC_TEX(0, 1), 1.0f, menuText((clearedStage != 0) && (clearedStage != MAX_STAGE)), menuText((clearedStage == 0) || (clearedStage == MAX_STAGE)), DT_CC);
	middleF.printShadow("�X�e�[�W�Z���N�g", REC_TEX(0, 2), 1.0f, menuText(clearedStage != 0), menuText(clearedStage == 0), DT_CC);
	middleF.printShadow("�I�[�f�B�I", REC_TEX(0, 3), 1.0f,TEXT_SHOW, TEXT_HIDE, DT_CC);
	middleF.printShadow("�I������", REC_TEX(0, 4), 1.0f,TEXT_SHOW, TEXT_HIDE, DT_CC);
	//���`��
	if ((stateNumber >= 2) && (stateNumber <= 3)){
		drawArrowHorizontal(REC_ARR(0,count),false,RED);
	}

	graphics->spriteEnd();
}

//�X�e�[�W�Z���N�g��`��
void AstralDebu::renderTitleSelect(){
	using namespace menuNS;
	using namespace menuNS::selectMenuNS;
	graphics->spriteBegin();
	//�I�����Ă���X�e�[�W��`��
	middleF.printShadow(std::to_string(stage), REC_TEX(0,0),1.0f, WHITE,BLACK, DT_CC);
	//�㉺�̖���`��
	drawTriangleVertical(REC_TRI(0, 0), true, menuText(stage < clearedStage));
	drawTriangleVertical(REC_TRI(0,1),false, menuText(stage > 1));

	graphics->spriteEnd();
}

//���ʑI����`��
void AstralDebu::renderTitleVolume(){
	using namespace menuNS;
	using namespace menuNS::audioMenuNS;
	graphics->spriteBegin();

	drawFrame(REC_FRAME(false));

	middleF.printShadow("�a�f�l", REC_TEX(0, 0),1.0f, TEXT_SHOW, TEXT_HIDE, DT_CC);
	middleF.printShadow(doubleToString(bgmVolume), REC_TEX(2,0),1.0f, WHITE,BLACK, DT_CC);
	middleF.printShadow("���ʉ�", REC_TEX(0, 1),1.0f, TEXT_SHOW, TEXT_HIDE, DT_CC);
	middleF.printShadow(doubleToString(soundVolume), REC_TEX(2,1),1.0f, WHITE,BLACK, DT_CC);
	middleF.printShadow("�߂�", REC_TEX(1, 2),1.0f, TEXT_SHOW, TEXT_HIDE, DT_CC);

	drawArrowHorizontal(REC_ARR(0,count), false, RED);

	if (stateNumber == 5){
		drawTriangleVertical(REC_TRI(0,0), false, menuText(bgmVolume>0.0));
		drawTriangleVertical(REC_TRI(1,0), true, menuText(bgmVolume<1.0));
	}
	if (stateNumber == 6){
		drawTriangleVertical(REC_TRI(0, 1), false, menuText(soundVolume>0.0));
		drawTriangleVertical(REC_TRI(1, 1), true, menuText(soundVolume<1.0));
	}

	graphics->spriteEnd();
}

//�w�i�Ə�̂�`��
void AstralDebu::renderBack(){
	graphics->spriteBegin();

	ARGB back;
	if ((stage <= 10) || (stage == 20)) back = STG1_BACK;
	else if (stage <= 19) back = STG2_BACK;
	else if (stage <= 30) back = STG3_BACK;
	else back = STG4_BACK;

	//�w�i�h��Ԃ�
	drawQuad(0, 0, (float)WINDOW_W, (float)WINDOW_H, back);

	//�㕔�f�[�^�̈�h��Ԃ�
	drawQuad(0, 0, (float)WINDOW_W, (float)DATA_LEN, BLACK);

	//�̗̓��[�^��`��
	//�g
	drawQuad(LIFE_MAR_X - DATA_MAR, LIFE_MAR_Y - DATA_MAR,
		LIFE_LEN_X + DATA_MAR * 2, LIFE_LEN_Y + DATA_MAR * 2, WHITE);
	drawQuad(LIFE_MAR_X, LIFE_MAR_Y,
		LIFE_LEN_X, LIFE_LEN_Y, BLACK);

	//�̗�
	drawQuad(LIFE_MAR_X, LIFE_MAR_Y,
		life*DATA_MAR, LIFE_LEN_Y, LIFE_GAUGE);

	//�̗͕ω�
	if (vitalLife > life)
		drawQuad(LIFE_MAR_X + life*DATA_MAR, LIFE_MAR_Y,
		(vitalLife - life)*DATA_MAR, LIFE_LEN_Y, LIFE_VITAL_MINUS);
	else if (vitalLife < life)
		drawQuad(LIFE_MAR_X + vitalLife*DATA_MAR, LIFE_MAR_Y,
		(life - vitalLife)*DATA_MAR, LIFE_LEN_Y, LIFE_VITAL_PLUS);

	//�̗͕\��
	middleF.print("LIFE",
		WORD_MAR_X, WORD_MAR_Y,
		WORD_LEN_X, WORD_LEN_Y,
		WHITE, DT_CC);

	bigF.printShadow(std::to_string(life),
		LIFE_MAR_X, LIFE_MAR_Y,
		LIFE_LEN_X, LIFE_LEN_Y,
		2.0f,WHITE,BLACK, DT_CC);

	//�X�e�[�W�\��
	numberF.print("STAGE" + std::to_string(stage),
		STG_MAR_X, STG_MAR_Y,
		STG_LEN_X, STG_LEN_Y,
		WHITE, DT_CC);

	graphics->spriteEnd();
}

//�}�b�v�`�b�v��`��
void AstralDebu::renderChip(){
	graphics->spriteBegin();

	//�}�b�v�`��
	for (int j = 0; j < MAP_ROW; j++){
		chip.setY((float)CHIP_D(j));
		for (int i = 0; i < MAP_COL; i++){
			if (map[i][j]){
				chip.setX((float)CHIP(i));
				if (map[i][j] <= 12) chip.setCurrentFrame(setChipImage(map[i][j]));
				else chip.setCurrentFrame(map[i][j]);
				if (stage >= 31) chip.draw(graphicsNS::ALPHA50);
				else chip.draw();
			}
		}
	}

	graphics->spriteEnd();
}

//�q���g�Ƃ��`��
void AstralDebu::renderHint(){
	graphics->spriteBegin();

	switch (stage){
	case 1:
		drawPanel("Z", 3.5f, 10.25f, 1.0f, HINT_PANEL);
		drawArrowHorizontal(3.0f, 11.5f, false, HINT_ARROW);
		drawPanel("GOAL", 2.0f, 1.25f, 3.0f, HINT_PANEL);
		drawArrowHorizontal(2.5f, 2.5f, true, HINT_ARROW);
		drawPanel("��", 19.5f, 5.75f, 1.0f, HINT_PANEL);
		drawArrowVertical(20.5f, 5.0f, true, HINT_ARROW);
		break;
	case 2:
		drawPanel("C", 4.0f, 11.25f, 1.0f, HINT_PANEL);
		drawArrowHorizontal(3.5f, 12.5f, false, HINT_ARROW);
		drawPanel("X", 12.0f, 9.25f, 1.0f, HINT_PANEL);
		drawArrowHorizontal(11.5f, 10.5f, false, HINT_ARROW);
		drawPanel("X", 16.0f, 11.25f, 1.0f, HINT_PANEL);
		drawArrowVertical(17.0f, 11.0f, false, HINT_ARROW);
		drawPanel("Z", 22.0f, 10.25f, 1.0f, HINT_PANEL);
		drawArrowVertical(21.0f, 9.5f, true, HINT_ARROW);
		break;
	case 3:
		drawPanel("X��C", 4.5f, 10.25f, 2.0f, HINT_PANEL);
		drawArrowHorizontal(4.5f, 11.5f, false, HINT_ARROW);
		drawPanel("X��C", 18.5f, 8.25f, 2.0f, HINT_PANEL);
		drawArrowHorizontal(18.5f, 9.5f, true, HINT_ARROW);
		drawPanel("X", 14.0f, 0.5f, 1.0f, HINT_PANEL);
		drawArrowVertical(13.0f, 0.25f, false, HINT_ARROW);
		drawPanel(CHIP_BOMB, 10.0f, 10.5f, HINT_PANEL);
		break;
	case 4:
		drawPanelD(CHIP_BOMB,CHIP_HIBOMB, 16.25f, 10.5f, HINT_PANEL);
		break;
	case 5:
		drawPanel("��", 10.75f, 4.5f, 1.0f, HINT_PANEL);
		drawPanelD(CHIP_BOMB,CHIP_HIBOMB_BOX,18.0f,12.5f,HINT_PANEL);
		break;
	case 7:
		drawPanelD(CHIP_BOMB, CHIP_STEEL_BOX, 14.25f, 5.25f, HINT_PANEL);
		drawArrowHorizontal(14.5f,6.5f,false,HINT_ARROW);
		break;
	case 10:
		drawPanel(CHIP_WOOD_BOX, 6.0f, 10.25f, HINT_PANEL);
		drawArrowHorizontal(5.5f, 11.5f, false, HINT_ARROW);
		drawPanel(CHIP_LEAD_BOX, 15.0f, 8.25f, HINT_PANEL);
		drawArrowHorizontal(14.5f, 9.5f, false, HINT_ARROW);
		break;
	case 11:
		drawPanel("X+C", 20.5f, 4.25f, 2.0f, HINT_PANEL);
		drawArrowHorizontal(20.5f, 5.5f, true, HINT_ARROW);
		drawPanel(CHIP_HAMMER, 17.0f, 9.25f, HINT_PANEL);
		drawArrowHorizontal(16.5f, 10.5f, false, HINT_ARROW);
		break;
	case 15:
		drawArrowVertical(17.0f, 8.0f, false, HINT_ARROW);
		break;
	default:
		break;
	}

	graphics->spriteEnd();
}

//�I�u�W�F�N�g��`��
void AstralDebu::renderObject(){
	graphics->spriteBegin();

	//�I�u�W�F�N�g��`��
	for (entityNS::RENDER_ORDER ro = entityNS::RO_BASE; ro < entityNS::RO_DEBU; ro = static_cast<entityNS::RENDER_ORDER>(ro + 1)){
		ALL_OBJ if ((isMovable(object[i])&&(object[i]->isRenderOrder(ro)))) object[i]->draw();
	}

	//��������`��
	if ((objHolded >= 0)&&(object[objHolded]->isRenderOrder(entityNS::RO_HOLD))) object[objHolded]->draw();

	//�f�u��`��
	debu->draw();

	//�������̃n���}�[���̓f�u�̑O�ɕ`��
	if ((objHolded >= 0) && (object[objHolded]->isRenderOrder(entityNS::RO_HAMMER))) object[objHolded]->draw();
	//�J�[�\����`��
	drawCursor();

	graphics->spriteEnd();
}

//�f�o�b�O�p �����`��
void AstralDebu::renderEdge(){
	drawQuad((float)debu->getLeft(true), (float)debu->getTop(false),
		(float)debu->getRight(true) - debu->getLeft(true), (float)debu->getBottom(false) - debu->getTop(false), RED & graphicsNS::ALPHA50);
	drawQuad((float)debu->getLeft(false), (float)debu->getTop(true),
		(float)debu->getRight(false) - debu->getLeft(false), (float)debu->getBottom(true) - debu->getTop(true), BLUE & graphicsNS::ALPHA50);

	ALL_OBJ{
		Entity *e = getObject(i);
		if (isTouchable(e)){
			drawQuad((float)e->getLeft(true), (float)e->getTop(false),
				(float)e->getRight(true) - e->getLeft(true), (float)e->getBottom(false) - e->getTop(false), RED & graphicsNS::ALPHA50);
			drawQuad((float)e->getLeft(false), (float)e->getTop(true),
				(float)e->getRight(false) - e->getLeft(false), (float)e->getBottom(true) - e->getTop(true), BLUE & graphicsNS::ALPHA50);
		}
	}
}

//�f�o�b�O�p �����`��
void AstralDebu::renderVelocity(){
	const int BUF_SIZE = 10;
	static char buffer[BUF_SIZE];

	graphics->spriteBegin();
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getVelX());
	middleF.print(buffer, VEL_MAR_X, VEL_MAR_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getVelY());
	middleF.print(buffer, VEL_MAR_X + VEL_LEN_X, VEL_MAR_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getMovX());
	middleF.print(buffer, VEL_MAR_X, VEL_MAR_Y + VEL_LEN_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getMovY());
	middleF.print(buffer, VEL_MAR_X + VEL_LEN_X, VEL_MAR_Y + VEL_LEN_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	graphics->spriteEnd();
}

//�J�[�\����`��
void AstralDebu::drawCursor(){
	chip.setX((float)CHIP(getCursorChipX(debu)));
	chip.setY((float)CHIP_D(getCursorChipY(debu)));

	chip.setCurrentFrame(CHIP_CURSOR);
	chip.draw();
}