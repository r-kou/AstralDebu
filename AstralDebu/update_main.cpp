#include "astraldebu.h"

using namespace astralNS;

//コマンド読み込み
void AstralDebu::readCommand(){
	std::string str = input->getTextIn();

	//地上で素手ならジャンプできる
	//チート時はいつでも可能
	if (input->isKeyPressed('Z')){
		if ((cheat1) || ((debu->getState() == entityNS::ST_STAND) && (!debu->getHold()))){
			debu->setVelY(debuNS::VEL_JUMP);
			subLife(10);
		}
	}

	//アイテム取得は長いので関数に渡す
	handleObject();

	//やり直し
	if (input->isKeyPressed('Q')) life = 0;

	//メニュー表示
	if (input->isKeyPressed(VK_SPACE)) {
		menu = true;
		count = 0;
		audio->playCue(audioNS::OK);
	}

	//以下チートコマンド
	//クリア
	if (input->isKeyPressed('W') && (cheat1)){
		debu->setState(entityNS::ST_CLEAR);
		debu->setVelX(0.0f);
		debu->setVelY(0.0f);
	}
	//爆発する
	if (input->isKeyPressed('E') && (cheat3)){
		int tmp = getEmptyIndex();
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Blast(true);
		if (!object[tmp]->initialize(this, &bombT, debu->ChipCX(), debu->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
	}
	//デブがすごくなる
	if (str == "tabata") {
		cheat1 = !cheat1;
		input->clearTextIn();
	}
	//判定が見える
	else if (str == "atari") {
		cheat2 = !cheat2;
		input->clearTextIn();
	}
	//爆発する
	else if (str == "tuyoi"){
		cheat3 = !cheat3;
		input->clearTextIn();
	}
}

//オブジェクトの取得判定
void AstralDebu::handleObject(){
	int exist = -1;

	//立ってる時かハンマー所持時だけ有効
	if ((debu->getState() != entityNS::ST_STAND) && (debu->getState() != entityNS::ST_HAMMER)) return;
	//地形には何もできない
	if (map[getCursorChipX()][getCursorChipY()])  return;

	//存在しているオブジェクトを見る
	ALL_OBJ{
		if ((i != objHolded) &&
		(object[i]->getType() != entityNS::TY_LADDER) &&
		(object[i]->ChipCX() == getCursorChipX()) &&
		(object[i]->ChipCY() == getCursorChipY()) &&
		canTouch(object[i])) {
			if (canHold(object[i])) exist = i;
			else return;
		}
	}

		if (input->isKeyPressed('X')){
		if (debu->getHold()){
			if (exist < 0) putObject();
		}
		else{
			if (exist >= 0) holdObject(exist);
		}
		}
	if (input->isKeyPressed('C')){
		if (debu->getHold()){
			if (exist < 0) throwObject();
		}
		else{
			if (exist >= 0) pushObject(exist);
		}
	}
}

//オブジェクトを掴む
void AstralDebu::holdObject(int i){
	Entity *e = object[i];

	if (canEat(e)){
		//肉は20回復 超にくは全回復
		if (e->getType() == entityNS::TY_MEAT) {
			addLife(30);
			audio->playCue(audioNS::EAT_1);
		}
		else{
			life = 100;
			audio->playCue(audioNS::EAT_2);
		}
		e->setState(entityNS::ST_EMPTY);
		e->setType(entityNS::TY_NONE);
	}
	else {
		objHolded = i;
		debu->setHold(true);

		e->setVelX(0.0f);
		e->setVelY(0.0f);
		moveHold(i);

		if (e->getType() == entityNS::TY_HAMMER) ((Hammer*)e)->setHold(debu);
		else e->setState(entityNS::ST_LOCK);

		e->setRenderOrder(entityNS::RO_HOLD);

		switch (e->getType()){
		case entityNS::TY_WOOD_BOX:
		case entityNS::TY_BOMB_BOX:
		case entityNS::TY_AIR_BOX:
			subLife(5);
			break;
		case entityNS::TY_STEEL_BOX:
		case entityNS::TY_BOMB:
		case entityNS::TY_HIBOMB:
			subLife(10);
			break;
		case entityNS::TY_LEAD_BOX:
		case entityNS::TY_HIBOMB_BOX:
			subLife(20);
			break;
		case entityNS::TY_HAMMER:
			//ハンマーは持つだけでは体力減らない
			break;
		}
	}
}

//オブジェクトを放す
void AstralDebu::putObject(){
	Entity *e = object[objHolded];

	objHolded = -1;
	debu->setHold(false);
	debu->setState(entityNS::ST_STAND);

	if (e->getType() == entityNS::TY_HAMMER){
		if (e->getDirect()) e->setPosX((debu->ChipCX() - 0.5f) * CHIP_SIZE);
		else e->setPosX((debu->ChipCX() + 1.5f) * CHIP_SIZE);
	}
	else {
		e->setPosX((getCursorChipX() + 0.5f) * CHIP_SIZE);
	}
	e->setPosY((getCursorChipY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::ST_STAND);
	e->setRenderOrder(entityNS::RO_OBJECT);
	e->setEdge();
	if ((e->ChipCY() == MAP_ROW - 1) || (map[e->ChipCX()][e->ChipCY() + 1]) != 0) e->playPut();

}

//オブジェクトを押す
void AstralDebu::pushObject(int exist){
	int cursorNX = 0;
	Entity *e = object[exist];

	//ハンマーとにくは押せない
	if ((e->getType() == entityNS::TY_HAMMER) || (canEat(e))) return;

	if (debu->getDirect()) {
		if (debu->ChipCX() > 1) cursorNX = debu->ChipCX() - 2;
		else return;
	}
	else {
		if (debu->ChipCX() < MAP_COL - 2) cursorNX = debu->ChipCX() + 2;
		else return;
	}

	//押す先に何かあったら不可
	if (map[cursorNX][getCursorChipY()]) return;
	ALL_OBJ{
		if (canTouch(object[i])&&
		(object[i]->getType() != entityNS::TY_LADDER) &&
		(object[i]->ChipCX() == cursorNX) &&
		(object[i]->ChipCY() == getCursorChipY())) return;
	}

	e->setPosX((cursorNX + 0.5f) * CHIP_SIZE);
	e->setPosY((getCursorChipY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::ST_STAND);
	e->setEdge();
	e->playPut();

	//体力減少
	switch (e->getType()){
	case entityNS::TY_WOOD_BOX:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_AIR_BOX:
		//木箱は体力減少なし
		break;
	case entityNS::TY_STEEL_BOX:
	case entityNS::TY_BOMB:
	case entityNS::TY_HIBOMB:
		subLife(5);
		break;
	case entityNS::TY_LEAD_BOX:
	case entityNS::TY_HIBOMB_BOX:
		subLife(10);
		break;
	}
}

//オブジェクトを投げる
void AstralDebu::throwObject(){
	Entity *e = object[objHolded];

	objHolded = -1;
	debu->setHold(false);
	debu->setState(entityNS::ST_STAND);

	e->setPosX((getCursorChipX() + 0.5f) * CHIP_SIZE);
	e->setPosY((getCursorChipY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	if (debu->getDirect()) e->setVelX(-VEL_THROW);
	else e->setVelX(VEL_THROW);
	e->setVelY(0.0f);
	e->setState(entityNS::ST_KNOCK);
	e->setEdge();
	e->setRenderOrder(entityNS::RO_OBJECT);

	//体力減少
	switch (e->getType()){
	case entityNS::TY_WOOD_BOX:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_AIR_BOX:
		subLife(5);
		audio->playCue(audioNS::THROW_WOOD);
		break;
	case entityNS::TY_STEEL_BOX:
	case entityNS::TY_BOMB:
	case entityNS::TY_HIBOMB:
		subLife(10);
		audio->playCue(audioNS::THROW_STEEL);
		break;
	case entityNS::TY_LEAD_BOX:
	case entityNS::TY_HIBOMB_BOX:
		subLife(20);
		audio->playCue(audioNS::THROW_LEAD);
		break;
	case entityNS::TY_HAMMER:
		subLife(40);
		audio->playCue(audioNS::THROW_HAMMER);
		break;
	}
}

//所持オブジェクトをデブの前に移動
void AstralDebu::moveHold(int i){
	Entity *e = object[i];

	if (e->getType() == entityNS::TY_HAMMER) {
		debu->setHammer(debu->getPosX() >= e->getPosX());
	}
	else {
		if (debu->getDirect()) e->setPosX(debu->getPosX() - HOLD_MAR_X);
		else e->setPosX(debu->getPosX() + HOLD_MAR_X);
		e->setPosY(debu->getPosY() - HOLD_MAR_Y);
	}

}

//オブジェクトの消滅処理
void AstralDebu::deadObject(int i){
	Entity *e = object[i];
	int x = e->ChipCX(), y = e->ChipCY();

	switch (e->getType()){
	case entityNS::TY_BOMB:
	case entityNS::TY_BOMB_BOX:
	case entityNS::TY_MISSILE:
	case entityNS::TY_MINE:
		//消去してから爆風を生成する
		SAFE_DELETE(e);
		object[i] = new Blast(false);
		if (!object[i]->initialize(this, &bombT, x, y))
			throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
		audio->playCue(audioNS::BLAST1);
		break;
	case entityNS::TY_HIBOMB:
	case entityNS::TY_HIBOMB_BOX:
		//消去してからすごい爆風を生成する
		SAFE_DELETE(e);
		object[i] = new Blast(true);
		if (!object[i]->initialize(this, &bombT, x, y))
			throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
		audio->playCue(audioNS::BLAST2);
		break;
	default:
		//消去 配列からの削除はしない
		e->setState(entityNS::ST_EMPTY);
		e->setType(entityNS::TY_NONE);
		break;
	}
}

//オブジェクトの特殊行動
void AstralDebu::actionObject(int i){
	Entity *e = object[i];
	int tmp;
	switch (e->getType()){
	case entityNS::TY_ENEMY_3:
		//銃弾生成
		e->setAction(false);
		tmp = getEmptyIndex();
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Bullet();
		if (!object[tmp]->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
		object[tmp]->setDirect(e->getDirect());
		object[tmp]->setState(entityNS::ST_KNOCK);
		break;
	case entityNS::TY_ENEMY_4:
		break;
	case entityNS::TY_ENEMY_5:
		//ミサイル生成
		e->setAction(false);
		tmp = getEmptyIndex();
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Missile();
		if (!object[tmp]->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "オブジェクトの初期化に失敗しました"));
		object[tmp]->setDirect(e->getDirect());
		object[tmp]->setState(entityNS::ST_KNOCK);
		break;
	}
}
