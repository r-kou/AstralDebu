#ifndef _ENTITY_H
#define _ENTITY_H
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "graphics.h"
#include "input.h"
#include "texture.h"
#include "game.h"

namespace entityNS{
	enum ENTITY_TYPE {
		NONE, DEBU,
		BOX_W, BOX_S,
		BOX_L, BOX_B,
		BOX_H, BOX_A,
		BOX_F, BOMB,
		HIBOMB, ROCK,
		HAMMER, MEAT,
		HIMEAT, EN_1,
		EN_2, EN_3,
		EN_4, EN_5,
		EN_B, EN_M,
		WARP_R, WARP_G,
		WARP_Y, GOAL,
		BLAST
	};
	enum ENTITY_STATE {
		EMPTY, LOCK,
		STAND, JUMP,
		CLEAR, KNOCK,
		DEAD, LADDER,
		HOLD_HAMMER,
		ACT
	};
	//衝突判定の上下左右
	const UCHAR LEFT = 1;
	const UCHAR TOP = 2;
	const UCHAR RIGHT = 4;
	const UCHAR BOTTOM = 8;
	const UCHAR GETLADDER = 16;
	//衝突判定の四隅
	const UCHAR EDGE1 = LEFT + TOP;
	const UCHAR EDGE2 = RIGHT + TOP;
	const UCHAR EDGE3 = RIGHT + BOTTOM;
	const UCHAR EDGE4 = LEFT + BOTTOM;
	//はしご
	const int CHIP_LADDER = 30;
	//edgeの最大値
	const int EDGE_MAX = 16;
	//落下速度
	const float GRAVITY_RATE = 1000.0f;
	//落下速度の最大
	const float VEL_FALL_MAX = 300;

}

class Entity{
protected:
	//グラフィックスポインタ
	Graphics *graphics;
	//使う画像
	Texture *texture;
	Image image;

	//状態
	entityNS::ENTITY_STATE state;
	//分類
	entityNS::ENTITY_TYPE type;

	//座標
	VC2 pos;
	//速度
	VC2 vel;
	//向き
	bool direct;
	//大きさ　xy同じ
	int size;
	//表示する画像
	int img;
	//衝突判定用の短径
	RECT edge;
	//短径の幅
	int edge_x, edge_y;
	//許容余白
	int margin_x, margin_y;
	//ワープの転移先　ワープ専用
	int partnerX, partnerY;
	//行数
	int col;
	//汎用インターバル
	float anim_interval;
	//転移インターバル
	float warp_interval;
	//状態が変更されたか
	bool state_change;
	//左右の床への衝突判定
	bool bottom_object[2];
	//特殊行動中か判定
	bool action;
	//オブジェクトへの反応の判定
	INT response;
	//オブジェクトへの反応のオプション
	VC2 responseVC[32];

	//速度の差を求める
	float diffVelX(Entity *e) { return vel.x - e->getVelX(); }
	float diffVelY(Entity *e) { return vel.y - e->getVelY(); }
	//判定の差を求める　trueならマージン有
	long diffLeft(Entity *e, bool m) { return getLeft(m) - e->getRight(m); }
	long diffRight(Entity *e, bool m) { return getRight(m) - e->getLeft(m); }
	long diffTop(Entity *e, bool m) { return getTop(m) - e->getBottom(m); }
	long diffBottom(Entity *e, bool m) { return getBottom(m) - e->getTop(m); }
	//爆風でどこに飛ぶかを決める
	bool blastLeft(Entity *e) { return (getPosX() - e->getPosX() < -8); }
	bool blastRight(Entity *e) { return (getPosX() - e->getPosX() > 8); }
	bool blastTop(Entity *e) { return (getPosY() - e->getPosY() < -8); }
	bool blastBottom(Entity *e) { return (getPosY() - e->getPosY() > 8); }
	//オブジェクト反応の演算
	UINT shift(int i) { return 1 << i; }
	const UINT getRes(int i) { return response & shift(i); }
	const float getResX(int i) { return responseVC[i].x; }
	const float getResY(int i) { return responseVC[i].y; }
	void setRes(int i) { response |= shift(i); }
	void setRes(int i, float x, float y) { response |= shift(i); responseVC[i].x = x; responseVC[i].y = y; }
public:
	//コンストラクタ
	Entity();
	//デストラクタ
	virtual ~Entity();

	//初期化
	virtual bool initialize(Game *game, Texture *t, int i, int j);

	//移動
	virtual void move(float frameTime);

	//地形への接触判定
	virtual void touchMap(int map[MAP_COL][MAP_ROW]);

