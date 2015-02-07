#include "astraldebu.h"
using namespace astralNS;

//�\������`�b�v��ݒ�
int AstralDebu::setChipImage(int c){
	if (stage <= 10) return c - 1;
	else if (stage <= 20) return c + 7;
	else if (stage <= 30) return c + 15;
	else return 23;
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

//�f�o�b�O�p �����`��
void AstralDebu::drawEdge(){
	drawQuad((float)debu->getLeft(true), (float)debu->getTop(false),
		(float)debu->getRight(true) - debu->getLeft(true), (float)debu->getBottom(false) - debu->getTop(false), RED & graphicsNS::ALPHA50);
	drawQuad((float)debu->getLeft(false), (float)debu->getTop(true),
		(float)debu->getRight(false) - debu->getLeft(false), (float)debu->getBottom(true) - debu->getTop(true), BLUE & graphicsNS::ALPHA50);

	ALL_OBJ if (canTouch(object[i])){
		drawQuad((float)object[i]->getLeft(true), (float)object[i]->getTop(false),
			(float)object[i]->getRight(true) - object[i]->getLeft(true), (float)object[i]->getBottom(false) - object[i]->getTop(false), RED & graphicsNS::ALPHA50);
		drawQuad((float)object[i]->getLeft(false), (float)object[i]->getTop(true),
			(float)object[i]->getRight(false) - object[i]->getLeft(false), (float)object[i]->getBottom(true) - object[i]->getTop(true), BLUE & graphicsNS::ALPHA50);
	}
}

//�q���g�p�̖���`�� ��
void AstralDebu::drawArrowHorizontal(float cx, float cy, bool d, ARGB c){
	if (d){
		drawQuad((cx + 0.5f)*CHIP_SIZE, (cy + 0.25f)*CHIP_SIZE + DATA_LEN, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE), BLACK);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (cx + 0.5f)*CHIP_SIZE, (cy + 1)*CHIP_SIZE + DATA_LEN,
			cx*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, BLACK);
		drawQuad((cx + 0.5f)*CHIP_SIZE - 2, (cy + 0.25f)*CHIP_SIZE + DATA_LEN + 2, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE) - 4, c);
		drawTriangle((cx + 0.5f)*CHIP_SIZE - 2, cy*CHIP_SIZE + DATA_LEN + 4, (cx + 0.5f)*CHIP_SIZE - 2, (cy + 1)*CHIP_SIZE + DATA_LEN - 4,
			cx*CHIP_SIZE + 3, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, c);
	}
	else {
		drawQuad(cx*CHIP_SIZE, (cy + 0.25f)*CHIP_SIZE + DATA_LEN, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE), BLACK);
		drawTriangle((cx + 1.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (cx + 2)*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN,
			(cx + 1.5f)*CHIP_SIZE, (cy + 1)*CHIP_SIZE + DATA_LEN, BLACK);
		drawQuad(cx*CHIP_SIZE + 2, (cy + 0.25f)*CHIP_SIZE + DATA_LEN + 2, (1.5f*CHIP_SIZE), (0.5f*CHIP_SIZE) - 4, c);
		drawTriangle((cx + 1.5f)*CHIP_SIZE + 2, cy*CHIP_SIZE + DATA_LEN + 4, (cx + 2)*CHIP_SIZE - 3, (cy + 0.5f)*CHIP_SIZE + DATA_LEN,
			(cx + 1.5f)*CHIP_SIZE + 2, (cy + 1)*CHIP_SIZE + DATA_LEN - 4, c);
	}
}

//�q���g�p�̖���`�� �c
void AstralDebu::drawArrowVertical(float cx, float cy, bool d, ARGB c){
	if (d){
		drawQuad((cx + 0.25f)*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, (0.5f*CHIP_SIZE), (1.5f*CHIP_SIZE), BLACK);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (cx + 1)*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN,
			cx*CHIP_SIZE, (cy + 0.5f)*CHIP_SIZE + DATA_LEN, BLACK);
		drawQuad((cx + 0.25f)*CHIP_SIZE + 2, (cy + 0.5f)*CHIP_SIZE + DATA_LEN - 2, (0.5f*CHIP_SIZE) - 4, (1.5f*CHIP_SIZE), c);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN + 3, (cx + 1)*CHIP_SIZE - 4, (cy + 0.5f)*CHIP_SIZE + DATA_LEN - 2,
			cx*CHIP_SIZE + 4, (cy + 0.5f)*CHIP_SIZE + DATA_LEN - 2, c);
	}
	else {
		drawQuad((cx + 0.25f)*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, (0.5f*CHIP_SIZE), (1.5f*CHIP_SIZE), BLACK);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, (cy + 2)*CHIP_SIZE + DATA_LEN, cx*CHIP_SIZE, (cy + 1.5f)*CHIP_SIZE + DATA_LEN,
			(cx + 1)*CHIP_SIZE, (cy + 1.5f)*CHIP_SIZE + DATA_LEN, BLACK);
		drawQuad((cx + 0.25f)*CHIP_SIZE + 2, cy*CHIP_SIZE + DATA_LEN + 2, (0.5f*CHIP_SIZE) - 4, (1.5f*CHIP_SIZE), c);
		drawTriangle((cx + 0.5f)*CHIP_SIZE, (cy + 2)*CHIP_SIZE + DATA_LEN - 3, cx*CHIP_SIZE + 4, (cy + 1.5f)*CHIP_SIZE + DATA_LEN + 2,
			(cx + 1)*CHIP_SIZE - 4, (cy + 1.5f)*CHIP_SIZE + DATA_LEN + 2, c);
	}
}

//�q���g�p�̃p�l����`��
void AstralDebu::drawPanel(std::string str, float cx, float cy, float len, ARGB c) {
	drawQuad(cx*CHIP_SIZE, cy*CHIP_SIZE + DATA_LEN, CHIP_SIZE*len, (float)CHIP_SIZE, BLACK);
	drawQuad(cx*CHIP_SIZE + 2, cy*CHIP_SIZE + DATA_LEN + 2, CHIP_SIZE*len - 4, (float)CHIP_SIZE - 4, c);
	middleF.print(str, (int)(cx * CHIP_SIZE), (int)(cy * CHIP_SIZE + DATA_LEN), (int)(len * CHIP_SIZE), CHIP_SIZE, BLACK, DT_CC);
}

//�摜���q���g�p�̃p�l����`��
void AstralDebu::drawPanel(int img, float cx, float cy, ARGB c) {
	drawQuad(cx*CHIP_SIZE - 3, cy*CHIP_SIZE + DATA_LEN - 3, (float)CHIP_SIZE + 6, (float)CHIP_SIZE + 6, BLACK);
	drawQuad(cx*CHIP_SIZE - 1, cy*CHIP_SIZE + DATA_LEN - 1, (float)CHIP_SIZE + 2, (float)CHIP_SIZE + 2, c);
	chip.setX(cx*CHIP_SIZE);
	chip.setY(cy*CHIP_SIZE + DATA_LEN);
	chip.setCurrentFrame(img);
	chip.draw();
}
