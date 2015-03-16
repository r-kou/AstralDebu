#ifndef _TEXT_H
#define _TEXT_H
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "const.h"
#include "graphics.h"

class Text{
private:
	Graphics *graphics;
	ARGB color;
	LP_FONT font;
	RECT rect;
	D3DXMATRIX matrix;
	float angle;
public:
	//�R���X�g���N�^
	Text();
	//�f�X�g���N�^
	virtual ~Text();

	//������
	virtual bool initialize(Graphics *g, int height, bool bold, bool italic, const std::string &fontName);
	//�`��
	virtual int print(const std::string &str, int x, int y);
	//�t�H�[�}�b�g���w�肵�ĕ`��
	virtual int print(const std::string &str, RECT &rect, UINT format);
	//�ʒu�C�͈́C�F�C�t�H�[�}�b�g���܂Ƃ߂Ďw�肵�ĕ`��
	virtual int print(const std::string &str, int l, int t, int r, int b, ARGB c, UINT format);
	//�ʒu�C�͈́C�F�C�t�H�[�}�b�g���܂Ƃ߂Ďw�肵�ĕ`�� float��
	virtual int print(const std::string &str, float l, float t, float r, float b, ARGB c, UINT format);
	//�e�t���ŕ�����`��
	virtual int printShadow(const std::string &str, float l, float t, float r, float b, float e,ARGB c, ARGB s, UINT format);
	//setter
	virtual void setDegrees(float deg) { angle = deg*(180.0f / (float)PI); }
	virtual void setRadians(float rad) { angle = rad; }
	virtual void setFontColor(ARGB c) { color = c; }

	//getter
	virtual float getDegrees() { return angle* (180.0f / (float)PI); }
	virtual float getRadians() { return angle; }
	virtual ARGB getFontColor() { return color; }

	//�f�o�C�X�폜
	virtual void onLostDevice();
	//�f�o�C�X���Z�b�g
	virtual void onResetDevice();
};

#endif