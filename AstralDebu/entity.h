#ifndef _ENTITY_H
#define _ENTITY_H
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "graphics.h"
#include "input.h"
#include "texture.h"
#include "audio.h"
#include "game.h"

namespace entityNS{
	enum ENTITY_TYPE {
		TY_NONE, TY_DEBU,
		TY_WOOD_BOX, TY_STEEL_BOX,
		TY_LEAD_BOX, TY_BOMB_BOX,
		TY_HIBOMB_BOX, TY_AIR_BOX,
		TY_FRAME_BOX, TY_GOAST_BOX,
		TY_BOMB, TY_HIBOMB,
		TY_MINE, TY_ROCK,
		TY_HAMMER, TY_MEAT,
		TY_HIMEAT, TY_ENEMY_1,
		TY_ENEMY_2, TY_ENEMY_3,
		TY_ENEMY_4, TY_ENEMY_5,
		TY_BULLET, TY_MISSILE,
		TY_RED_WARP, TY_GREEN_WARP,
		TY_YELLOW_WARP, TY_LADDER,
		TY_GOAL, TY_BLAST
	};
	enum ENTITY_STATE {
		ST_EMPTY, ST_LOCK,
		ST_STAND, ST_JUMP,
		ST_CLEAR, ST_KNOCK,
		ST_DEAD, ST_LADDER,
		ST_HAMMER, ST_ACTION
	};
	enum RENDER_ORDER {
		RO_BASE, RO_BOX,
		RO_OBJECT, RO_ENEMY,
		RO_BLAST, RO_HOLD,
		RO_DEBU, RO_HAMMER
	};
	//衝突判定の上下左右
	const UCHAR LEFT = 1;
	const UCHAR TOP = 2;
	const UCHAR RIGHT = 4;
	const UCHAR BOTTOM = 8;
	//衝突判定の四隅
	const UCHAR EDGE1 = LEFT + TOP;
	const UCHAR EDGE2 = RIGHT + TOP;
	const UCHAR EDGE3 = RIGHT + BOTTOM;
	const UCHAR EDGE4 = LEFT + BOTTOM;
	//爆発画像のチップの大きさ
	const int BLAST_SIZE = 96;
	//各画像の横チップ数
	const int COL_CHIP = 8;
	const int COL_DEBU = 7;
	const int COL_BLAST = 6;
	const int COL_ENEMY = 7;
	//edgeの最大値
	const int EDGE_MAX = 16;
	//落下速度
	const float GRAVITY_RATE = 1000.0f;
	//ぶつかった時の落下補正
	const float TOP_GRAVITY_RATE = 1000.0f;
	//吹っ飛び時の微妙な浮き
	const float VEL_KNOCK_JUMP = 150;
	//吹っ飛び時の上方向への最大速度(KNOCK_JUMP差引き)
	const float VEL_MAX_JUMP = 250;
	//吹っ飛び時の最大速度
	const float VEL_MAX = 400;
	//反応の対応値
	const UINT RES_LEFT = 0;
	const UINT RES_RIGHT = 1;
	const UINT RES_TOP = 2;
	const UINT RES_BOTTOM = 3;
	const UINT RES_BOTTOM_CHIP = 4;
	const UINT RES_STOP = 5;
	const UINT RES_CHIP = 6;
	const UINT RES_COLLIDE = 7;
	const UINT RES_WARP = 8;
	const UINT RES_KNOCK = 9;
	const UINT RES_JUMP = 10;
	const UINT RES_DEAD = 11;
	const UINT RES_LADDER = 12;
	const UINT RES_CLEAR = 13;
}

class Entity{
protected:
	//グラフィックスポインタ
	Graphics *graphics;
	//音声ポインタ
	Audio *audio;
	//使う画像
	Texture *texture;
	Image image;

	//状態
	entityNS::ENTITY_STATE state;
	//分類
	entityNS::ENTITY_TYPE type;
	//描画順
	entityNS::RENDER_ORDER renderOrder;
	//座標
	VC2 pos;
	//速度
	VC2 vel;
	//移動距離
	VC2 mov;
	//向き
	bool direct;
	//大きさ　xy同じ
	int size;
	//表示する画像
	int img;
	//衝突判定用の短径
	RECT edge;
	//短径の幅
	int edgeX, edgeY;
	//許容余白
	int marginX, marginY;
	//ワープの転移先　ワープ専用
	int partnerX, partnerY;
	//行数
	int col;
	//フレーム時間
	float frameTime;
	//汎用インターバル
	float animInterval;
	//転移インターバル
	float warpInterval;
	//吹っ飛び用インターバル
	float knockInterval;
	//状態が変更されたか
	bool stateChanged;
	//左右の床への衝突判定
	bool bottomObj[2];
	//特殊行動中か判定
	bool action;
	//オブジェクトへの反応の判定
	int response;
	//オブジェクトへの反応のオプション
	VC2 responseVC[32];
	//置いた時の音
	std::string putSound;
	//破壊時の音
	std::string deadSound;
	//半透明にするか
	bool trans;
	//落下する科
	bool fall;

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
	float blastX(Entity *e, float x) { if (blastLeft(e)) return -x; else if (blastRight(e)) return x; else return 0; }
	float blastY(Entity *e, float y) { if (blastTop(e)) return -y; else if (blastBottom(e)) return y; else return 0; }
	//オブジェクト反応の演算
	UINT shift(int i) { return 1 << i; }
	const UINT getRes(int i) { return response & shift(i); }
	const float getResX(int i) { return responseVC[i].x; }
	const float getResY(int i) { return responseVC[i].y; }
	void setRes(int i) { response |= shift(i); }
	void setRes(int i, float x, float y) { response |= shift(i); responseVC[i].x += x; responseVC[i].y += y; }
	//各値の最大値を設定
	const float setLimit(float n, float max) { return ((n>max) ? max : ((n<-max) ? -max : n)); }

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
	virtual UCHAR touchMapDirect(int c, UCHAR t) { return c ? t : 0; }

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
	virtual bool isDead() { return (state == entityNS::ST_DEAD) && (animInterval == 0.0f); }

