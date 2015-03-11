#include "astraldebu.h"
using namespace astralNS;
using namespace entityNS;


//オブジェクトの追加
void AstralDebu::addObject(Entity *e, Texture &t, int i, int j){
	if (objMax >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING,"オブジェクトの数が上限に達しました"));
	map[i][j] = 0;
	setObject(objMax, e);
	if (!e->initialize(this, &t, i, j))
		throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
	objMax++;
}

//敵の追加　デブの位置を入れる
void AstralDebu::addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j){
	if (objMax >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "オブジェクトの数が上限に達しました"));
	map[i][j] = 0;
	setObject(objMax, e);
	if (!e->initialize(this, &t, d, i, j))
		throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
	objMax++;
}

//ワープの追加
void AstralDebu::addWarp(int i, int j){
	Entity *ei = getObject(i),*ej = getObject(j);
	ei->setpartner(ej);
	ej->setpartner(ei);
}

//オブジェクトが背景タイプか判定
bool AstralDebu::isBase(Entity *e){
	switch (e->getType()){
	case TY_GOAL:
	case TY_RED_WARP:
	case TY_GREEN_WARP:
	case TY_YELLOW_WARP:
	case TY_LADDER:
	case TY_BULLET:
	case TY_MISSILE:
		return true;
	}
	return false;
}

//オブジェクトが持てるか判定
bool AstralDebu::isHoldable(Entity *e){
	//空気箱は空中でもつかめる
	if (e->getType() == TY_AIR_BOX) return isTouchable(e);
	if (e->getState() != ST_STAND) return false;
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_FRAME_BOX:
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_MEAT:
	case TY_HIMEAT:
	case TY_HAMMER:
		return isTouchable(e);
		break;
	default:
		return false;
		break;
	}
}

//オブジェクトが肉か判定
bool AstralDebu::isFood(Entity *e){
	switch (e->getType()){
	case TY_MEAT:
	case TY_HIMEAT:
		return true;
		break;
	default:
		return false;
		break;
	}
}

//オブジェクトが接触可能か判定
bool AstralDebu::isTouchable(Entity *e){
	if ((e->getState() == ST_LOCK) ||
		(e->getState() == ST_CLEAR) ||
		(e->getState() == ST_DEAD)) return false;
	return isMovable(e);
}

//オブジェクトを描画するか判定
bool AstralDebu::isMovable(Entity *e){
	if ((e->getState() == ST_EMPTY) ||
		(e->getState() == ST_LOCK) ||
		(e->getType() == TY_NONE)) return false;
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
	int empty = objMax;
	if (objMax > OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "オブジェクトの数が上限に達しました"));
	ALL_OBJ {
		Entity *tmp = getObject(i);
		if ((tmp->getType() == TY_NONE) && (tmp->getState() == ST_EMPTY)) {
			empty = i;
			break;
		}
	}
	if (empty == objMax) objMax++;
	return empty;
}

//bgm再生
void AstralDebu::playBgm(){
	std::string playBgm;
	if (bgm) return;

	if (stage == 0) playBgm = audioNS::BGM_TITLE;
	else if (stage <= 10) playBgm = audioNS::BGM1;
	else if (stage <= 20) playBgm = audioNS::BGM2;
	else if (stage <= 30) playBgm = audioNS::BGM3;
	else playBgm = audioNS::BGM4;

	if (audio->isPlaying(playBgm.c_str())) return;
	audio->playCue(playBgm.c_str());

	bgm = true;
}

//bgm停止
void AstralDebu::stopBgm(){
	if (!bgm) return;

	if (audio->isPlaying(audioNS::BGM_TITLE)) audio->stopCue(audioNS::BGM_TITLE);
	if (audio->isPlaying(audioNS::BGM1)) audio->stopCue(audioNS::BGM1);
	if (audio->isPlaying(audioNS::BGM2)) audio->stopCue(audioNS::BGM2);
	if (audio->isPlaying(audioNS::BGM3)) audio->stopCue(audioNS::BGM3);
	if (audio->isPlaying(audioNS::BGM4)) audio->stopCue(audioNS::BGM4);

	bgm = false;
}

//bgm変更
void AstralDebu::changeBgm(int m){
	switch (m){
	case 0:
		if (audio->isPlaying(audioNS::BGM_TITLE)) break;
		stopBgm();
		playBgm();
		break;
	case 1:
		if (audio->isPlaying(audioNS::BGM1)) break;
		stopBgm();
		playBgm();
		break;
	case 2:
		if (audio->isPlaying(audioNS::BGM2)) break;
		stopBgm();
		playBgm();
		break;
	case 3:
		if (audio->isPlaying(audioNS::BGM3)) break;
		stopBgm();
		playBgm();
		break;
	case 4:
		if (audio->isPlaying(audioNS::BGM4)) break;
		stopBgm();
		playBgm();
		break;
	}
}

//オブジェクトデータとかを初期化
void AstralDebu::resetObject(){
	//古いオブジェクトをすべて消去
	ALL_OBJ	SAFE_DELETE(object[i]);
	objMax = 0;
	objHolded = -1;
	warpRed = -1;
	warpGreen = -1;
	warpYellow = -1;
	FOR_2(MAP_COL, MAP_ROW){
		map[i][j] = 0;
	}
}

//doubleを整数にして文字列化
std::string AstralDebu::doubleToString(double f){
	int i = (int)(f * 100);
	return std::to_string(i);
}

//マップチップのデコード
short AstralDebu::decodeChip(short c, int i, int j) {
	short first, second, third;
	first = ((i*(j + 1)) << 4);
	second = ((i + 1) * 11);
	third = c - first;
	if ((third % second)!=0) throw(GameError(gameErrorNS::FATAL, "マップデータの読み込みに失敗しました\nマップデータが壊れている可能性があります"));
	return third / second;
}


//カーソルのX位置を返す
int AstralDebu::getCursorChipX(Entity *e){
	if (e->getDirect()) {
		if (e->ChipCX() > 0) return e->ChipCX() - 1;
		return 0;
	}
	else {
		if (e->ChipCX() < MAP_COL - 1) return e->ChipCX() + 1;
		return MAP_COL - 1;

	}
}


//カーソルのY位置を返す
int AstralDebu::getCursorChipY(Entity *e){
	return e->ChipCY();
}


//存在しているオブジェクトを返す
int AstralDebu::getCursorObject(int cx,int cy,bool b){
	//地形には何もできない
	if (map[cx][cy])  return -2;

	//存在しているオブジェクトを見る
	ALL_OBJ{
		if (i == objHolded) continue;
		Entity *tmp = getObject(i);
		if (!isBase(tmp) &&
			isTouchable(tmp) &&
			(tmp->inChip(cx, cy))) {
				//trueなら持てるもののみ、falseなら常にiを返す
				if ((!b)||(isHoldable(tmp))) return i;
				else return -2;
		}
	}

	return -1;
}

//オブジェクトを返す
Entity* AstralDebu::getObject(int i){
	if (i >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "オブジェクトの数が上限に達しました"));
	Entity *obj = object[i];
	if (obj == NULL)
		throw(GameError(gameErrorNS::WARNING, "存在しないオブジェクトが参照されました"));
	return obj;
}

//オブジェクトを保存する
void AstralDebu::setObject(int i,Entity *e){
	if (i >= OBJ_SIZE)
		throw(GameError(gameErrorNS::WARNING, "オブジェクトの数が上限に達しました"));
	object[i] = e;
}