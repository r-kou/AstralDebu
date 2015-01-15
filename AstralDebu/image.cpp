#include "image.h"

//�R���X�g���N�^
Image::Image(){
	sprite.width = 2;
	sprite.height = 2;
	sprite.x = 0.0f;
	sprite.y = 0.0f;
	sprite.scale = 1.0f;
	sprite.angle = 0.0;
	SETRECT(sprite.rect, 0, 0, sprite.width, sprite.height);
	sprite.texture = NULL;
	sprite.flipH = false;
	sprite.flipV = false;

	cols = 1;
	texture = NULL;
	graphics = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	delay = 1.0;
	animTimer = 0.0;
	visible = true;
	loop = true;
	complete = false;
	filter = graphicsNS::WHITE;
}

//�f�X�g���N�^
Image::~Image(){

}

//������
bool Image::initialize(Graphics *g, Texture *t, int width, int height, int ncols){
	try{
		//�e�����o��ݒ�
		graphics = g;
		texture = t;
		sprite.texture = texture->getTexture();

		if (width <= 0) width = texture->getWidth();
		sprite.width = width;

		if (height <= 0) height = texture->getHeight();
		sprite.height = height;

		cols = ncols;
		if (cols <= 0) cols = 1;

		setRect();
	}
	catch (...) { return false; }

	initialized = true;
	return true;
}

//�`��
void Image::draw(ARGB color){
	//��\���Ȃ�Ȃɂ����Ȃ�
	if (!visible || graphics == NULL) return;

	//�摜��`��
	sprite.texture = texture->getTexture();
	if (color == graphicsNS::FILTER) graphics->drawSprite(sprite, filter);
	else graphics->drawSprite(sprite, color);
}

//�w��X�v���C�g��`��
void Image::draw(Sprite s, ARGB color){
	//��\���Ȃ牽�����Ȃ�
	if (!visible || graphics == NULL) return;

	//�X�v���C�g��ݒ�
	s.rect = sprite.rect;
	s.texture = texture->getTexture();

	//�`��
	if (color == graphicsNS::FILTER) graphics->drawSprite(s, filter);
	else graphics->drawSprite(s, color);
}

//�t���[�����X�V
void Image::update(float frameTime)
{
	//�J�n���I���Ȃ�A�j���[�V����
	if (endFrame - startFrame > 0) {
		//���Ԃ�ǉ�
		animTimer += frameTime;
		//�ҋ@���Ԃ��߂���΃A�j���[�V����
		if (animTimer > delay) {
			animTimer -= delay;
			//�t���[���i�߂�
			currentFrame++;
			//�A�j���[�V�����I�������ׂ�
			if (currentFrame < startFrame || currentFrame > endFrame) {
				//���[�v�Ȃ�߂�
				if (loop) currentFrame = startFrame;
				//���[�v�łȂ��Ȃ�I������
				else   {
					currentFrame = endFrame;
					complete = true;
				}
			}
			setRect();
		}
	}
}

//�t���[����ݒ�
void Image::setCurrentFrame(int c)
{
	if (c >= 0) {
		currentFrame = c;
		complete = false;
		setRect();
	}
}

//RECT���Đݒ�
inline void Image::setRect()
{
	SETRECT(sprite.rect, (currentFrame % cols) * sprite.width,
		(currentFrame / cols) * sprite.height, sprite.width, sprite.height);
}