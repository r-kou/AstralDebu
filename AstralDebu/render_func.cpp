#include "astraldebu.h"
using namespace astralNS;

//表示するチップを設定
int AstralDebu::setChipImage(int c){
	if (stage <= 10) return c - 1;
	else if (stage <= 20) return c + 7;
	else if (stage <= 30) return c + 15;
	else return c + 23;
}

//四角の描画
void AstralDebu::setVertex(float l, float t, float r, float b, ARGB c){
	//左上
	vtx[0].x = l;
	vtx[0].y = t;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//右上
	vtx[1].x = l + r;
	vtx[1].y = t;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//右下
	vtx[2].x = l + r;
	vtx[2].y = t + b;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//左下
	vtx[3].x = l;
	vtx[3].y = t + b;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//四角を描画
void AstralDebu::drawQuad(float l, float t, float r, float b, ARGB c){
	//短径を設定
	setVertex(l, t, r, b, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphicsに任せる
	graphics->drawQuad(vertex);
	SAFE_RELEASE(vertex);
}

//三角のvertexを描画
void AstralDebu::setVertexTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//上
	vtx[0].x = x1;
	vtx[0].y = y1;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//右
	vtx[1].x = x2;
	vtx[1].y = y2;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//下
	vtx[2].x = x3;
	vtx[2].y = y3;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//上に帰ってくる
	vtx[3].x = x1;
	vtx[3].y = y1;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//三角を描画
void AstralDebu::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//短径を設定
	setVertexTriangle(x1, y1, x2, y2, x3, y3, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphcsに任せる
	graphics->drawQuad(vertex);
	SAFE_RELEASE(vertex);
}

//三角を描画
void AstralDebu::drawTriangleHorizontal(float l, float t, float r, float b, bool d, ARGB c){
	//短径を設定
	if (d) drawTriangle(l, t + (b / 2.0f), l + r, t, l+r, t + b, c);
	else drawTriangle(l+r, t + (b / 2.0f), l, t+b, l, t, c);
}

//三角を描画
void AstralDebu::drawTriangleVertical(float l, float t, float r, float b, bool d, ARGB c){
	//短径を設定
	if (d) drawTriangle(l + (r / 2.0f), t, l + r, t + b, l, t + b, c);
	else drawTriangle(l + (r / 2.0f), t + b, l, t, l + r, t, c);
}

//矢印を描画 横
void AstralDebu::drawArrowHorizontal(float cx, float cy, bool d, ARGB c){
	drawArrowHorizontal(CHIP(cx),CHIP_D(cy),CHIP(2),CHIP(1),d,c);
}

//矢印を描画 縦
void AstralDebu::drawArrowVertical(float cx, float cy, bool d, ARGB c){
	drawArrowVertical(CHIP(cx), CHIP_D(cy), CHIP(1), CHIP(2), d, c);
}

//矢印を描画 横
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

//矢印を描画 縦
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

//パネルを描画
void AstralDebu::drawPanel(std::string str, float cx, float cy, float len, ARGB c) {
	drawFrame(CHIP(cx),CHIP_D(cy),CHIP(len),CHIP(1.0f),2.0f,c,BLACK);
	middleF.print(str, CHIP(cx), CHIP_D(cy), CHIP(len),CHIP(1.0f), BLACK, DT_CC);
}

//画像つきパネルを描画
void AstralDebu::drawPanel(int img, float cx, float cy, ARGB c) {
	drawFrame(CHIP(cx) - 4, CHIP_D(cy) - 4, CHIP(1.0f) + 8, CHIP(1.0f) + 8, 2.0f, c, BLACK);
	chip.setX(CHIP(cx));
	chip.setY(CHIP_D(cy));
	chip.setCurrentFrame(img);
	chip.draw();
}

//画像つきパネルを描画
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

//枠付き四角を描画
void AstralDebu::drawFrame(float l, float t, float r, float b, float f, ARGB c, ARGB fc){
	//本体描画
	drawQuad(l, t, r, b, c);
	//枠を描画
	drawQuad(l, t, r, f, fc);
	drawQuad(l, t + b - f, r, f, fc);
	drawQuad(l, t + f, f, b - (f * 2), fc);
	drawQuad(l + r - f, t + f, f, b - (f * 2), fc);
}

//陰有り枠付き四角を描画
void AstralDebu::drawFrame(float l, float t, float r, float b, float f, float s, ARGB c, ARGB fc, ARGB fs, ARGB fl){
	drawFrame(l,t,r,b,f,c,fc);
	//枠の陰影を描画
	drawQuad(l, t, r, s, fl);
	drawQuad(l + f - s, t + b - f, r - (f * 2) + (s * 2), s, fl);
	drawQuad(l + f - s, t + f - s, s, b - (f * 2) + s, fl);
	drawQuad(l + r - s, t + s, s, b - s, fl);

	drawQuad(l + f, t + f - s, r - (f * 2) + s, s, fs);
	drawQuad(l, t + b - s, r - s, s, fs);
	drawQuad(l, t + s, s, b - (s * 2), fs);
	drawQuad(l + r - f, t + f, s, b - (f * 2), fs);
}

//チップ画像を敷き詰める
void AstralDebu::spreadChip(int l,int t,int r,int b,int c) {
	chip.setCurrentFrame(c);
	for (int j = t; j <= b; j++){
		chip.setY((float)CHIP_D(j));
		for (int i = l; i <= r; i++){
			chip.setX((float)CHIP(i));
			chip.draw();
		}
	}
}

//文字色変化
int AstralDebu::colorWave(float f){
	while (f > 1.0f) f -= 1.0f;
	if (f < 0.0f) f = 0.0f;
	if (f < 0.5f) return (int) (255 * f);
	else return (int)(255 * (1.0f - f));
}