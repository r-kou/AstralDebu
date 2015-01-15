#ifndef _TEXTURE_H
#define _TEXTURE_H
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "const.h"

//画像データを管理するクラス
class Texture {
private:
	//画像サイズ
	UINT width, height;
	//テクスチャデータ
	LP_TEXTURE texture;
	//ファイル名
	const char *file;
	//グラフィック
	Graphics *graphics;
	//初期化済みか
	bool initialized;
	//処理成功判定
	HRESULT hr;
public:
	//コンストラクタ
	Texture();
	//デストラクタ
	virtual ~Texture();

	//初期化
	virtual bool initialize(Graphics *g, const char *f);
	//デバイス削除
	virtual void onLostDevice();
	//デバイスリセット
	virtual void onResetDevice();

	//getter
	LP_TEXTURE getTexture() const { return texture; }
	UINT getWidth() const { return width; }
	UINT getHeight() const { return height; }
};

#endif