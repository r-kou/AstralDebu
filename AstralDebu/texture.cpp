#include "texture.h"

//コンストラクタ
Texture::Texture(){
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;
}

//デストラクタ
Texture::~Texture(){
	SAFE_RELEASE(texture);
}

//初期化
bool Texture::initialize(Graphics *g, const char *f){
	hr = E_FAIL;
	try{
		graphics = g;
		file = f;
		hr = graphics->loadTexture(file, TRANS, width, height, texture);
		if (FAILED(hr)){
			SAFE_RELEASE(texture);
			return false;
		}
	}
	catch (...){ return false; }

	initialized = true;
	return true;
}

//デバイス削除
void Texture::onLostDevice(){
	if (!initialized) return;
	SAFE_RELEASE(texture);
}
//デバイスリセット
void Texture::onResetDevice(){
	if (!initialized) return;
	graphics->loadTexture(file, TRANS, width, height, texture);
}