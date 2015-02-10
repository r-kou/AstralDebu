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
	if (cheat2) drawEdge();

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
	using namespace mainMenuNS;
	graphics->spriteBegin();

	drawFrame(MAR_X,MAR_Y,LEN_X,LEN_Y,FRAME,SHADOW,BACK&graphicsNS::ALPHA90,BACK,BACK_SHADE,BACK_LIGHT);

	numberF.print("�y�F�W�����v", TEX_MAR_X, TEX_MAR_Y, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("�w�F�A�C�e���������グ��", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("�@�@�����グ���͒u��", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 2, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("�b�F�A�C�e��������", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 3, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("�@�F�����グ���͓�����", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 4, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("�p�F��蒼��", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 5, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("�Q�[���𑱂���", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, BLACK, DT_CC);
	numberF.print("�^�C�g���ɖ߂�", TEX_MAR_X + TEX_SEP_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, BLACK, DT_CC);
	
	//���`��
	drawArrowHorizontal(ARR_MAR_X + (count ? TEX_SEP_X : 0),ARR_MAR_Y,CHIP(1.5f),CHIP(1.0f),false,RED);
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
	graphics->spriteBegin();
	//���j���[��`��
	drawFrame(CHIP(8), CHIP_D(7.5f), CHIP(9), CHIP(6),3.0f,MENU_BACK,BLACK);

	middleF.print("�͂��߂���", CHIP(9), CHIP_D(8), CHIP(7),CHIP(1), MENU_TEXT, DT_CC);
	middleF.print("�Â�����", CHIP(9), CHIP_D(9), CHIP(7), CHIP(1), menuText(clearedStage!=0), DT_CC);
	middleF.print("�X�e�[�W�Z���N�g", CHIP(9), CHIP_D(10), CHIP(7), CHIP(1), menuText(clearedStage != 0), DT_CC);
	middleF.print("���ʐݒ�", CHIP(9), CHIP_D(11), CHIP(7), CHIP(1), MENU_TEXT, DT_CC);
	middleF.print("�I������", CHIP(9), CHIP_D(12), CHIP(7), CHIP(1), MENU_TEXT, DT_CC);
	//���`��
	if ((stateNumber >= 2) && (stateNumber <= 3)){
		drawArrowHorizontal(CHIP(8.25f), CHIP_D(8.0f + count), CHIP(1.5f), CHIP(1.0f),false,RED);
	}

	graphics->spriteEnd();
}

//�X�e�[�W�Z���N�g��`��
void AstralDebu::renderTitleSelect(){
	graphics->spriteBegin();
	//�I�����Ă���X�e�[�W��`��
	middleF.print(std::to_string(stage), CHIP(15), CHIP_D(10), CHIP(2), CHIP(1), WHITE, DT_CC);
	//�㉺�̖���`��
	drawTriangle(CHIP(15.75f), CHIP_D(10.0f), CHIP(16.0f), CHIP_D(9.75f),
		CHIP(16.25f), CHIP_D(10.0f), MENU_TEXT);
	drawTriangle(CHIP(15.75f), CHIP_D(11.0f), CHIP(16.0f), CHIP_D(11.25f),
		CHIP(16.25f), CHIP_D(11.0f), MENU_TEXT);

	graphics->spriteEnd();
}

//���ʑI����`��
void AstralDebu::renderTitleVolume(){
	graphics->spriteBegin();

	drawFrame(CHIP(7), CHIP_D(8.0f), CHIP(11), CHIP(4), 3.0f, MENU_BACK, BLACK);

	middleF.print("�a�f�l", CHIP(9), (int)CHIP_D(8.5f), CHIP(3), CHIP(1), MENU_TEXT, DT_CC);
	middleF.print(doubleToString(bgmVolume), CHIP(13), (int)CHIP_D(8.5f), CHIP(3), CHIP(1), WHITE, DT_CC);
	middleF.print("���ʉ�", CHIP(9), (int)CHIP_D(9.5f), CHIP(3), CHIP(1), MENU_TEXT, DT_CC);
	middleF.print(doubleToString(soundVolume), CHIP(13), (int)CHIP_D(9.5f), CHIP(3), CHIP(1), WHITE, DT_CC);
	middleF.print("�߂�", CHIP(9), (int)CHIP_D(10.5f), CHIP(7), CHIP(1), MENU_TEXT, DT_CC);

	drawArrowHorizontal(CHIP(7.25f), CHIP_D(8.5f+count), CHIP(1.5f), CHIP(1.0f), false, RED);

	if (stateNumber == 5){
		drawTriangleVertical(CHIP(13.0f), CHIP_D(8.75f), CHIP(0.5f), CHIP(0.5f), true, menuText(bgmVolume<1.0));
		drawTriangleVertical(CHIP(15.5f), CHIP_D(8.75f), CHIP(0.5f), CHIP(0.5f), false, menuText(bgmVolume>0.0));
	}
	if (stateNumber == 6){
		drawTriangleVertical(CHIP(13.0f), CHIP_D(9.75f), CHIP(0.5f), CHIP(0.5f), true, menuText(soundVolume<1.0));
		drawTriangleVertical(CHIP(15.5f), CHIP_D(9.75f), CHIP(0.5f), CHIP(0.5f), false, menuText(soundVolume>0.0));
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

	bigF.print(std::to_string(life),
		LIFE_MAR_X, LIFE_MAR_Y,
		LIFE_LEN_X, LIFE_LEN_Y,
		BLACK, DT_CC);

	bigF.print(std::to_string(life),
		LIFE_MAR_X - DATA_MAR / 2, LIFE_MAR_Y - DATA_MAR / 2,
		LIFE_LEN_X, LIFE_LEN_Y,
		WHITE, DT_CC);

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

	if (stage == 1){
		drawPanel("Z", 3.5f, 10.5f, 1.0f, HINT_PANEL);

		drawArrowHorizontal(3.0f, 11.5f, false, HINT_ARROW);

		drawPanel("GOAL", 2.0f, 1.5f, 3.0f, HINT_PANEL);

		drawArrowHorizontal(2.5f, 2.5f, true, HINT_ARROW);

		drawArrowVertical(20.5f, 5.0f, true, HINT_ARROW);
	}
	else if (stage == 2){
		drawPanel("C", 4, 11.0f, 1.0f, HINT_PANEL);

		drawArrowHorizontal(3.5f, 12.0f, false, HINT_ARROW);

		drawPanel("X", 13.0f, 8.0f, 1.0f, HINT_PANEL);

		drawArrowHorizontal(12.5f, 9.0f, false, HINT_ARROW);

		drawPanel("X", 20.0f, 11.25f, 1.0f, HINT_PANEL);

		drawArrowVertical(21.0f, 11.0f, false, HINT_ARROW);
	}
	else if (stage == 3){
		drawPanel("X+C", 4.5f, 10.0f, 2.0f, HINT_PANEL);

		drawArrowHorizontal(4.5f, 11.0f, false, HINT_ARROW);

		drawPanel("X", 14.0f, 0.5f, 1.0f, HINT_PANEL);

		drawArrowVertical(13.0f, 0.25f, false, HINT_ARROW);

		drawPanel(CHIP_BOMB, 10.0f, 10.5f, HINT_PANEL);
	}
	else if (stage == 10){
		drawPanel(CHIP_WOOD_BOX, 6.0f, 10.5f, HINT_PANEL);

		drawArrowHorizontal(5.5f, 11.5f, false, HINT_ARROW);

		drawPanel(CHIP_LEAD_BOX, 15.0f, 8.5f, HINT_PANEL);

		drawArrowHorizontal(14.5f, 9.5f, false, HINT_ARROW);
	}
	
	graphics->spriteEnd();
}

//�I�u�W�F�N�g��`��
void AstralDebu::renderObject(){
	graphics->spriteBegin();

	//�I�u�W�F�N�g��`��
	for (entityNS::RENDER_ORDER ro = entityNS::RO_BASE; ro < entityNS::RO_DEBU; ro = static_cast<entityNS::RENDER_ORDER>(ro + 1)){
		ALL_OBJ if ((canMove(object[i])&&(object[i]->isRenderOrder(ro)))) object[i]->draw();
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

//�J�[�\����`��
void AstralDebu::drawCursor(){
	chip.setX((float)CHIP(getCursorChipX()));
	chip.setY((float)CHIP_D(getCursorChipY()));

	chip.setCurrentFrame(CHIP_CURSOR);
	chip.draw();
}