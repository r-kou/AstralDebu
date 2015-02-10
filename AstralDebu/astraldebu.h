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
	//配列の大きさ
	const int OBJ_SIZE = 128;
	const int STG_SIZE = 40;

	//各画像の横枚数
	const int IMG_COL_CHIP = 8;
	const int IMG_COL_DEBU = 7;
	const int IMG_COL_BLAST = 6;
	const int IMG_COL_ENEMY = 7;

	//画像チップの対応値
	const int CHIP_WOOD_BOX = 32;
	const int CHIP_STEEL_BOX = 33;
	const int CHIP_LEAD_BOX = 34;
	const int CHIP_BOMB_BOX = 35;
	const int CHIP_HIBOMB_BOX = 36;
	const int CHIP_AIR_BOX = 37;
	const int CHIP_FRAME_BOX = 38;
	const int CHIP_GOAST_BOX = 39;
	const int CHIP_BOMB = 40;
	const int CHIP_HIBOMB = 41;
	const int CHIP_HAMMER = 42;
	const int CHIP_MINE = 43;
	const int CHIP_MEAT = 44;
	const int CHIP_HIMEAT = 45;
	const int CHIP_LADDER = 46;
	const int CHIP_CURSOR = 47;

	//所持オブジェクトの位置補正
	const float HOLD_MAR_X = 8;
	const float HOLD_MAR_Y = 16;
	const float VEL_THROW = 300;

	//短径描画の位置
	const float LIFE_MAR_X = 120;
	const float LIFE_MAR_Y = 48;
	const float LIFE_LEN_X = 400;
	const float LIFE_LEN_Y = 64;
	const float WORD_MAR_X = 40;
	const float WORD_MAR_Y = 64;
	const float WORD_LEN_X = LIFE_MAR_X - WORD_MAR_X;
	const float WORD_LEN_Y = 24;
	const float DATA_MAR = 4;
	const float STG_MAR_X = 520;
	const float STG_MAR_Y = LIFE_MAR_Y;
	const float STG_LEN_X = WINDOW_W - STG_MAR_X;
	const float STG_LEN_Y = LIFE_LEN_Y;
	namespace mainMenuNS{
		const float FRAME = 10;
		const float SHADOW = 2.5f;
		const float MAR_X = 50;
		const float MAR_Y = 50;
		const float LEN_X = WINDOW_W - MAR_X * 2;
		const float LEN_Y = WINDOW_H - MAR_Y * 2;
		const float TEX_MAR_X = 100;
		const float TEX_MAR_Y = 100;
		const float TEX_LEN_X = WINDOW_W - TEX_MAR_X * 2;
		const float TEX_LEN_Y = WINDOW_H - TEX_MAR_Y * 2;
		const float TEX_SEP_X = TEX_LEN_X / 2;
		const float TEX_SEP_Y = TEX_LEN_Y / 8;
		const float ARR_MAR_X = TEX_MAR_X - CHIP(0.5f);
		const float ARR_MAR_Y = TEX_MAR_Y + TEX_SEP_Y * 7 + 10.0f;
		const ARGB BACK = D3DCOLOR_ARGB(255, 160, 64, 16);
		const ARGB BACK_SHADE = D3DCOLOR_ARGB(255, 144, 48, 0);
		const ARGB BACK_LIGHT = D3DCOLOR_ARGB(255, 176, 80, 32);
	}
	//色
	const ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const ARGB GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);
	const ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const ARGB STG1_BACK = D3DCOLOR_ARGB(255, 128, 224, 255);
	const ARGB STG2_BACK = D3DCOLOR_ARGB(255, 255, 255, 255);
	const ARGB STG3_BACK = D3DCOLOR_ARGB(255, 32, 48, 48);
	const ARGB STG4_BACK = D3DCOLOR_ARGB(255, 192, 255, 255);
	const ARGB LIFE_GAUGE = D3DCOLOR_ARGB(255, 224, 224, 0);
	const ARGB LIFE_VITAL_PLUS = D3DCOLOR_ARGB(255, 0, 224, 0);
	const ARGB LIFE_VITAL_MINUS = D3DCOLOR_ARGB(255, 224, 0, 0);
	const ARGB MENU_BACK = D3DCOLOR_ARGB(255, 160, 64, 16);
	const ARGB MENU_TEXT = D3DCOLOR_ARGB(255, 224, 224, 32);
	const ARGB MENU_HIDE = D3DCOLOR_ARGB(255, 96, 64, 32);
	const ARGB HINT_ARROW = D3DCOLOR_ARGB(255, 255, 255, 32);
	const ARGB HINT_PANEL = D3DCOLOR_ARGB(255, 64, 192, 64);

	const std::string FONT = "メイリオ";

	const enum GameState{
		S_TITLE, S_STAGE, S_MAIN, S_OVER, S_CLEAR, S_END
	};
}

