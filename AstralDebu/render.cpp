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
	graphics->spriteBegin();
	drawQuad(MENU_MAR_X - MENU_FRAME, MENU_MAR_Y - MENU_FRAME, MENU_LEN_X + MENU_FRAME * 2, MENU_FRAME, MENU_BACK);
	drawQuad(MENU_MAR_X - MENU_FRAME, MENU_MAR_Y + MENU_LEN_Y, MENU_LEN_X + MENU_FRAME * 2, MENU_FRAME, MENU_BACK);
	drawQuad(MENU_MAR_X - MENU_FRAME, MENU_MAR_Y, MENU_FRAME, MENU_LEN_Y, MENU_BACK);
	drawQuad(MENU_MAR_X + MENU_LEN_X, MENU_MAR_Y, MENU_FRAME, MENU_LEN_Y, MENU_BACK);
	drawQuad(MENU_MAR_X, MENU_MAR_Y, MENU_LEN_X, MENU_LEN_Y, MENU_BACK&graphicsNS::ALPHA90);

	numberF.print("�y�F�W�����v", MENU_TEXT_MAR_X, MENU_TEXT_MAR_Y, MENU_TEXT_LEN_X, MENU_TEXT_SEP_Y, BLACK, DT_LC);
	numberF.print("�w�F�A�C�e���������グ��", MENU_TEXT_MAR_X, MENU_TEXT_MAR_Y + MENU_TEXT_SEP_Y, MENU_TEXT_LEN_X, MENU_TEXT_SEP_Y, BLACK, DT_LC);
	numberF.print("�@�@�����グ���͒u��", MENU_TEXT_MAR_X, MENU_TEXT_MAR_Y + MENU_TEXT_SEP_Y * 2, MENU_TEXT_LEN_X, MENU_TEXT_SEP_Y, BLACK, DT_LC);
	numberF.print("�b�F�A�C�e��������", MENU_TEXT_MAR_X, MENU_TEXT_MAR_Y + MENU_TEXT_SEP_Y * 3, MENU_TEXT_LEN_X, MENU_TEXT_SEP_Y, BLACK, DT_LC);
	numberF.print("�@�F�����グ���͓�����", MENU_TEXT_MAR_X, MENU_TEXT_MAR_Y + MENU_TEXT_SEP_Y * 4, MENU_TEXT_LEN_X, MENU_TEXT_SEP_Y, BLACK, DT_LC);
	numberF.print("�p�F��蒼��", MENU_TEXT_MAR_X, MENU_TEXT_MAR_Y + MENU_TEXT_SEP_Y * 5, MENU_TEXT_LEN_X, MENU_TEXT_SEP_Y, BLACK, DT_LC);
	numberF.print("�Q�[���𑱂���", MENU_TEXT_MAR_X, MENU_TEXT_MAR_Y + MENU_TEXT_SEP_Y * 7, MENU_TEXT_SEP_X, MENU_TEXT_SEP_Y, BLACK, DT_CC);
	numberF.print("�^�C�g���ɖ߂�", MENU_TEXT_MAR_X + MENU_TEXT_SEP_X, MENU_TEXT_MAR_Y + MENU_TEXT_SEP_Y * 7, MENU_TEXT_SEP_X, MENU_TEXT_SEP_Y, BLACK, DT_CC);
	
	//���`��
	drawQuad(MENU_ARROW_QUA_MAR_X + (count ? MENU_TEXT_SEP_X : 0), MENU_ARROW_QUA_MAR_Y, MENU_ARROW_QUA_LEN_X, MENU_ARROW_QUA_LEN_Y, BLACK);
	drawTriangle(MENU_ARROW_TRI_X_L + (count ? MENU_TEXT_SEP_X : 0), MENU_ARROW_TRI_Y_T, MENU_ARROW_TRI_X_R + (count ? MENU_TEXT_SEP_X : 0), MENU_ARROW_TRI_Y_M,
		MENU_ARROW_TRI_X_L + (count ? MENU_TEXT_SEP_X : 0), MENU_ARROW_TRI_Y_B, BLACK);
	drawQuad(MENU_ARROW_QUA_MAR_X + (count ? MENU_TEXT_SEP_X : 0)+2, MENU_ARROW_QUA_MAR_Y+2, MENU_ARROW_QUA_LEN_X, MENU_ARROW_QUA_LEN_Y-4, RED);
	drawTriangle(MENU_ARROW_TRI_X_L + (count ? MENU_TEXT_SEP_X : 0)+2, MENU_ARROW_TRI_Y_T+4, MENU_ARROW_TRI_X_R + (count ? MENU_TEXT_SEP_X : 0)-3, MENU_ARROW_TRI_Y_M,
		MENU_ARROW_TRI_X_L + (count ? MENU_TEXT_SEP_X : 0)+2, MENU_ARROW_TRI_Y_B-4, RED);

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
	drawQuad(8 * CHIP_SIZE, 7.5f*CHIP_SIZE + DATA_LEN, 9 * CHIP_SIZE, 6 * CHIP_SIZE, BLACK);
	drawQuad(8 * CHIP_SIZE + 3, 7.5f*CHIP_SIZE + DATA_LEN + 3, 9 * CHIP_SIZE - 6, 6 * CHIP_SIZE - 6, MENU_BACK);
	middleF.print("�͂��߂���", (int)(9 * CHIP_SIZE), (int)(8 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, MENU_TEXT, DT_CC);
	middleF.print("�Â�����", (int)(9 * CHIP_SIZE), (int)(9 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, (clearedStage == 0) ? MENU_HIDE : MENU_TEXT, DT_CC);
	middleF.print("�X�e�[�W�Z���N�g", (int)(9 * CHIP_SIZE), (int)(10 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, (clearedStage == 0) ? MENU_HIDE : MENU_TEXT, DT_CC);
	middleF.print("���ʐݒ�", (int)(9 * CHIP_SIZE), (int)(11 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, MENU_TEXT, DT_CC);
	middleF.print("�I������", (int)(9 * CHIP_SIZE), (int)(12 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, MENU_TEXT, DT_CC);
	//���`��
	if ((stateNumber >= 2) && (stateNumber <= 3)){
		drawQuad(8.25f*CHIP_SIZE, (8.25f + count)*CHIP_SIZE + DATA_LEN, (float)CHIP_SIZE, (0.5f*CHIP_SIZE), BLACK);
		drawTriangle((9.25f)*CHIP_SIZE, (8.0f + count)*CHIP_SIZE + DATA_LEN, (9.75f)*CHIP_SIZE, (8.5f + count)*CHIP_SIZE + DATA_LEN,
			(9.25f)*CHIP_SIZE, (9.0f + count)*CHIP_SIZE + DATA_LEN, BLACK);
		drawQuad(8.25f*CHIP_SIZE + 2, (8.25f + count)*CHIP_SIZE + DATA_LEN + 2, (float)CHIP_SIZE, (0.5f*CHIP_SIZE) - 4, RED);
		drawTriangle((9.25f)*CHIP_SIZE + 2, (8.0f + count)*CHIP_SIZE + DATA_LEN + 4, (9.75f)*CHIP_SIZE - 3, (8.5f + count)*CHIP_SIZE + DATA_LEN,
			(9.25f)*CHIP_SIZE + 2, (9.0f + count)*CHIP_SIZE + DATA_LEN - 4, RED);
	}

	graphics->spriteEnd();
}

//�X�e�[�W�Z���N�g��`��
void AstralDebu::renderTitleSelect(){
	graphics->spriteBegin();
	//�I�����Ă���X�e�[�W��`��
	middleF.print(std::to_string(stage), (int)(15.0f * CHIP_SIZE), (int)(10 * CHIP_SIZE + DATA_LEN), (int)(2.0f * CHIP_SIZE), CHIP_SIZE, WHITE, DT_CC);
	//�㉺�̖���`��
	drawTriangle((15.75f)*CHIP_SIZE, (10.0f)*CHIP_SIZE + DATA_LEN, (16.0f)*CHIP_SIZE, (9.75f)*CHIP_SIZE + DATA_LEN,
		(16.25f)*CHIP_SIZE, (10.0f)*CHIP_SIZE + DATA_LEN, MENU_TEXT);
	drawTriangle((15.75f)*CHIP_SIZE, (11.0f)*CHIP_SIZE + DATA_LEN, (16.0f)*CHIP_SIZE, (11.25f)*CHIP_SIZE + DATA_LEN,
		(16.25f)*CHIP_SIZE, (11.0f)*CHIP_SIZE + DATA_LEN, MENU_TEXT);

	graphics->spriteEnd();
}

//���ʑI����`��
void AstralDebu::renderTitleVolume(){
	graphics->spriteBegin();

	drawQuad(8.25f*CHIP_SIZE, 11.25f*CHIP_SIZE + DATA_LEN, (float)CHIP_SIZE, (0.5f*CHIP_SIZE), BLACK);
	drawTriangle((9.25f)*CHIP_SIZE, 11.0f*CHIP_SIZE + DATA_LEN, (9.75f)*CHIP_SIZE, 11.5f*CHIP_SIZE + DATA_LEN,
		(9.25f)*CHIP_SIZE, 12.0f*CHIP_SIZE + DATA_LEN, BLACK);
	drawQuad(8.25f*CHIP_SIZE + 2, 11.25f*CHIP_SIZE + DATA_LEN + 2, (float)CHIP_SIZE, (0.5f*CHIP_SIZE) - 4, RED);
	drawTriangle((9.25f)*CHIP_SIZE + 2, 11.0f*CHIP_SIZE + DATA_LEN + 4, (9.75f)*CHIP_SIZE - 3, 11.5f*CHIP_SIZE + DATA_LEN,
		(9.25f)*CHIP_SIZE + 2, 12.0f*CHIP_SIZE + DATA_LEN - 4, RED);

	drawQuad(7 * CHIP_SIZE, 8.0f*CHIP_SIZE + DATA_LEN, 11 * CHIP_SIZE, 4 * CHIP_SIZE, BLACK);
	drawQuad(7 * CHIP_SIZE + 3, 8.0f*CHIP_SIZE + DATA_LEN + 3, 11 * CHIP_SIZE - 6, 4 * CHIP_SIZE - 6, MENU_BACK);
	middleF.print("�a�f�l", (int)(9 * CHIP_SIZE), (int)(8.5f * CHIP_SIZE + DATA_LEN), (int)(3 * CHIP_SIZE), CHIP_SIZE, MENU_TEXT, DT_CC);
	middleF.print(floatToString(bgmVolume), 13 * CHIP_SIZE, (int)(8.5f * CHIP_SIZE + DATA_LEN), (int)(3 * CHIP_SIZE), CHIP_SIZE, WHITE, DT_CC);
	middleF.print("���ʉ�", (int)(9 * CHIP_SIZE), (int)(9.5f * CHIP_SIZE + DATA_LEN), (int)(3 * CHIP_SIZE), CHIP_SIZE, MENU_TEXT, DT_CC);
	middleF.print(floatToString(soundVolume), 13 * CHIP_SIZE, (int)(9.5f * CHIP_SIZE + DATA_LEN), (int)(3 * CHIP_SIZE), CHIP_SIZE, WHITE, DT_CC);
	middleF.print("�߂�", (int)(9 * CHIP_SIZE), (int)(10.5f * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, MENU_TEXT, DT_CC);

	drawQuad(7.25f*CHIP_SIZE, (8.75f + count)*CHIP_SIZE + DATA_LEN, (float)CHIP_SIZE, (0.5f*CHIP_SIZE), BLACK);
	drawTriangle((8.25f)*CHIP_SIZE, (8.5f + count)*CHIP_SIZE + DATA_LEN, (8.75f)*CHIP_SIZE, (9.0f + count)*CHIP_SIZE + DATA_LEN,
		(8.25f)*CHIP_SIZE, (9.5f + count)*CHIP_SIZE + DATA_LEN, BLACK);
	drawQuad(7.25f*CHIP_SIZE + 2, (8.75f + count)*CHIP_SIZE + DATA_LEN + 2, (float)CHIP_SIZE, (0.5f*CHIP_SIZE) - 4, RED);
	drawTriangle((8.25f)*CHIP_SIZE + 2, (8.5f + count)*CHIP_SIZE + DATA_LEN + 4, (8.75f)*CHIP_SIZE - 3, (9.0f + count)*CHIP_SIZE + DATA_LEN,
		(8.25f)*CHIP_SIZE + 2, (9.5f + count)*CHIP_SIZE + DATA_LEN - 4, RED);

	if (stateNumber == 5){
		drawTriangle((13.0f)*CHIP_SIZE, (9.25f)*CHIP_SIZE + DATA_LEN, (13.25f)*CHIP_SIZE, (8.75f)*CHIP_SIZE + DATA_LEN,
			(13.5f)*CHIP_SIZE, (9.25f)*CHIP_SIZE + DATA_LEN, (bgmVolume<1.0f ? MENU_TEXT : MENU_HIDE));
		drawTriangle((15.5f)*CHIP_SIZE, (8.75f)*CHIP_SIZE + DATA_LEN, (15.75f)*CHIP_SIZE, (9.25f)*CHIP_SIZE + DATA_LEN,
			(16.0f)*CHIP_SIZE, (8.75f)*CHIP_SIZE + DATA_LEN, (bgmVolume>0.0f ? MENU_TEXT : MENU_HIDE));
	}
	if (stateNumber == 6){
		drawTriangle((13.0f)*CHIP_SIZE, (10.25f)*CHIP_SIZE + DATA_LEN, (13.25f)*CHIP_SIZE, (9.75f)*CHIP_SIZE + DATA_LEN,
			(13.5f)*CHIP_SIZE, (10.25f)*CHIP_SIZE + DATA_LEN, (soundVolume<1.0f ? MENU_TEXT : MENU_HIDE));
		drawTriangle((15.5f)*CHIP_SIZE, (9.75f)*CHIP_SIZE + DATA_LEN, (15.75f)*CHIP_SIZE, (10.25f)*CHIP_SIZE + DATA_LEN,
			(16.0f)*CHIP_SIZE, (9.75f)*CHIP_SIZE + DATA_LEN, (soundVolume>0.0f ? MENU_TEXT : MENU_HIDE));
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
	drawQuad((float)LIFE_MAR_X - DATA_MAR, (float)LIFE_MAR_Y - DATA_MAR,
		(float)LIFE_LEN_X + DATA_MAR * 2, (float)LIFE_LEN_Y + DATA_MAR * 2, WHITE);
	drawQuad((float)LIFE_MAR_X, (float)LIFE_MAR_Y,
		(float)LIFE_LEN_X, (float)LIFE_LEN_Y, BLACK);

	//�̗�
	drawQuad((float)LIFE_MAR_X, (float)LIFE_MAR_Y,
		(float)life*DATA_MAR, (float)LIFE_LEN_Y, LIFE_GAUGE);

	//�̗͕ω�
	if (vitalLife > life)
		drawQuad((float)LIFE_MAR_X + life*DATA_MAR, (float)LIFE_MAR_Y,
		(float)(vitalLife - life)*DATA_MAR, (float)LIFE_LEN_Y, LIFE_VITAL_MINUS);
	else if (vitalLife < life)
		drawQuad((float)LIFE_MAR_X + vitalLife*DATA_MAR, (float)LIFE_MAR_Y,
		(float)(life - vitalLife)*DATA_MAR, (float)LIFE_LEN_Y, LIFE_VITAL_PLUS);

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
		chip.setY((float)(j*CHIP_SIZE) + DATA_LEN);
		for (int i = 0; i < MAP_COL; i++){
			if (map[i][j]){
				chip.setX((float)i*CHIP_SIZE);
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
	chip.setX((float)(getCursorChipX() * CHIP_SIZE));
	chip.setY((float)(getCursorChipY() * CHIP_SIZE) + DATA_LEN);

	chip.setCurrentFrame(CHIP_CURSOR);
	chip.draw();
}