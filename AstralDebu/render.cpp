#include "astraldebu.h"
using namespace astralNS;

//�^�C�g����ʂ̕`��
void AstralDebu::renderTitle(){
	graphics->spriteBegin();
	title.draw();

	bigF.print("�A�X�g�����E�f�u",
		2, 122, WINDOW_W, WINDOW_H,
		graphicsNS::WHITE, DT_CT);

	bigF.print("�A�X�g�����E�f�u",
		0, 120, WINDOW_W, WINDOW_H,
		graphicsNS::RED, DT_CT);

	middleF.print("�|�|���̎��f�u�͐��ɂȂ�|�|",
		0, 260, WINDOW_W, WINDOW_H,
		graphicsNS::WHITE, DT_CT);

	if (count < 20){
		bigF.print("�u�y�v�L�[�Ł@�X�^�[�g",
			0, 400, WINDOW_W, WINDOW_H,
			graphicsNS::RED, DT_CT);
	}

	graphics->spriteEnd();
}

//�X�e�[�W�\����ʂ̕`��
void AstralDebu::renderStage(){
	graphics->spriteBegin();

	drawQuad(0, 0, (float)WINDOW_W, (float)WINDOW_H, graphicsNS::BLACK);

	bigF.print("STAGE" + std::to_string(stage),
		0, (WINDOW_H - 60) / 2,
		WINDOW_W, WINDOW_H,
		graphicsNS::WHITE, DT_CT);

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

	if (cheat2) drawEdge();
}

//�N���A��ʂ̕`��
void AstralDebu::renderClear(){
	graphics->spriteBegin();

	graphics->spriteEnd();
}

//�\������`�b�v��ݒ�
int AstralDebu::chipFormat(int c){
	if (stage <= 10) return c - 1;
	else if (stage <= 20) return c + 4;
	else if (stage <= 30) return c + 9;
	else return 14;
}

//�w�i�Ə�̂�`��
void AstralDebu::renderBack(){
	graphics->spriteBegin();

	ARGB back;
	if ((stage <= 10) || (stage == 20)) back = graphicsNS::S1;
	else if (stage <= 19) back = graphicsNS::S2;
	else back = graphicsNS::S3;

	//�w�i�h��Ԃ�
	drawQuad(0, 0, (float)WINDOW_W, (float)WINDOW_H, back);

	//�㕔�f�[�^�̈�h��Ԃ�
	drawQuad(0, 0, (float)WINDOW_W, (float)DATA_LEN, graphicsNS::BLACK);

	//�̗̓��[�^��`��
	//�g
	drawQuad((float)LIFE_MAR_X - DATA_MAR, (float)LIFE_MAR_Y - DATA_MAR,
		(float)LIFE_LEN_X + DATA_MAR * 2, (float)LIFE_LEN_Y + DATA_MAR * 2, graphicsNS::WHITE);
	drawQuad((float)LIFE_MAR_X, (float)LIFE_MAR_Y,
		(float)LIFE_LEN_X, (float)LIFE_LEN_Y, graphicsNS::BLACK);

	//�̗�
	drawQuad((float)LIFE_MAR_X, (float)LIFE_MAR_Y,
		(float)life*DATA_MAR, (float)LIFE_LEN_Y, graphicsNS::LIFE);

	//�̗͕ω�
	if (life_v > life)
		drawQuad((float)LIFE_MAR_X + life*DATA_MAR, (float)LIFE_MAR_Y,
		(float)(life_v - life)*DATA_MAR, (float)LIFE_LEN_Y, graphicsNS::RED);
	else if (life_v < life)
		drawQuad((float)LIFE_MAR_X + life_v*DATA_MAR, (float)LIFE_MAR_Y,
		(float)(life - life_v)*DATA_MAR, (float)LIFE_LEN_Y, graphicsNS::LIME);

	//�̗͕\��
	middleF.print("LIFE",
		WORD_MAR_X, WORD_MAR_Y,
		WORD_LEN_X, WORD_LEN_Y,
		graphicsNS::WHITE, DT_CC);

	bigF.print(std::to_string(life),
		LIFE_MAR_X, LIFE_MAR_Y,
		LIFE_LEN_X, LIFE_LEN_Y,
		graphicsNS::BLACK, DT_CC);

	bigF.print(std::to_string(life),
		LIFE_MAR_X - DATA_MAR / 2, LIFE_MAR_Y - DATA_MAR / 2,
		LIFE_LEN_X, LIFE_LEN_Y,
		graphicsNS::WHITE, DT_CC);

	//�X�e�[�W�\��

	numberF.print(std::to_string(debu->getResponse()),//"STAGE" + std::to_string(stage),
		STG_MAR_X, STG_MAR_Y,
		STG_LEN_X, STG_LEN_Y,
		graphicsNS::WHITE, DT_CC);

	graphics->spriteEnd();
}