class AstralDebu :public Game{
private:
	//汎用アニメカウンタ
	int count;
	//汎用アニメカウンタ
	float fCount;
	//現在のステージ
	int stage;
	//読み込み判定
	bool read;
	//マップチップ
	int map[MAP_COL][MAP_ROW];
	//ゲーム状態
	astralNS::GameState state;
	//タイトルとかの状態
	int stateNumber;
	//ステージクリアにかかった時間
	DWORD clearTimeStart, clearTimeEnd;
	//クリア時間一覧
	double clearTime[astralNS::STG_SIZE];
	//クリアした最後のステージ
	int clearedStage;
	//音楽の初期化とか
	bool bgm;
	//メニュー画面
	bool menu;
	//音量
	double bgmVolume, soundVolume;

	//ステージクリア
	bool clear;
	//主人公
	Debu *debu;
	//オブジェクト
	Entity *object[astralNS::OBJ_SIZE];
	//現在のオブジェクト数
	int objMax;
	//デブが保持するオブジェクト
	int objHolded;
	//ワープ１つ目のインデックス
	int warpRed, warpGreen, warpYellow;
	//チートモードを発動したか
	bool cheat;

	//主人公体力
	int life, vitalLife;

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
	void updateMenu();
	void renderTitle();
	void renderStage();
	void renderMain();
	void renderClear();
	void renderMenu();

	//セーブデータ読み込み
	void loadData();
	//セーブデータ書き込み
	void saveData();
	//クリアタイムの更新
	void updateClearTime();

	//ステージ読み込み
	void loadStage();
	//チップ割り当て
	void loadChip(int, int, short);
	//ステージに合わせてチップを選択
	int setChipImage(int);

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
	//オブジェクトデータとかを初期化
	void resetObject();

	//タイトルと背景を描画
	void renderTitleBack();
	//スタートを描画
	void renderTitleStart();
	//メニューを描画
	void renderTitleMenu();
	//ステージセレクトを描画
	void renderTitleSelect();
	//音量選択を描画
	void renderTitleVolume();
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
	void setVertexTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//三角を描画
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c);
	//三角を描画
	void drawTriangleHorizontal(float l, float t, float r, float b, bool d, ARGB c);
	//三角を描画
	void drawTriangleVertical(float l, float t, float r, float b, bool d, ARGB c);
	//カーソルを描画
	void drawCursor();
	//デバッグ用 判定を描画
	void drawEdge();
	//矢印を描画 横
	void drawArrowHorizontal(float cx, float cy, bool d, ARGB c);
	//矢印を描画 縦
	void drawArrowVertical(float cx, float cy, bool d, ARGB c);
	//ヒント用矢印を描画 横
	void drawArrowHorizontal(float l, float t, float r,float b, bool d, ARGB c);
	//ヒント用矢印を描画 縦
	void drawArrowVertical(float l, float t, float r, float b, bool d, ARGB c);
	//パネルを描画
	void drawPanel(std::string str, float cx, float cy, float len, ARGB c);
	//画像つきパネルを描画
	void drawPanel(int img,float cx,float cy,ARGB c);
	//枠付き四角を描画
	void drawFrame(float l, float t, float r, float b, float f, ARGB c, ARGB fc);
	//陰あり枠付き四角を描画
	void drawFrame(float l, float t, float r, float b, float f, float s, ARGB c, ARGB fc, ARGB fs, ARGB fl);

	//カーソルの位置を返す
	int getCursorChipX();
	int getCursorChipY();
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
	//未使用のオブジェクトを取得
	int getEmptyIndex();
	//マップチップのでコード
	short decodeChip(short c, int i, int j) {return (c - ((i*(j + 1)) << 4)) / ((i + 1) * 11);}
	//bgm再生
	void playBgm();
	//bgm停止
	void stopBgm();
	//doubleを整数にして文字列化
	std::string doubleToString(double f);
	//色を選択
	ARGB menuText(bool b) { return (b ? astralNS::MENU_TEXT : astralNS::MENU_HIDE); }

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