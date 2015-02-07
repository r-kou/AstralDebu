#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Mmsystem.h>
#include "const.h"
#include "gameerror.h"
#include "input.h"
#include "graphics.h"
#include "text.h"
#include "audio.h"

namespace gameNS {
	const std::string FONT = "Arial";
	const int POINT_SIZE = 14;
	const ARGB FONT_COLOR = graphicsNS::WHITE;
	const int DBG_MAR_X = 100;
	const int DBG_MAR_Y = 16;
}

class Game {
protected:
	//グラフィックスとか
	Graphics *graphics;
	Input *input;
	Text *text;
	Audio *audio;
	//ウィンドウハンドラ
	HWND hwnd;
	HRESULT hr;
	//時間管理
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	float frameTime;
	DWORD sleep;
	float fps;
	bool fpsOn;
	//画面停止
	bool pause;
	//初期化済み
	bool initialized;
	//デバッグコマンド
	std::string command;
	bool cheat1, cheat2, cheat3;

public:
	//コンストラクタ
	Game();
	//デストラクタ
	virtual ~Game();
	//メッセージハンドラ
	LRESULT messageHandler(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	//初期化
	virtual void initialize(HWND hw);
	//実行ループ
	virtual void run(HWND hw);
	//描画
	virtual void renderGame();
	//デバイスが壊れたか判断する
	virtual void handleLostGraphicsDevice();
	//ポインタの解放とか
	virtual void releaseAll();
	virtual void resetAll();
	virtual void deleteAll();

	//getter
	Graphics* getGraphics() { return graphics; }
	Audio* getAudio() { return audio; }
	Input* getInput() { return input; }
	//終了メッセージ
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }
	//仮想関数
	virtual void update() = 0;
	virtual void render() = 0;
};
#endif