//�}�b�v�`�b�v��`��
void AstralDebu::renderChip(){
	graphics->spriteBegin();

	//�}�b�v�`��
	for (int j = 0; j < MAP_ROW; j++){
		chip.setY((float)(j*CHIP_SIZE) + DATA_LEN);
		for (int i = 0; i < MAP_COL; i++){
			if (map[i][j] != 0){
				chip.setX((float)i*CHIP_SIZE);
				if (map[i][j] < 10) chip.setCurrentFrame(chipFormat(map[i][j]));
				else chip.setCurrentFrame(map[i][j]);
				chip.draw();
			}
		}
	}

	graphics->spriteEnd();
}

//�q���g�Ƃ��`��
void AstralDebu::renderHint(){
	graphics->spriteBegin();

	if (stage == 1){
		drawPanel("Z", 3.5f, 10.5f, 1.0f, graphicsNS::RED);

		drawArrowH(3.0f, 11.5f, false, graphicsNS::RED);

		drawPanel("GOAL", 2.0f, 1.5f, 3.0f, graphicsNS::RED);

		drawArrowH(2.5f, 2.5f, true, graphicsNS::RED);

		drawArrowV(20.5f, 5.0f, true, graphicsNS::RED);
	}
	else if (stage == 2){
		drawPanel("C", 4, 11.0f, 1.0f, graphicsNS::TEAL);

		drawArrowH(3.5f, 12.0f, false, graphicsNS::YELLOW);

		drawPanel("X", 13.0f, 8.0f, 1.0f, graphicsNS::TEAL);

		drawArrowH(12.5f, 9.0f, false, graphicsNS::YELLOW);

		drawPanel("X", 20.0f, 11.25f, 1.0f, graphicsNS::TEAL);

		drawArrowV(21.0f, 11.0f, false, graphicsNS::YELLOW);
	}
	else if (stage == 3){
		drawPanel("X+C", 4.5f, 10.0f, 2.0f, graphicsNS::TEAL);

		drawArrowH(4.5f, 11.0f, false, graphicsNS::YELLOW);

		drawPanel("X", 14.0f, 0.5f, 1.0f, graphicsNS::TEAL);

		drawArrowV(13.0f, 0.25f, false, graphicsNS::YELLOW);
	}

	graphics->spriteEnd();
}

//�I�u�W�F�N�g��`��
void AstralDebu::renderObject(){
	graphics->spriteBegin();

	//�I�u�W�F�N�g��`��@�������͌��
	ALL_OBJ if ((canMove(object[i])) && (i != obj_hold))
		object[i]->draw();

	//��������`��
	if (obj_hold >= 0){
		if (debu->getState() != entityNS::HOLD_HAMMER) object[obj_hold]->draw();
		else if (object[obj_hold]->getVelX() > 0) object[obj_hold]->draw();
	}

	//�f�u��`��
	debu->draw();

	//�������̃n���}�[���̓f�u�̑O�ɕ`��
	if ((debu->getState() == entityNS::HOLD_HAMMER) && (object[obj_hold]->getVelX() <= 0))
		object[obj_hold]->draw();

	//�J�[�\����`��
	drawCursor();

	graphics->spriteEnd();
}

//�J�[�\����`��
void AstralDebu::drawCursor(){
	chip.setX((float)(cursorX() * CHIP_SIZE));
	chip.setY((float)(cursorY() * CHIP_SIZE) + DATA_LEN);

	chip.setCurrentFrame(CHIP_CURSOR);
	chip.draw();
}

//�f�o�b�O�p �����`��
void AstralDebu::drawEdge(){
	drawQuad((float)debu->getLeft(true), (float)debu->getTop(false),
		(float)debu->getRight(true) - debu->getLeft(true), (float)debu->getBottom(false) - debu->getTop(false), graphicsNS::RED & graphicsNS::ALPHA50);
	drawQuad((float)debu->getLeft(false), (float)debu->getTop(true),
		(float)debu->getRight(false) - debu->getLeft(false), (float)debu->getBottom(true) - debu->getTop(true), graphicsNS::BLUE & graphicsNS::ALPHA50);

	ALL_OBJ if (canTouch(object[i])){
		drawQuad((float)object[i]->getLeft(true), (float)object[i]->getTop(false),
			(float)object[i]->getRight(true) - object[i]->getLeft(true), (float)object[i]->getBottom(false) - object[i]->getTop(false), graphicsNS::RED & graphicsNS::ALPHA50);
		drawQuad((float)object[i]->getLeft(false), (float)object[i]->getTop(true),
			(float)object[i]->getRight(false) - object[i]->getLeft(false), (float)object[i]->getBottom(true) - object[i]->getTop(true), graphicsNS::BLUE & graphicsNS::ALPHA50);
	}
}

