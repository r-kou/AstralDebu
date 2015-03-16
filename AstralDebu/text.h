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
	//コンストラクタ
	Text();
	//デストラクタ
	virtual ~Text();

	//初期化
	virtual bool initialize(Graphics *g, int height, bool bold, bool italic, const std::string &fontName);
	//描画
	virtual int print(const std::string &str, int x, int y);
	//フォーマットを指定して描画
	virtual int print(const std::string &str, RECT &rect, UINT format);
	//位置，範囲，色，フォーマットをまとめて指定して描画
	virtual int print(const std::string &str, int l, int t, int r, int b, ARGB c, UINT format);
	//位置，範囲，色，フォーマットをまとめて指定して描画 float版
	virtual int print(const std::string &str, float l, float t, float r, float b, ARGB c, UINT format);
	//影付きで文字を描画
	virtual int printShadow(const std::string &str, float l, float t, float r, float b, float e,ARGB c, ARGB s, UINT format);
	//setter
	virtual void setDegrees(float deg) { angle = deg*(180.0f / (float)PI); }
	virtual void setRadians(float rad) { angle = rad; }
	virtual void setFontColor(ARGB c) { color = c; }

	//getter
	virtual float getDegrees() { return angle* (180.0f / (float)PI); }
	virtual float getRadians() { return angle; }
	virtual ARGB getFontColor() { return color; }

	//デバイス削除
	virtual void onLostDevice();
	//デバイスリセット
	virtual void onResetDevice();
};

#endif