#include "astraldebu.h"

using namespace astralNS;

//コンストラクタ
AstralDebu::AstralDebu(){
	stage = 0;
	count = 0;
	life = 0;
	vitalLife = 0;
	read = false;
	clear = false;
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
}

//デストラクタ
AstralDebu::~AstralDebu(){
	deleteAll();
}

//初期化
void AstralDebu::initialize(HWND hwnd){
	Game::initialize(hwnd);

	//画像テクスチャを設定
	initTexture(debuT, IMG_FILE_DEBU);
	initTexture(chipT, IMG_FILE_CHIP);
	initTexture(bombT, IMG_FILE_BLAST);
	initTexture(enemyT, IMG_FILE_ENEMY);
	initTexture(titleT, IMG_FILE_TITLE);

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
	if (!t.initialize(graphics, (IMG_FILE_DIR + "\\" + file).c_str()))
		throw(GameError(gameErrorNS::FATAL, "Error initializing texture."));
}

//フォントの初期化
void AstralDebu::initFont(Text &t, int point){
	if (t.initialize(graphics, point, true, false, FONT) == false)
		throw(GameError(gameErrorNS::FATAL, "Error initializing font."));
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
void AstralDebu::setVertexTriangle(float x1, float y1, float x2, float y2, float x3, float y3, ARGB c){
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
	setVertexTriangle(x1, y1, x2, y2, x3, y3, c);
	graphics->createVertexBuffer(vtx, sizeof vtx, vertex);
	//graphcsに任せる
	graphics->drawQuad(vertex);
}

//カーソルのX位置を返す
int AstralDebu::getCursorChipX(){
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
int AstralDebu::getCursorChipY(){
	return debu->ChipCY();
}

//オブジェクトが持てるか判定
bool AstralDebu::canHold(Entity *e){
	switch (e->getType()){
	case entityNS::TY_WOOD_BOX:
	case entityNS::TY_STEEL_BOX:
	case entityNS::TY_LEAD_BOX:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_HIBOMB_BOX:
	case entityNS::TY_AIR_BOX:
	case entityNS::TY_FRAME_BOX:
	case entityNS::TY_BOMB:
	case entityNS::TY_HIBOMB:
	case entityNS::TY_MEAT:
	case entityNS::TY_HIMEAT:
	case entityNS::TY_HAMMER:
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
	case entityNS::TY_MEAT:
	case entityNS::TY_HIMEAT:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//オブジェクトが接触可能か判定
bool AstralDebu::canTouch(Entity *e){
	if ((e->getState() == entityNS::ST_LOCK) ||
		(e->getState() == entityNS::ST_CLEAR) ||
		(e->getState() == entityNS::ST_DEAD)) return false;
	return canMove(e);
}

//オブジェクトを描画するか判定
bool AstralDebu::canMove(Entity *e){
	if ((e->getState() == entityNS::ST_EMPTY) ||
		(e->getState() == entityNS::ST_LOCK) ||
		(e->getType() == entityNS::TY_NONE)) return false;
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

//未使用のオブジェクトを取得
int AstralDebu::getEmptyIndex(){
	int tmp = objMax;
	ALL_OBJ if ((object[i]->getType() == entityNS::TY_NONE) && (object[i]->getState() == entityNS::ST_EMPTY)) {
		tmp = i;
		break;
	}
	if (tmp == objMax) objMax++;
	return tmp;
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