#include "astraldebu.h"

using namespace astralNS;

//コンストラクタ
AstralDebu::AstralDebu(){
	stage = 0;
	count = 0;
	life = 0;
	life_v = 0;
	read = false;
	clear = false;
	state = S_TITLE;
	state_num = 0;
	stage_start = 0;
	stage_end = 0;
	stage_max = 0;
	//クリア時間を初期化
	FOR(STG_SIZE) clear_time[i] = 0;
	//マップを初期化
	FOR_D(MAP_COL, MAP_ROW){
		map[i][j] = 0;
	}
	//オブジェクトも初期化
	debu = NULL;
	ALL_OBJ object[i] = NULL;
	obj_num = 0;
	obj_hold = -1;
	vertex = NULL;
	cheat = false;
	cheat1 = false;
	cheat2 = false;
	cheat3 = false;
}

//デストラクタ
AstralDebu::~AstralDebu(){
	deleteAll();
}

//初期化
void AstralDebu::initialize(HWND hwnd){
	Game::initialize(hwnd);

	//画像テクスチャを設定
	initTexture(debuT, IMG_DEBU);
	initTexture(chipT, IMG_CHIP);
	initTexture(bombT, IMG_BOMB);
	initTexture(enemyT, IMG_ENEMY);
	initTexture(titleT, IMG_TITLE);

	//画像を設定
	if (!title.initialize(graphics, &titleT, 0, 0, 0))
		throw(GameError(gameErrorNS::FATAL, "Error initializing image."));
	if (!chip.initialize(graphics, &chipT, CHIP_SIZE, CHIP_SIZE, IMG_COL_CHIP))
		throw(GameError(gameErrorNS::FATAL, "Error initializing image."));

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
	if (!t.initialize(graphics, (IMG_DIR + "\\" + file).c_str()))
		throw(GameError(gameErrorNS::FATAL, "Error initializing texture."));
}

//フォントの初期化
void AstralDebu::initFont(Text &t, int point){
	if (t.initialize(graphics, point, true, false, "メイリオ") == false)
		throw(GameError(gameErrorNS::FATAL, "Error initializing font."));
	t.setFontColor(graphicsNS::WHITE);
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
}

//四角の描画
void AstralDebu::setVertex(float l, float t, float r, float b, ARGB c){
	//左上
	vtx[0].x = l;
	vtx[0].y = t;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//右上
	vtx[1].x = l + r;
	vtx[1].y = t;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//右下
	vtx[2].x = l + r;
	vtx[2].y = t + b;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//左下
	vtx[3].x = l;
	vtx[3].y = t + b;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//四角を描画
void AstralDebu::drawQuad(float l, float t, float r, float b, ARGB c){
	//短径を設定
	setVertex(l, t, r, b, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphicsに任せる
	graphics->drawQuad(vertex);
}

//三角のvertexを描画
void AstralDebu::setVertexT(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//上
	vtx[0].x = x1;
	vtx[0].y = y1;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = c;

	//右
	vtx[1].x = x2;
	vtx[1].y = y2;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = c;

	//下
	vtx[2].x = x3;
	vtx[2].y = y3;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = c;

	//上に帰ってくる
	vtx[3].x = x1;
	vtx[3].y = y1;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = c;
}

//三角を描画
void AstralDebu::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
	//短径を設定
	setVertexT(x1, y1, x2, y2, x3, y3, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphcsに任せる
	graphics->drawQuad(vertex);
}

//カーソルのX位置を返す
int AstralDebu::cursorX(){
	if (debu->getDirect()) {
		if (debu->ChipCX() > 0) return debu->ChipCX() - 1;
		return 0;
	}
	else {
		if (debu->ChipCX() < MAP_COL - 1) return debu->ChipCX() + 1;
		return MAP_COL - 1;

	}
}

//カーソルのY位置を返す
int AstralDebu::cursorY(){
	return debu->ChipCY();
}

//オブジェクトが持てるか判定
bool AstralDebu::canHold(Entity *e){
	switch (e->getType()){
	case entityNS::BOX_W:
	case entityNS::BOX_S:
	case entityNS::BOX_L:
	case entityNS::BOX_B:
	case entityNS::BOX_H:
	case entityNS::BOX_A:
	case entityNS::BOX_F:
	case entityNS::BOMB:
	case entityNS::HIBOMB:
	case entityNS::MEAT:
	case entityNS::HIMEAT:
	case entityNS::HAMMER:
		return canTouch(e);
		break;
	default:
		return false;
		break;
	}
}

//オブジェクトが肉か判定
bool AstralDebu::canEat(Entity *e){
	switch (e->getType()){
	case entityNS::MEAT:
	case entityNS::HIMEAT:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//オブジェクトが接触可能か判定
bool AstralDebu::canTouch(Entity *e){
	if ((e->getState() == entityNS::LOCK) ||
		(e->getState() == entityNS::CLEAR) ||
		(e->getState() == entityNS::DEAD)) return false;
	return canMove(e);
}

//オブジェクトを描画するか判定
bool AstralDebu::canMove(Entity *e){
	if ((e->getState() == entityNS::EMPTY) ||
		(e->getState() == entityNS::LOCK) ||
		(e->getType() == entityNS::NONE)) return false;
	return true;
}

//体力の増加
void AstralDebu::addLife(int i){
	life += i;
	if (life > 100) life = 100;
}

//体力の減少
void AstralDebu::subLife(int i){
	if (cheat1) return;
	life -= i;
	if (life < 0) life = 0;
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