#ifndef _IMAGE_H
#define _IMAGE_H
#define WIN32_LEAN_AND_MEAN

#include "const.h"
#include "texture.h"

//画像を描画するクラス
class Image{
protected:
	//ポインタとか
	Graphics *graphics;
	Texture *texture;
	//スプライト
	Sprite sprite;
	ARGB filter;
	//フレームとか
	int cols;
	int startFrame, endFrame, currentFrame;
	float delay;
	float animTimer;
	HRESULT hw;
	bool loop, visible, complete;
	bool initialized;
public:
	//コンストラクタ
	Image();
	//デストラクタ
	virtual ~Image();

	//初期化
	virtual bool initialize(Graphics *g, Texture *t, int width, int height, int ncols);
	//描画
	virtual void draw(ARGB color = graphicsNS::WHITE);
	//スプライトを描画
	virtual void draw(Sprite s, ARGB color = graphicsNS::WHITE);
	//更新
	virtual void update(float flameTime);

	//getter
	const virtual Sprite& getSprite() { return sprite; }
	virtual bool getVisible() { return visible; }
	virtual float getX() { return sprite.x; }
	virtual float getY() { return sprite.y; }
	virtual float getScale() { return sprite.scale; }
	virtual int getWidth() { return sprite.width; }
	virtual int getHeight() { return sprite.height; }
	virtual float getCenterX()      { return sprite.x + sprite.width / 2 * getScale(); }
	virtual float getCenterY()      { return sprite.y + sprite.height / 2 * getScale(); }
	virtual float getDegrees()      { return sprite.angle*(180.0f / (float)PI); }
	virtual float getRadians()      { return sprite.angle; }
	virtual float getDelay()   { return delay; }
	virtual int   getStartFrame()   { return startFrame; }
	virtual int   getEndFrame()     { return endFrame; }
	virtual int   getCurrentFrame() { return currentFrame; }
	virtual RECT  getRect() { return sprite.rect; }
	virtual bool  getComplete() { return complete; }
	virtual bool getFlipH() { return sprite.flipH; }
	virtual bool getFlipV() { return sprite.flipV; }
	virtual ARGB getColorFilter() { return filter; }

	virtual void setX(float newX)   { sprite.x = newX; }
	virtual void setY(float newY)   { sprite.y = newY; }
	virtual void setScale(float s)  { sprite.scale = s; }
	virtual void setDegrees(float deg)  { sprite.angle = deg*((float)PI / 180.0f); }
	virtual void setRadians(float rad)  { sprite.angle = rad; }
	virtual void setVisible(bool v) { visible = v; }
	virtual void setDelay(float d) { delay = d; }
	virtual void setFrames(int s, int e){ startFrame = s; endFrame = e; }
	virtual void setCurrentFrame(int c);
	virtual void setRect();
	virtual void setRect(RECT r)  { sprite.rect = r; }
	virtual void setLoop(bool lp) { loop = lp; }
	virtual void setComplete(bool a) { complete = a; };
	virtual void setColorFilter(ARGB color) { filter = color; }
	virtual void setTexture(Texture *t) { texture = t; }
	virtual void setFlipH(bool flip)  { sprite.flipH = flip; }
	virtual void setFlipV(bool flip)    { sprite.flipV = flip; }
};
#endif