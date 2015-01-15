#ifndef _TEXTURE_H
#define _TEXTURE_H
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "const.h"

//�摜�f�[�^���Ǘ�����N���X
class Texture {
private:
	//�摜�T�C�Y
	UINT width, height;
	//�e�N�X�`���f�[�^
	LP_TEXTURE texture;
	//�t�@�C����
	const char *file;
	//�O���t�B�b�N
	Graphics *graphics;
	//�������ς݂�
	bool initialized;
	//������������
	HRESULT hr;
public:
	//�R���X�g���N�^
	Texture();
	//�f�X�g���N�^
	virtual ~Texture();

	//������
	virtual bool initialize(Graphics *g, const char *f);
	//�f�o�C�X�폜
	virtual void onLostDevice();
	//�f�o�C�X���Z�b�g
	virtual void onResetDevice();

	//getter
	LP_TEXTURE getTexture() const { return texture; }
	UINT getWidth() const { return width; }
	UINT getHeight() const { return height; }
};

#endif