	//左の接地判定を求める
	bool collideBottomLeft(Entity *e) { return (getLeft(true) >= e->getLeft(false)) && (getLeft(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//右の接地判定を求める
	bool collideBottomRight(Entity *e) { return (getRight(true) >= e->getLeft(false)) && (getRight(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//左右の接地判定
	void checkBottom(bool d, Entity *e);

	//着地音を鳴らす
	void playPut() { if (putSound != "") audio->playCue(putSound.c_str()); }

	//死亡音を鳴らす
	void playDead() { if (deadSound != "") audio->playCue(deadSound.c_str()); }

	//getter
	const entityNS::ENTITY_STATE getState() { return state; }
	const entityNS::ENTITY_TYPE getType() { return type; }
	const entityNS::RENDER_ORDER getRenderOrder() { return renderOrder; }
	const VC2* getPos(){ return &pos; }
	const VC2* getVel(){ return &vel; }
	const VC2* getMov(){ return &mov; }
	const float getPosX() { return pos.x; }
	const float getPosY() { return pos.y; }
	const float getVelX() { return vel.x; }
	const float getVelY() { return vel.y; }
	const float getMovX() { return mov.x; }
	const float getMovY() { return mov.y; }
	const bool getDirect() { return direct; }
	const int getSize() { return size; }
	const RECT& getEdge() { return edge; }
	const int getEdgeX() { return edgeX; }
	const int getEdgeY() { return edgeY; }
	const bool getComplete() { return image.getComplete(); }
	const int getMarginX() { return marginX; }
	const int getMarginY() { return marginY; }
	const int getPartnerX() { return partnerX; }
	const int getPartnerY() { return partnerY; }
	const float getAnim() { return animInterval; }
	const float getWarp() { return warpInterval; }
	const bool getAction() { return action; }
	const int getResponse() { return response; }
	const std::string getPut() { return putSound; }

	//マップ上での座標を返す
	const int ChipX(float x) { return (int)x / CHIP_SIZE; }
	const int ChipY(float y) { return (int)(y - DATA_LEN) / CHIP_SIZE; }
	const int ChipCX() { return ChipX(pos.x); }
	const int ChipCY() { return ChipY(pos.y); }
	//当たり判定の式長いので短縮用
	const long getLeft(bool b) { return edge.left + (b ? marginX - (int)mov.x : 0); }
	const long getRight(bool b) { return edge.right - (b ? marginX + (int)mov.x : 0); }
	const long getTop(bool b) { return edge.top + (b ? marginY - (int)mov.y : 0); }
	const long getBottom(bool b) { return edge.bottom - (b ? marginY + (int)mov.y : 0); }

	//setter
	void setState(entityNS::ENTITY_STATE s) { state = s; }
	void setType(entityNS::ENTITY_TYPE t) { type = t; }
	void setRenderOrder(entityNS::RENDER_ORDER o) { renderOrder = o; }
	void setPosX(float n) { pos.x = n; }
	void setPosY(float n) { pos.y = n; }
	void setVelX(float n) { vel.x = n; }
	void setVelY(float n) { vel.y = n; }
	void setMovX(float n) { mov.x = n; }
	void setMovY(float n) { mov.y = n; }
	void setDirect(bool d){ direct = d; }
	void setChange(bool s) { stateChanged = s; }
	void setPartnerX(int x) { partnerX = x; }
	void setPartnerY(int y) { partnerY = y; }
	void setAnim(float a) { animInterval = a; }
	void setWarp(float i) { warpInterval = i; }
	void setAction(bool b) { action = b; }
	//反応を初期化
	void resetResponse() { response = 0; FOR(entityNS::RES_CLEAR){ responseVC[i].x = 0; responseVC[i].y = 0; } }
	//ワープ相手を設定
	void setpartner(Entity *e) {partnerX = e->ChipCX(); partnerY = e->ChipCY();	}

	//描画順を指定
	bool isRenderOrder(entityNS::RENDER_ORDER ro){ return renderOrder == ro; }
	bool inChip(int x, int y) { return ((ChipCX() == x) && (ChipCY() == y)); }
	//マップチップに合わせる
	void setCX() { pos.x = (float)CHIP(ChipCX() + 0.5f); }
	void setCY() { pos.y = (float)CHIP_D(ChipCY() + 0.5f); }
	//判定をぎりぎりに抑える
	void setLeft(bool b) { pos.x = (float)CHIP(ChipCX()) + edgeX - (b ? marginX : 0); }
	void setRight(bool b) { pos.x = (float)CHIP(ChipCX() + 1) - edgeX + (b ? marginX : 0); }
	void setTop(bool b) { pos.y = (float)CHIP_D(ChipCY()) + edgeY - (b ? marginY : 0); }
	void setBottom(bool b) { pos.y = (float)CHIP_D(ChipCY() + 1) - edgeY + (b ? marginY : 0); }
};


#endif