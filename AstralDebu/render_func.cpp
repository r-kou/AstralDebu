#include "astraldebu.h"
using namespace astralNS;

//•\¦‚·‚éƒ`ƒbƒv‚ğİ’è
int AstralDebu::setChipImage(int c){
	if (stage <= 10) return c - 1;
	else if (stage <= 20) return c + 7;
	else if (stage <= 30) return c + 15;
	else return c + 23;
}

//lŠp‚Ì•`‰æ
void AstralDebu::setVertex(float l, float t, float r, float b, ARGB c){
	//¶ã
	vtx[0].x = l;
	vtx[0].y = t;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//‰Eã
	vtx[1].x = l + r;
	vtx[1].y = t;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//‰E‰º
	vtx[2].x = l + r;
	vtx[2].y = t + b;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//¶‰º
	vtx[3].x = l;
	vtx[3].y = t + b;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//lŠp‚ğ•`‰æ
void AstralDebu::drawQuad(float l, float t, float r, float b, ARGB c){
	//’ZŒa‚ğİ’è
	setVertex(l, t, r, b, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphics‚É”C‚¹‚é
	graphics->drawQuad(vertex);
}

//OŠp‚Ìvertex‚ğ•`‰æ
void AstralDebu::setVertexTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//ã
	vtx[0].x = x1;
	vtx[0].y = y1;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//‰E
	vtx[1].x = x2;
	vtx[1].y = y2;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//‰º
	vtx[2].x = x3;
	vtx[2].y = y3;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//ã‚É‹A‚Á‚Ä‚­‚é
	vtx[3].x = x1;
	vtx[3].y = y1;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//OŠp‚ğ•`‰æ
void AstralDebu::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//’ZŒa‚ğİ’è
	setVertexTriangle(x1, y1, x2, y2, x3, y3, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphcs‚É”C‚¹‚é
	graphics->drawQuad(vertex);
}

//OŠp‚ğ•`‰æ
void AstralDebu::drawTriangleHorizontal(float l, float t, float r, float b, bool d, ARGB c){
	//’ZŒa‚ğİ’è
	if (d) drawTriangle(l, t + (b / 2.0f), l + r, t, l+r, t + b, c);
	else drawTriangle(l+r, t + (b / 2.0f), l, t+b, l, t, c);
}

//OŠp‚ğ•`‰æ
void AstralDebu::drawTriangleVertical(float l, float t, float r, float b, bool d, ARGB c){
	//’ZŒa‚ğİ’è
	if (d) drawTriangle(l + (r / 2.0f), t, l + r, t + b, l, t + b, c);
	else drawTriangle(l + (r / 2.0f), t + b, l, t, l + r, t, c);
}

//–îˆó‚ğ•`‰æ ‰¡
void AstralDebu::drawArrowHorizontal(float cx, float cy, bool d, ARGB c){
	drawArrowHorizontal(CHIP(cx),CHIP_D(cy),CHIP(2),CHIP(1),d,c);
}

//–îˆó‚ğ•`‰æ c
void AstralDebu::drawArrowVertical(float cx, float cy, bool d, ARGB c){
	drawArrowVertical(CHIP(cx), CHIP_D(cy), CHIP(1), CHIP(2), d, c);
}

//–îˆó‚ğ•`‰æ ‰¡
void AstralDebu::drawArrowHorizontal(float l, float t, float r, float b, bool d, ARGB c){
	float qMarX = l + (d?CHIP(0.5f):0),qMarY = t + (b / 4.0f);
	float qLenX = r - CHIP(0.5f),qLenY = b / 2.0f;
	float qSliX = (d ? -2.0f : 2.0f), qSliY = 2.0f;
	float tMarX = l + (d?0:r-CHIP(0.5f)), tMarY = t;
	float tLenX = CHIP(0.5f), tLenY = b;
	float tSliX = (d ? 3.0f : 2.0f), tSliY = 4.0f, tSliB = 5.0f;

	drawQuad(qMarX, qMarY, qLenX, qLenY, BLACK);
	drawTriangleHorizontal(tMarX, tMarY, tLenX, tLenY, d, BLACK);
	drawQuad(qMarX + qSliX, qMarY + qSliY, qLenX, qLenY - qSliY * 2, c);
	drawTriangleHorizontal(tMarX + tSliX, tMarY + tSliY, tLenX - tSliB, tLenY - (tSliY*2.0f), d, c);
}

//–îˆó‚ğ•`‰æ c
void AstralDebu::drawArrowVertical(float l, float t, float r, float b, bool d, ARGB c){
	float qMarX = l + (r / 4.0f), qMarY = t + (d ? CHIP(0.5f) : 0);
	float qLenX = r / 2.0f, qLenY = b - CHIP(0.5f);
	float qSliX = 2.0f, qSliY = (d ? -2.0f : 2.0f);
	float tMarX = l, tMarY = t + (d ? 0 : b - CHIP(0.5f));
	float tLenX = r, tLenY = CHIP(0.5f);
	float tSliX = 4.0f, tSliY = (d ? 3.0f : 2.0f),tSliB = 5.0f;
	drawQuad(qMarX, qMarY, qLenX, qLenY, BLACK);
	drawTriangleVertical(tMarX,tMarY,tLenX,tLenY,d, BLACK);
	drawQuad(qMarX + qSliX, qMarY + qSliY, qLenX - (qSliX * 2), qLenY, c);
	drawTriangleVertical(tMarX+tSliX,tMarY+tSliY,tLenX-(tSliX*2.0f),tLenY-tSliB,d,c);
}

//ƒpƒlƒ‹‚ğ•`‰æ
void AstralDebu::drawPanel(std::string str, float cx, float cy, float len, ARGB c) {
	drawFrame(CHIP(cx),CHIP_D(cy),CHIP(len),CHIP(1.0f),2.0f,c,BLACK);
	middleF.print(str, CHIP(cx), CHIP_D(cy), CHIP(len),CHIP(1.0f), BLACK, DT_CC);
}

//‰æ‘œ‚Â‚«ƒpƒlƒ‹‚ğ•`‰æ
void AstralDebu::drawPanel(int img, float cx, float cy, ARGB c) {
	drawFrame(CHIP(cx) - 4, CHIP_D(cy) - 4, CHIP(1.0f) + 8, CHIP(1.0f) + 8, 2.0f, c, BLACK);
	chip.setX(CHIP(cx));
	chip.setY(CHIP_D(cy));
	chip.setCurrentFrame(img);
	chip.draw();
}

//‰æ‘œ‚Â‚«ƒpƒlƒ‹‚ğ•`‰æ
void AstralDebu::drawPanelD(int img1,int img2, float cx, float cy, ARGB c) {
	drawFrame(CHIP(cx) - 4, CHIP_D(cy) - 4, CHIP(2.5f) + 8, CHIP(1.0f) + 8, 2.0f, c, BLACK);
	chip.setX(CHIP(cx));
	chip.setY(CHIP_D(cy));
	chip.setCurrentFrame(img1);
	chip.draw();
	chip.setX(CHIP(cx + 1.5f));
	chip.setCurrentFrame(img2);
	chip.draw();
	middleF.print("+",CHIP(cx),CHIP_D(cy),CHIP(2.5f),CHIP(1.0f),BLACK,DT_CC);
}

//˜g•t‚«lŠp‚ğ•`‰æ
void AstralDebu::drawFrame(float l, float t, float r, float b, float f, ARGB c, ARGB fc){
	//–{‘Ì•`‰æ
	drawQuad(l, t, r, b, c);
	//˜g‚ğ•`‰æ
	drawQuad(l, t, r, f, fc);
	drawQuad(l, t + b - f, r, f, fc);
	drawQuad(l, t + f, f, b - (f * 2), fc);
	drawQuad(l + r - f, t + f, f, b - (f * 2), fc);
}

//‰A—L‚è˜g•t‚«lŠp‚ğ•`‰æ
void AstralDebu::drawFrame(float l, float t, float r, float b, float f, float s, ARGB c, ARGB fc, ARGB fs, ARGB fl){
	drawFrame(l,t,r,b,f,c,fc);
	//˜g‚Ì‰A‰e‚ğ•`‰æ
	drawQuad(l, t, r, s, fl);
	drawQuad(l + f - s, t + b - f, r - (f * 2) + (s * 2), s, fl);
	drawQuad(l + f - s, t + f - s, s, b - (f * 2) + s, fl);
	drawQuad(l + r - s, t + s, s, b - s, fl);

	drawQuad(l + f, t + f - s, r - (f * 2) + s, s, fs);
	drawQuad(l, t + b - s, r - s, s, fs);
	drawQuad(l, t + s, s, b - (s * 2), fs);
	drawQuad(l + r - f, t + f, s, b - (f * 2), fs);
}
