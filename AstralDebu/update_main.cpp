#include "astraldebu.h"

using namespace astralNS;


//コマンド読み込み
void AstralDebu::readCommand(){
	std::string str = input->getTextIn();

	//地上で素手ならジャンプできる
	//チート時はいつでも可能
	if (input->isKeyPressed('Z')){
		if ((cheat1) || ((debu->getState() == entityNS::STAND) && (!debu->getHold()))){
			debu->setVelY(debuNS::VEL_JUMP);
			subLife(10);
		}
	}

	//アイテム取得は長いので関数に渡す
	handleObject();

	//やり直し
	if (input->isKeyPressed('Q')) life = 0;

	//以下チートコマンド
	//クリア
	if (input->isKeyPressed('W') && (cheat1)){
		debu->setState(entityNS::CLEAR);
		debu->setVelX(0.0f);
		debu->setVelY(0.0f);
	}
	//爆発する
	if (input->isKeyPressed('E') && (cheat3)){
		int tmp = getEmptyIndex();
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Blast(true);
		if (!object[tmp]->initialize(this, &bombT, debu->ChipCX(), debu->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
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

//オブジェクトの追加
void AstralDebu::addObject(Entity *e, Texture &t, int i, int j){
	map[i][j] = 0;
	object[objMax] = e;
	if (!e->initialize(this, &t, i, j))
		throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
	objMax++;
}

//敵の追加　デブの位置を入れる
void AstralDebu::addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j){
	map[i][j] = 0;
	object[objMax] = e;
	if (!e->initialize(this, &t, d, i, j))
		throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
	objMax++;
}

//ワープの追加
void AstralDebu::addWarp(int i, int j){
	object[i]->setPartnerX(object[j]->ChipCX());
	object[i]->setPartnerY(object[j]->ChipCY());
	object[j]->setPartnerX(object[i]->ChipCX());
	object[j]->setPartnerY(object[i]->ChipCY());
}

//オブジェクトの取得判定
void AstralDebu::handleObject(){
	int exist = -1;

	//立ってる時かハンマー所持時だけ有効
	if ((debu->getState() != entityNS::STAND) && (debu->getState() != entityNS::HOLD_HAMMER)) return;
	//地形には何もできない
	if ((map[getCursorChipX()][getCursorChipY()] != 0) && (map[getCursorChipX()][getCursorChipY()] != CHIP_LADDER))  return;

	//存在しているオブジェクトを見る
	ALL_OBJ{
		if ((i != objHolded) &&
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
		if (e->getType() == entityNS::MEAT) addLife(30);
		else life = 100;
		e->setState(entityNS::EMPTY);
		e->setType(entityNS::NONE);
	}
	else {
		objHolded = i;
		debu->setHold(true);

		e->setVelX(0.0f);
		e->setVelY(0.0f);
		moveHold(i);

		if (e->getType() == entityNS::HAMMER) ((Hammer*)e)->setHold(debu);
		else e->setState(entityNS::LOCK);

		switch (e->getType()){
		case entityNS::WOOD_BOX:
		case entityNS::BOMB_BOX:
		case entityNS::AIR_BOX:
			subLife(5);
			break;
		case entityNS::STEEL_BOX:
		case entityNS::BOMB:
		case entityNS::HIBOMB:
			subLife(10);
			break;
		case entityNS::LEAD_BOX:
		case entityNS::HIBOMB_BOX:
			subLife(20);
			break;
		case entityNS::HAMMER:
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
	debu->setState(entityNS::STAND);

	if (e->getType() == entityNS::HAMMER){
		if (e->getDirect()) e->setPosX((debu->ChipCX() - 0.5f) * CHIP_SIZE);
		else e->setPosX((debu->ChipCX() + 1.5f) * CHIP_SIZE);
	}
	else {
		e->setPosX((getCursorChipX() + 0.5f) * CHIP_SIZE);
	}
	e->setPosY((getCursorChipY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::STAND);
	e->setEdge();

}

//オブジェクトを押す
void AstralDebu::pushObject(int exist){
	int cursorNX = 0;
	Entity *e = object[exist];

	//ハンマーとにくは押せない
	if ((e->getType() == entityNS::HAMMER) || (canEat(e))) return;

	if (debu->getDirect()) {
		if (debu->ChipCX() > 1) cursorNX = debu->ChipCX() - 2;
		else return;
	}
	else {
		if (debu->ChipCX() < MAP_COL - 2) cursorNX = debu->ChipCX() + 2;
		else return;
	}

	//押す先に何かあったら不可
	if ((map[cursorNX][getCursorChipY()] != 0) && (map[cursorNX][getCursorChipY()] != CHIP_LADDER)) return;
	ALL_OBJ{
		if ((object[i]->ChipCX() == cursorNX) &&
		(object[i]->ChipCY() == getCursorChipY()) &&
		canTouch(object[i])) return;
	}

	e->setPosX((cursorNX + 0.5f) * CHIP_SIZE);
	e->setPosY((getCursorChipY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::STAND);
	e->setEdge();

	//体力減少
	switch (e->getType()){
	case entityNS::WOOD_BOX:
	case entityNS::BOMB_BOX:
	case entityNS::AIR_BOX:
		//木箱は体力減少なし
		break;
	case entityNS::STEEL_BOX:
	case entityNS::BOMB:
	case entityNS::HIBOMB:
		subLife(5);
		break;
	case entityNS::LEAD_BOX:
	case entityNS::HIBOMB_BOX:
		subLife(10);
		break;
	}
}

//オブジェクトを投げる
void AstralDebu::throwObject(){
	Entity *e = object[objHolded];

	objHolded = -1;
	debu->setHold(false);
	debu->setState(entityNS::STAND);

	e->setPosX((getCursorChipX() + 0.5f) * CHIP_SIZE);
	e->setPosY((getCursorChipY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	if (debu->getDirect()) e->setVelX(-VEL_THROW);
	else e->setVelX(VEL_THROW);
	e->setVelY(0.0f);
	e->setState(entityNS::KNOCK);
	e->setEdge();

	//体力減少
	switch (e->getType()){
	case entityNS::WOOD_BOX:
	case entityNS::BOMB_BOX:
	case entityNS::AIR_BOX:
		subLife(5);
		break;
	case entityNS::STEEL_BOX:
	case entityNS::BOMB:
	case entityNS::HIBOMB:
		subLife(10);
		break;
	case entityNS::LEAD_BOX:
	case entityNS::HIBOMB_BOX:
		subLife(20);
		break;
	case entityNS::HAMMER:
		subLife(40);
		break;
	}
}

//所持オブジェクトをデブの前に移動
void AstralDebu::moveHold(int i){
	Entity *e = object[i];

	if (e->getType() == entityNS::HAMMER) {
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
	case entityNS::BOMB:
	case entityNS::BOMB_BOX:
	case entityNS::MISSILE:
	case entityNS::MINE:
		//消去してから爆風を生成する
		SAFE_DELETE(e);
		object[i] = new Blast(false);
		if (!object[i]->initialize(this, &bombT, x, y))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	case entityNS::HIBOMB:
	case entityNS::HIBOMB_BOX:
		//消去してからすごい爆風を生成する
		SAFE_DELETE(e);
		object[i] = new Blast(true);
		if (!object[i]->initialize(this, &bombT, x, y))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	default:
		//消去 配列からの削除はしない
		e->setState(entityNS::EMPTY);
		e->setType(entityNS::NONE);
		break;
	}
}

//オブジェクトの特殊行動
void AstralDebu::actionObject(int i){
	Entity *e = object[i];
	int tmp;
	switch (e->getType()){
	case entityNS::EN_3:
		//銃弾生成
		e->setAction(false);
		tmp = getEmptyIndex();
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Bullet();
		if (!object[tmp]->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		object[tmp]->setDirect(e->getDirect());
		object[tmp]->setState(entityNS::KNOCK);
		break;
	case entityNS::EN_4:
		break;
	case entityNS::EN_5:
		//ミサイル生成
		e->setAction(false);
		tmp = getEmptyIndex();
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Missile();
		if (!object[tmp]->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		object[tmp]->setDirect(e->getDirect());
		object[tmp]->setState(entityNS::KNOCK);
		break;
	}
}
