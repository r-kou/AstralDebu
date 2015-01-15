#include "texture.h"

//�R���X�g���N�^
Texture::Texture(){
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;
}

//�f�X�g���N�^
Texture::~Texture(){
	SAFE_RELEASE(texture);
}

//������
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

//�f�o�C�X�폜
void Texture::onLostDevice(){
	if (!initialized) return;
	SAFE_RELEASE(texture);
}
//�f�o�C�X���Z�b�g
void Texture::onResetDevice(){
	if (!initialized) return;
	graphics->loadTexture(file, TRANS, width, height, texture);
}