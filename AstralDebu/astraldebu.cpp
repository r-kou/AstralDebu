#include "astraldebu.h"

using namespace astralNS;

//コンストラクタ
AstralDebu::AstralDebu(){
	stage = 0;
	count = 0;
	fCount = 0;
	life = 0;
	vitalLife = 0;
	bgmVolume = 0.0;
	soundVolume = 0.0;
	timeout = 0.0f;
	read = false;
	clear = false;
	bgm = false;
	initializeFont = false;
	menu = false;
	state = S_TITLE;
	stateNumber = 0;
	clearTimeStart = 0;
	clearTimeEnd = 0;
	clearedStage = 0;
	//クリア時間を初期化
	FOR(STG_SIZE) clearTime[i] = 0;
	//マップを初期化
	FOR_2(MAP_COL, MAP_ROW){
		map[i][j] = 0;
	}
	//オブジェクトも初期化
	debu = NULL;
	ALL_OBJ object[i] = NULL;
	objMax = 0;
	objHolded = -1;
	vertex = NULL;
	cheat = false;
	cheat1 = false;
	cheat2 = false;
	cheat3 = false;
	cheat4 = false;
}

//デストラクタ
AstralDebu::~AstralDebu(){
	deleteAll();

	//フォント削除
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (!RemoveFontResourceEx(DATA(FON_FILE), FR_PRIVATE, NULL))
		throw(GameError(gameErrorNS::FATAL, "フォントデータの初期化に失敗しました"));
#endif
}

//初期化
void AstralDebu::initialize(HWND hwnd){
	Game::initialize(hwnd);

	//フォント追加
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (AddFontResourceEx(DATA(FON_FILE), FR_PRIVATE, NULL)) initializeFont = true;
#endif

	//画像テクスチャを設定
	initTexture(debuT, IMG_FILE_DEBU);
	initTexture(chipT, IMG_FILE_CHIP);
	initTexture(bombT, IMG_FILE_BLAST);
	initTexture(enemyT, IMG_FILE_ENEMY);
	initTexture(titleT, IMG_FILE_TITLE);

	//画像を設定
	if (!title.initialize(graphics, &titleT, 0, 0, 0))
		throw(GameError(gameErrorNS::FATAL, "画像データの初期化に失敗しました"));
	if (!chip.initialize(graphics, &chipT, CHIP_SIZE, CHIP_SIZE, IMG_COL_CHIP))
		throw(GameError(gameErrorNS::FATAL, "画像データの初期化に失敗しました"));

	//フォントを設定
	initFont(stageF, 80);
	initFont(numberF, 48);
	initFont(bigF, 64);
	initFont(middleF, 32);
	initFont(smallF, 16);

	//主人公を設定
	debu = new Debu();
}

//テクスチャの初期化
void AstralDebu::initTexture(Texture &t, std::string file){
	if (!t.initialize(graphics, IMG(file)))
		throw(GameError(gameErrorNS::FATAL, "画像データの初期化に失敗しました"));
}

//フォントの初期化
void AstralDebu::initFont(Text &t, int point){
	if (t.initialize(graphics, point, false, false, (initializeFont?GEN:MEI)) == false)
		throw(GameError(gameErrorNS::FATAL, "フォントの初期化に失敗しました"));
	t.setFontColor(WHITE);
}

//内部の更新
void AstralDebu::update(){
	//FPSの表示
	if (input->isKeyPressed('F')) fpsOn = !fpsOn;

	//ゲーム状態に応じて内容変更
	switch (state){
	case S_TITLE:
		updateTitle();
		break;
	case S_STAGE:
		updateStage();
		break;
	case S_MAIN:
		updateMain();
		break;
	case S_CLEAR:
		updateClear();
		break;
	}
}

//描画
void AstralDebu::render(){
	//ゲーム状態に応じて内容変更
	switch (state){
	case S_TITLE:
		renderTitle();
		break;
	case S_STAGE:
		renderStage();
		break;
	case S_MAIN:
		renderMain();
		break;
	case S_CLEAR:
		renderClear();
		break;
	}
	graphics->spriteBegin();
	if (cheat1) text->print("スゴイデブ", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	if (cheat2) text->print("ハンテイ", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y * 2, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	if (cheat3) text->print("ジバク", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y * 3, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	if (cheat4) text->print("ソクド", WINDOW_W - gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y * 4, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
	graphics->spriteEnd();
}

//ポインタの解放
void AstralDebu::releaseAll(){
	//フォントを破棄
	stageF.onLostDevice();
	numberF.onLostDevice();
	bigF.onLostDevice();
	middleF.onLostDevice();
	smallF.onLostDevice();

	//テクスチャを破棄
	debuT.onLostDevice();
	chipT.onLostDevice();
	bombT.onLostDevice();
	enemyT.onLostDevice();
	titleT.onLostDevice();

	//短径を破棄
	SAFE_RELEASE(vertex);

	Game::releaseAll();
	return;
}

//ポインタの解放
void AstralDebu::resetAll(){
	//フォントをリセット
	stageF.onResetDevice();
	numberF.onResetDevice();
	bigF.onResetDevice();
	middleF.onResetDevice();
	smallF.onResetDevice();

	//テクスチャをリセット
	debuT.onResetDevice();
	chipT.onResetDevice();
	bombT.onResetDevice();
	enemyT.onResetDevice();
	titleT.onResetDevice();

	//短径をリセット
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);

	Game::resetAll();
	return;
}


//ポインタの解放
void AstralDebu::deleteAll(){
	//リリースも実行
	releaseAll();

	//オブジェクトデータを破棄
	SAFE_DELETE(debu);
	FOR(OBJ_SIZE) SAFE_DELETE(object[i]);

	Game::deleteAll();
	return;
}