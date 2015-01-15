#include "image.h"

//コンストラクタ
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

//デストラクタ
Image::~Image(){

}

//初期化
bool Image::initialize(Graphics *g, Texture *t, int width, int height, int ncols){
	try{
		//各メンバを設定
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

//描画
void Image::draw(ARGB color){
	//非表示ならなにもしない
	if (!visible || graphics == NULL) return;

	//画像を描画
	sprite.texture = texture->getTexture();
	if (color == graphicsNS::FILTER) graphics->drawSprite(sprite, filter);
	else graphics->drawSprite(sprite, color);
}

//指定スプライトを描画
void Image::draw(Sprite s, ARGB color){
	//非表示なら何もしない
	if (!visible || graphics == NULL) return;

	//スプライトを設定
	s.rect = sprite.rect;
	s.texture = texture->getTexture();

	//描画
	if (color == graphicsNS::FILTER) graphics->drawSprite(s, filter);
	else graphics->drawSprite(s, color);
}

//フレームを更新
void Image::update(float frameTime)
{
	//開始＜終了ならアニメーション
	if (endFrame - startFrame > 0) {
		//時間を追加
		animTimer += frameTime;
		//待機時間を過ぎればアニメーション
		if (animTimer > delay) {
			animTimer -= delay;
			//フレーム進める
			currentFrame++;
			//アニメーション終了か調べる
			if (currentFrame < startFrame || currentFrame > endFrame) {
				//ループなら戻る
				if (loop) currentFrame = startFrame;
				//ループでないなら終了する
				else   {
					currentFrame = endFrame;
					complete = true;
				}
			}
			setRect();
		}
	}
}

//フレームを設定
void Image::setCurrentFrame(int c)
{
	if (c >= 0) {
		currentFrame = c;
		complete = false;
		setRect();
	}
}

//RECTを再設定
inline void Image::setRect()
{
	SETRECT(sprite.rect, (currentFrame % cols) * sprite.width,
		(currentFrame / cols) * sprite.height, sprite.width, sprite.height);
}