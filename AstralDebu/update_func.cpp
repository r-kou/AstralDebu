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
	if (e->getState() != ST_STAND) return false;
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_BOMB:
	case TY_HIBOMB:
	case TY_MEAT:
	case TY_HIMEAT:
	case TY_HAMMER:
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
bool AstralDebu::canTouch(Entity *e){
	if ((e->getState() == ST_LOCK) ||
		(e->getState() == ST_CLEAR) ||
		(e->getState() == ST_DEAD)) return false;
	return canMove(e);
}

//オブジェクトを描画するか判定
bool AstralDebu::canMove(Entity *e){
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
	int tmp = objMax;
	ALL_OBJ if ((object[i]->getType() == TY_NONE) && (object[i]->getState() == ST_EMPTY)) {
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
	if ((third % second)!=0) throw(GameError(gameErrorNS::FATAL, "マップデータが壊れています"));
	return third / second;
}