	//上下左右の衝突判定
	virtual UCHAR touchMapDirect(int c, UCHAR t);

	//地形への接触
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触判定
	virtual void touchObj(Entity *e);

	//各方向への判定が重なっているかを調べる
	virtual bool touchObjDirect(long x, long y, float v);

	//他オブジェクトへの接触
	virtual void collideObj(Entity *e, UCHAR t);

	//他オブジェクトへの反応
	virtual void responseObj();

	//描画する画像を変更
	virtual void changeImage();

	//画像変更の一括設定
	virtual void setImage(int s, int e, bool l);

	//一枚絵の設定
	virtual void setImage(int c);

	//描画
	virtual void draw();

	//判定の位置を設定
	virtual void setEdge();

	//死亡判定
	virtual bool isDead() { return (state == entityNS::DEAD) && (anim_interval == 0.0f); }

	//左の接地判定を求める
	bool collideBottomLeft(Entity *e) { return (getLeft(true) >= e->getLeft(false)) && (getLeft(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//右の接地判定を求める
	bool collideBottomRight(Entity *e) { return (getRight(true) >= e->getLeft(false)) && (getRight(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//左右の接地判定
	void bottomObj(bool d, Entity *e);

	//getter
	const entityNS::ENTITY_STATE getState() { return state; }
	const entityNS::ENTITY_TYPE getType() { return type; }
	const VC2* getPos(){ return &pos; }
	const VC2* getVel(){ return &vel; }
	const float getPosX() { return pos.x; }
	const float getPosY() { return pos.y; }
	const float getVelX() { return vel.x; }
	const float getVelY() { return vel.y; }
	const bool getDirect() { return direct; }
	const int getSize() { return size; }
	const RECT& getEdge() { return edge; }
	const int getEdgeX() { return edge_x; }
	const int getEdgeY() { return edge_y; }
	const bool getComplete() { return image.getComplete(); }
	const int getMarginX() { return margin_x; }
	const int getMarginY() { return margin_y; }
	const int getPartnerX() { return partnerX; }
	const int getPartnerY() { return partnerY; }
	const float getAnim() { return anim_interval; }
	const float getWarp() { return warp_interval; }
	const bool getAction() { return action; }
	const UINT getResponse() { return response; }

	//マップ上での座標を返す
	const int ChipX(float x) { return (int)x / CHIP_SIZE; }
	const int ChipY(float y) { return (int)(y - DATA_LEN) / CHIP_SIZE; }
	const int ChipCX() { return ChipX(pos.x); }
	const int ChipCY() { return ChipY(pos.y); }
	//当たり判定の式長いので短縮用
	const long getLeft(bool b) { return edge.left + (b ? margin_x : 0); }
	const long getRight(bool b) { return edge.right - (b ? margin_x : 0); }
	const long getTop(bool b) { return edge.top + (b ? margin_y : 0); }
	const long getBottom(bool b) { return edge.bottom - (b ? margin_y : 0); }

	//setter
	void setState(entityNS::ENTITY_STATE s) { state = s; }
	void setType(entityNS::ENTITY_TYPE t) { type = t; }
	void setPosX(float n) { pos.x = n; }
	void setPosY(float n) { pos.y = n; }
	void setVelX(float n) { vel.x = n; }
	void setVelY(float n) { vel.y = n; }
	void setDirect(bool d){ direct = d; }
	void setChange(bool s) { state_change = s; }
	void setPartnerX(int x) { partnerX = x; }
	void setPartnerY(int y) { partnerY = y; }
	void setAnim(float a) { anim_interval = a; }
	void setWarp(float i) { warp_interval = i; }
	void setAction(bool b) { action = b; }
	void resetResponse() { response = 0; }

	//マップチップに合わせる
	void setCX() { pos.x = (float)(ChipCX() + 0.5f) * CHIP_SIZE; }
	void setCY() { pos.y = (float)(ChipCY() + 0.5f) * CHIP_SIZE + DATA_LEN; }
	//判定をぎりぎりに抑える
	void setLeft(bool b) { pos.x = (float)ChipCX() * CHIP_SIZE + edge_x - (b ? margin_x : 0); }
	void setRight(bool b) { pos.x = (float)(ChipCX() + 1) * CHIP_SIZE - edge_x + (b ? margin_x : 0); }
	void setTop(bool b) { pos.y = (float)ChipCY() * CHIP_SIZE + DATA_LEN + edge_y - (b ? margin_y : 0); }
	void setBottom(bool b) { pos.y = (float)(ChipCY() + 1) * CHIP_SIZE + DATA_LEN - edge_y + (b ? margin_y : 0); }
};


#endif