//�q���g�p�̖���`�� ��
void AstralDebu::drawArrowH(float cx, float cy, bool d, ARGB c){
	if (d){
		drawQuad((cx + 0.5f)*CHIP_SIZE, (cy + 0.25f)*CHIP_SIZE + DATA_LEN, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE), graphicsNS::BLACK);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (cx + 0.5f)*CHIP_SIZE, (cy + 1)*CHIP_SIZE + DATA_LEN,
			cx*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, graphicsNS::BLACK);
		drawQuad((cx + 0.5f)*CHIP_SIZE - 2, (cy + 0.25f)*CHIP_SIZE + DATA_LEN + 2, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE) - 4, c);
		drawTriangle((cx + 0.5f)*CHIP_SIZE - 2, cy*CHIP_SIZE + DATA_LEN + 4, (cx + 0.5f)*CHIP_SIZE - 2, (cy + 1)*CHIP_SIZE + DATA_LEN - 4,
			cx*CHIP_SIZE + 3, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, c);
	}
	else {
		drawQuad(cx*CHIP_SIZE, (cy + 0.25f)*CHIP_SIZE + DATA_LEN, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE), graphicsNS::BLACK);
		drawTriangle((cx + 1.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (cx + 2)*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN,
			(cx + 1.5f)*CHIP_SIZE, (cy + 1)*CHIP_SIZE + DATA_LEN, graphicsNS::BLACK);
		drawQuad(cx*CHIP_SIZE + 2, (cy + 0.25f)*CHIP_SIZE + DATA_LEN + 2, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE) - 4, c);
		drawTriangle((cx + 1.5f)*CHIP_SIZE + 2, cy*CHIP_SIZE + DATA_LEN + 4, (cx + 2)*CHIP_SIZE - 3, (cy + 0.5f)*CHIP_SIZE + DATA_LEN,
			(cx + 1.5f)*CHIP_SIZE + 2, (cy + 1)*CHIP_SIZE + DATA_LEN - 4, c);
	}
}

//�q���g�p�̖���`�� �c
void AstralDebu::drawArrowV(float cx, float cy, bool d, ARGB c){
	if (d){
		drawQuad((cx + 0.25f)*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, (0.5f*CHIP_SIZE), (1.5f*CHIP_SIZE), graphicsNS::BLACK);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (cx + 1)*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN,
			cx*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, graphicsNS::BLACK);
		drawQuad((cx + 0.25f)*CHIP_SIZE + 2, (cy + 0.5f)*CHIP_SIZE + DATA_LEN - 2, (0.5f*CHIP_SIZE) - 4, (1.5f*CHIP_SIZE), c);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN + 3, (cx + 1)*CHIP_SIZE - 4, (cy + 0.5f)*CHIP_SIZE + DATA_LEN - 2,
			cx*CHIP_SIZE + 4, (cy + 0.5f)*CHIP_SIZE + DATA_LEN - 2, c);
	}
	else {
		drawQuad((cx + 0.25f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (0.5f*CHIP_SIZE), (1.5f*CHIP_SIZE), graphicsNS::BLACK);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, (cy + 2)*CHIP_SIZE + DATA_LEN, cx*CHIP_SIZE, (cy + 1.5f)*CHIP_SIZE + DATA_LEN,
			(cx + 1)*CHIP_SIZE, (cy + 1.5f)*CHIP_SIZE + DATA_LEN, graphicsNS::BLACK);
		drawQuad((cx + 0.25f)*CHIP_SIZE + 2, cy*CHIP_SIZE + DATA_LEN + 2, (0.5f*CHIP_SIZE) - 4, (1.5f*CHIP_SIZE), c);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, (cy + 2)*CHIP_SIZE + DATA_LEN - 3, cx*CHIP_SIZE + 4, (cy + 1.5f)*CHIP_SIZE + DATA_LEN + 2,
			(cx + 1)*CHIP_SIZE - 4, (cy + 1.5f)*CHIP_SIZE + DATA_LEN + 2, c);
	}
}

//�q���g�p�̃p�l����`��
void AstralDebu::drawPanel(std::string str, float cx, float cy, float len, ARGB c) {
	drawQuad(cx*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, CHIP_SIZE*len, (float)CHIP_SIZE, graphicsNS::BLACK);
	drawQuad(cx*CHIP_SIZE + 2, cy*CHIP_SIZE + DATA_LEN + 2, CHIP_SIZE*len - 4, (float)CHIP_SIZE - 4, c);
	middleF.print(str, (int)(cx * CHIP_SIZE), (int)(cy * CHIP_SIZE + DATA_LEN), (int)(len * CHIP_SIZE), CHIP_SIZE, graphicsNS::BLACK, DT_CC);
}
