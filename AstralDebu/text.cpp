#include "text.h"

//コンストラクタ
Text::Text(){
	SETRECT(rect, 0, 0, WINDOW_W, WINDOW_H);
	font = NULL;
	angle = 0;
}
//デストラクタ
Text::~Text(){
	SAFE_RELEASE(font);
}
//初期化
bool Text::initialize(Graphics *g, int height, bool bold, bool italic, const std::string &fontName){
	graphics = g;
	UINT weight = FW_NORMAL;
	if (bold) weight = FW_BOLD;
	if (FAILED(D3DXCreateFont(
		graphics->getDevice(), height,
		0, weight,
		1, italic,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
		fontName.c_str(), &font)))
		return false;
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);
	return true;
}

//描画
int Text::print(const std::string &str, int x, int y){
	if (font == NULL) return 0;
	rect.top = y;
	rect.left = x;

	D3DXVECTOR2 rCenter = D3DXVECTOR2((float)x, (float)y);
	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, &rCenter, angle, NULL);

	graphics->getSprite()->SetTransform(&matrix);
	return font->DrawTextA(
		graphics->getSprite(), str.c_str(),
		-1, &rect,
		DT_LEFT, color);
}

//フォーマットを指定して描画
int Text::print(const std::string &str, RECT &rect, UINT format){
	if (font == NULL) return 0;

	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	graphics->getSprite()->SetTransform(&matrix);
	return font->DrawTextA(
		graphics->getSprite(), str.c_str(),
		-1, &rect,
		format, color);
}

//位置，範囲，色をまとめて指定して描画　中央上部揃え
int Text::print(const std::string &str, int l, int t, int r, int b, ARGB c, UINT format){
	if (font == NULL) return 0;

	setFontColor(c);
	SETRECT(rect, l, t, r, b);
	return print(str, rect, format);
}

//デバイス削除
void Text::onLostDevice(){
	if (font == NULL) return;
	font->OnLostDevice();
}

//デバイスリセット
void Text::onResetDevice(){
	if (font == NULL) return;
	font->OnResetDevice();
}