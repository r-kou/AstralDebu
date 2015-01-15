#ifndef _ASTRAL_H
#define _ASTRAL_H
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <fstream>
#include "game.h"
#include "texture.h"
#include "Image.h"
#include "text.h"
#include "entity.h"
#include "debu.h"
#include "warp.h"
#include "box.h"
#include "bomb.h"
#include "enemy.h"
#include "etc.h"
#include "blast.h"

namespace astralNS {
	const int OBJ_SIZE = 64;

	const int IMG_COL_CHIP = 10;
	const int IMG_COL_DEBU = 7;
	const int IMG_COL_BLAST = 6;
	const int IMG_COL_ENEMY = 7;

	const int CHIP_ROCK = 5;
	const int CHIP_BOX_W = 20;
	const int CHIP_BOX_S = 21;
	const int CHIP_BOX_L = 22;
	const int CHIP_BOX_B = 23;
	const int CHIP_BOX_H = 24;
	const int CHIP_BOMB = 25;
	const int CHIP_HIBOMB = 26;
	const int CHIP_HAMMER = 27;
	const int CHIP_LADDER = 30;
	const int CHIP_MEAT = 31;
	const int CHIP_HIMEAT = 32;
	const int CHIP_WARP_R = 35;
	const int CHIP_WARP_G = 36;
	const int CHIP_WARP_Y = 37;
	const int CHIP_WARP_E = 38;
	const int CHIP_CURSOR = 40;
	const int CHIP_GOAL = 41;

	const float HOLD_MAR_X = 8;
	const float HOLD_MAR_Y = 16;
	const float VEL_THROW = 300;

	const int LIFE_MAR_X = 120;
	const int LIFE_MAR_Y = 48;
	const int LIFE_LEN_X = 400;
	const int LIFE_LEN_Y = 64;
	const int WORD_MAR_X = 40;
	const int WORD_MAR_Y = 64;
	const int WORD_LEN_X = LIFE_MAR_X - WORD_MAR_X;
	const int WORD_LEN_Y = 24;
	const int DATA_MAR = 4;
	const int STG_MAR_X = 520;
	const int STG_MAR_Y = LIFE_MAR_Y;
	const int STG_LEN_X = WINDOW_W - STG_MAR_X;
	const int STG_LEN_Y = LIFE_LEN_Y;

	const enum GameState{
		S_TITLE, S_STAGE, S_MAIN, S_OVER, S_CLEAR, S_END
	};
}

class AstralDebu :public Game{
private:
	//汎用アニメカウンタ
	int count;
	//現在のステージ
	int stage;
	//読み込み判定
	bool read;
	//マップチップ
	int map[MAP_COL][MAP_ROW];
	//ゲーム状態
	astralNS::GameState state;
	//ステージクリア
	bool clear;
	//主人公
	Debu *debu;
	//オブジェクト
	Entity *object[astralNS::OBJ_SIZE];
	//現在のオブジェクト数
	int obj_num;
	//デブが保持するオブジェクト
	int obj_hold;
	//ワープ１つ目のインデックス
	int warp_r, warp_g, warp_y;

	//主人公体力
	int life, life_v;

	//画像データ
	Texture debuT, chipT, enemyT, bombT, titleT;
	//画像のみデータ
	Image chip, title;
	//フォント
	Text stageF, numberF, bigF, middleF, smallF;
	//描画用短径
	RECT rect;
	//vertex
	Vertex vtx[4];
	LP_VERTEX vertex;

	//テクスチャの初期化
	void initTexture(Texture &t, std::string file);
	//フォントの初期化
	void initFont(Text &t, int point);

	//ゲーム状態に応じて更新
	void updateTitle();
	void updateStage();
	void updateMain();
	void updateClear();
	void renderTitle();
	void renderStage();
	void renderMain();
	void renderClear();
	//ステージ読み込み
	void loadStage();
	//チップ割り当て
	void loadChip(int, int, char);
	//ステージに合わせてチップを選択
	int chipFormat(int);
	//コマンド読み込み
	void readCommand();
	//オブジェクトの追加
	void addObject(Entity *e, Texture &t, int i, int j);
	//敵の追加
	void addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j);
	//ワープの追加
	void addWarp(int i, int j);
	//オブジェクトの取得判定
	void handleObject();
	//オブジェクトを掴む
	void holdObject(int i);
	//オブジェクトを放す
	void putObject();
	//オブジェクトを押す
	void pushObject(int i);
	//オブジェクトを投げる
	void throwObject();
	//所持オブジェクトをデブの前に移動
	void moveHold(int i);
	//オブジェクトの消滅処理
	void deadObject(int i);
	//オブジェクトの特殊行動
	void actionObject(int i);

	//背景と上のを描画
	void renderBack();
	//マップチップを描画
	void renderChip();
	//ヒントとか描画
	void renderHint();
	//オブジェクトを描画
	void renderObject();

	//vertexを設定
	void setVertex(float l, float t, float r, float b, ARGB c);
	//四角を描画
	void drawQuad(float l, float t, float r, float b, ARGB c);
	//三角のvertexを描画
	void setVertexT(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//三角を描画
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//カーソルを描画
	void drawCursor();
	//デバッグ用 判定を描画
	void drawEdge();
	//ヒント用矢印を描画 横
	void drawArrowH(float cx, float cy, bool d, ARGB c);
	//ヒント用矢印を描画 縦
	void drawArrowV(float cx, float cy, bool d, ARGB c);
	//ヒント用パネルを描画
	void drawPanel(std::string str, float cx, float cy, float len, ARGB c);

	//カーソルの位置を返す
	int cursorX();
	int cursorY();
	//オブジェクトが持てるか判定
	bool canHold(Entity *e);
	//オブジェクトが肉か判定
	bool canEat(Entity *e);
	//オブジェクトが接触可能か判定
	bool canTouch(Entity *e);
	//オブジェクトが描画可能か判定
	bool canMove(Entity *e);
	//体力の増加
	void addLife(int i);
	//体力の減少
	void subLife(int i);
public:
	//コンストラクタ
	AstralDebu();
	//デストラクタ
	virtual ~AstralDebu();
	//初期化
	virtual void initialize(HWND hwnd);
	//内部の更新
	virtual void update();
	//描画
	virtual void render();
	//ポインタの解放とか
	virtual void releaseAll();
	virtual void resetAll();
	virtual void deleteAll();
};


#endif