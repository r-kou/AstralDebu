#include "text.h"

//�R���X�g���N�^
Text::Text(){
	SETRECT(rect, 0, 0, WINDOW_W, WINDOW_H);
	font = NULL;
	angle = 0;
}
//�f�X�g���N�^
Text::~Text(){
	SAFE_RELEASE(font);
}
//������
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

//�`��
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

//�t�H�[�}�b�g���w�肵�ĕ`��
int Text::print(const std::string &str, RECT &rect, UINT format){
	if (font == NULL) return 0;

	D3DXMatrixTransformation2D(&matrix, NULL, 0.0f, NULL, NULL, NULL, NULL);
	graphics->getSprite()->SetTransform(&matrix);
	return font->DrawTextA(
		graphics->getSprite(), str.c_str(),
		-1, &rect,
		format, color);
}

//�ʒu�C�͈́C�F���܂Ƃ߂Ďw�肵�ĕ`��@�����㕔����
int Text::print(const std::string &str, int l, int t, int r, int b, ARGB c, UINT format){
	if (font == NULL) return 0;

	setFontColor(c);
	SETRECT(rect, l, t, r, b);
	return print(str, rect, format);
}

//�f�o�C�X�폜
void Text::onLostDevice(){
	if (font == NULL) return;
	font->OnLostDevice();
}

//�f�o�C�X���Z�b�g
void Text::onResetDevice(){
	if (font == NULL) return;
	font->OnResetDevice();
}