#include "game.h"

//コンストラクタ
Game::Game(){
	graphics = NULL;
	input = NULL;
	pause = false;
	fps = 100;
	fpsOn = false;
	initialized = false;
}

//デストラクタ
Game::~Game(){
	deleteAll();
}

//メッセージ取得
LRESULT Game::messageHandler(HWND hw, UINT msg, WPARAM wp, LPARAM lp){
	if (initialized){
		switch (msg){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			input->keyDown(wp);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:
			input->keyUp(wp);
			return 0;
		case WM_CHAR:
			input->keyIn(wp);
			return 0;
		}
	}
	return DefWindowProc(hw, msg, wp, lp);
}

//初期化
void Game::initialize(HWND hw){
	hwnd = hw;

	//グラフィックス初期化
	graphics = new Graphics();
	graphics->initialize(hwnd, WINDOW_W, WINDOW_H);

	//入力初期化
	input = new Input();
	input->initialize(hwnd);

	//汎用フォント初期化
	text = new Text();
	if (text->initialize(
		graphics, gameNS::POINT_SIZE,
		false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL, "Error initializing font"));
	text->setFontColor(gameNS::FONT_COLOR);

	//タイマー初期化
	if (QueryPerformanceFrequency(&timeFreq) == false)
		throw(GameError(gameErrorNS::FATAL, "Error initializing timer."));
	QueryPerformanceCounter(&timeStart);
	initialized = true;
}

//実行ループ
void Game::run(HWND hw){
	if (graphics == NULL) return;

	//実行時間を調べる
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) /
		(float)timeFreq.QuadPart;

	//短すぎるなら待つ
	if (frameTime < FRAME_TIME_MIN){
		sleep = (DWORD)((FRAME_TIME_MIN - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleep);
		timeEndPeriod(1);
		return;
	}
	//fpsを操作
	if (frameTime > 0.0) fps = (fps*0.99f) + (0.01f / frameTime);

	//長すぎるなら切り捨てる
	if (frameTime > FRAME_TIME_MAX) frameTime = FRAME_TIME_MAX;
	timeStart = timeEnd;

	//情報を更新
	if (!pause){
		update();
	}

	//描画
	renderGame();

	//入力を初期化
	input->clear(inputNS::KEYS_PRESSED);
}

//描画
void Game::renderGame(){
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];

	//描画開始
	if (SUCCEEDED(graphics->beginScene())){
		//描画
		render();
		//デバッグ情報を表示
		graphics->spriteBegin();
		if (fpsOn){
			_snprintf_s(buffer, BUF_SIZE, "FPS : %3.3f", fps);
			text->print(buffer, WINDOW_W - gameNS::DBG_MAR_X, 0, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
		}
		if (cheat1){
			text->print("スゴイデブ", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
		}
		if (cheat2){
			text->print("ハンテイ", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y * 2, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
		}
		//描画終了
		graphics->spriteEnd();
		graphics->endScene();
	}
	//デバイス損失を確認
	handleLostGraphicsDevice();
	//バッファに表示
	graphics->showBackBuffer();
}


//デバイスが壊れたか判断する
void Game::handleLostGraphicsDevice(){
	hr = graphics->getDeviceState();
	if (FAILED(hr)){
		//ただの損失なら待つ
		if (hr == D3DERR_DEVICELOST){
			Sleep(100);
			return;
		}
		//修復不可能なら再起動
		else if (hr == D3DERR_DEVICENOTRESET){
			releaseAll();
			hr = graphics->reset();
			if (FAILED(hr)) return;
			resetAll();
		}
		else return;

	}
}

void Game::releaseAll(){
	SAFE_ON_LOST_DEVICE(text);
}

void Game::resetAll(){
	SAFE_ON_RESET_DEVICE(text);
}

void Game::deleteAll(){
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	SAFE_DELETE(text);
	initialized = false;
}