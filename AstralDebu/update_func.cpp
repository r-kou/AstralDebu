#include "astraldebu.h"
using namespace astralNS;
using namespace entityNS;


//オブジェクトの追加
void AstralDebu::addObject(Entity *e, Texture &t, int i, int j){
	map[i][j] = 0;
	object[objMax] = e;
	if (!e->initialize(this, &t, i, j))
		throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
	objMax++;
}

//敵の追加　デブの位置を入れる
void AstralDebu::addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j){
	map[i][j] = 0;
	object[objMax] = e;
	if (!e->initialize(this, &t, d, i, j))
		throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
	objMax++;
}

//ワープの追加
void AstralDebu::addWarp(int i, int j){
	object[i]->setPartnerX(object[j]->ChipCX());
	object[i]->setPartnerY(object[j]->ChipCY());
	object[j]->setPartnerX(object[i]->ChipCX());
	object[j]->setPartnerY(object[i]->ChipCY());
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

//bgm再生
void AstralDebu::playBgm(){
	std::string playBgm;
	if (bgm) return;

	if (stage <= 10) playBgm = audioNS::BGM1;
	else if (stage <= 20) playBgm = audioNS::BGM2;
	else if (stage <= 30) playBgm = audioNS::BGM3;
	else playBgm = audioNS::BGM4;

	if (audio->isPlaying(playBgm.c_str())) return;
	audio->playCue(playBgm.c_str());

	bgm = true;
}

//bgm停止
void AstralDebu::stopBgm(){
	std::string playBgm;
	if (!bgm) return;

	if (stage == 11) playBgm = audioNS::BGM1;
	else if (stage == 21) playBgm = audioNS::BGM2;
	else if (stage == 31) playBgm = audioNS::BGM3;
	else playBgm = audioNS::BGM4;

	if (!audio->isPlaying(playBgm.c_str())) return;
	audio->stopCue(playBgm.c_str());

	bgm = false;
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