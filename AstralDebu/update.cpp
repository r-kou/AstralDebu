#include "astraldebu.h"

using namespace astralNS;

//タイトル画面の更新
void AstralDebu::updateTitle(){
	count = count++ % 30;

	if (input->isKeyPressed('Z')){
		state = S_STAGE;
		stage = 17;
		read = false;
	}
}

//ステージ表示画面の更新
void AstralDebu::updateStage(){
	if (!read) loadStage();

	if (input->isKeyPressed('Z') && read){
		clear = false;
		life = 100;
		life_v = 100;
		state = S_MAIN;
	}
}

//メインの更新
void AstralDebu::updateMain(){
	if (life_v > life) life_v--;
	else if (life_v < life) life_v++;

	if (life == 0) debu->setState(entityNS::DEAD);

	if ((debu->getState() == entityNS::DEAD) && (debu->getComplete())) {
		state = S_STAGE;
		read = false;
	}

	if ((debu->getState() == entityNS::CLEAR) && (debu->getComplete())) clear = true;

	if (clear) {
		clear = false;
		stage++;
		if (stage > 21){
			state = S_CLEAR;
		}
		else {
			state = S_STAGE;
			read = false;
		}
	}

	readCommand();

	//移動
	debu->move(frameTime);
	ALL_OBJ if (canMove(object[i]))
		object[i]->move(frameTime);

	//所持オブジェクトはデブの前に移動
	if (obj_hold >= 0) moveHold(obj_hold);

	ALL_OBJ if (object[i]->getAction())
		actionObject(i);

	//地形への接触
	if (canMove(debu))
		debu->touchMap(map);
	ALL_OBJ if (canMove(object[i]))
		object[i]->touchMap(map);

	debu->resetResponse();
	ALL_OBJ object[i]->resetResponse();

	//他オブジェクトへの接触
	if (canTouch(debu))
		ALL_OBJ if (canTouch(object[i]))
		debu->touchObj(object[i]);
	ALL_OBJ_D if (canTouch(object[i]) && canTouch(object[j]))
		object[i]->touchObj(object[j]);

	//オブジェクトへの反応実行
	if (canTouch(debu)) debu->responseObj();
	ALL_OBJ if (canTouch(object[i]))
		object[i]->responseObj();

	//オブジェクト消滅処理
	ALL_OBJ if (object[i]->isDead())
		deadObject(i);

	//描画する画像を変更
	debu->changeImage();
	ALL_OBJ object[i]->changeImage();

}

//クリア画面の更新
void AstralDebu::updateClear(){

}

void AstralDebu::loadStage(){
	char buf[256];
	std::ifstream file(MAP_DIR + "\\" + MAP_NAME(stage) + MAP_EXT);

	//古いオブジェクトをすべて消去
	ALL_OBJ	SAFE_DELETE(object[i]);
	obj_num = 0;
	obj_hold = -1;
	warp_r = -1;
	warp_g = -1;
	warp_y = -1;


	if (file.fail())
		throw(GameError(gameErrorNS::FATAL, "Error reading map file"));

	for (int j = 0; j < MAP_ROW; j++){
		file >> buf;
		for (int i = 0; i < MAP_COL; i++){
			loadChip(i, j, buf[i]);
		}
	}
	read = true;
}

void AstralDebu::loadChip(int i, int j, char c){
	switch (c){
	case '0': //空白
		map[i][j] = 0;
		break;
	case '1': //壁
		map[i][j] = 1;
		break;
	case '2': //床
		map[i][j] = 2;
		break;
	case '3': //底
		map[i][j] = 3;
		break;
	case '4': //床＋底
		map[i][j] = 4;
		break;
	case '5': //次壁
		map[i][j] = 5;
		break;
	case '6': //次床
		map[i][j] = 6;
		break;
	case '7': //次底
		map[i][j] = 7;
		break;
	case '8': //次床＋底
		map[i][j] = 8;
		break;
	case '9': //岩
		addObject(new Rock(stage), chipT, i, j);
		break;
	case 'A': //木箱
		addObject(new WoodBox(), chipT, i, j);
		break;
	case 'B': //鉄箱
		addObject(new SteelBox(), chipT, i, j);
		break;
	case 'C': //鉛箱
		addObject(new LeadBox(), chipT, i, j);
		break;
	case 'D': //爆弾箱
		addObject(new BombBox(), chipT, i, j);
		break;
	case 'E': //超爆弾箱
		addObject(new HibombBox(), chipT, i, j);
		break;
	case 'F': //爆弾
		addObject(new Bomb(), chipT, i, j);
		break;
	case 'G': //超爆弾
		addObject(new Hibomb(), chipT, i, j);
		break;
	case 'H': //にく
		addObject(new Meat(), chipT, i, j);
		break;
	case 'I': //超にく
		addObject(new Himeat(), chipT, i, j);
		break;
	case 'J': //はしご
		map[i][j] = CHIP_LADDER;
		break;
	case 'K': //敵(歩くだけ)
		addEnemy(new Enemy1(), enemyT, debu, i, j);
		break;
	case 'L': //敵(こっちに向かってくる)
		addEnemy(new Enemy2(), enemyT, debu, i, j);
		break;
	case 'M': //敵(動かずに弾を撃つ)
		addEnemy(new Enemy3(), enemyT, debu, i, j);
		break;
	case 'N': //敵(箱を見ると押す)
		//addEnemy(new Enemy4(), enemyT, debu, i, j);
		break;
	//case 'O':ゼロと区別つかんので欠番
	case 'P': //敵(動かずにミサイル)
		addEnemy(new Enemy5(), enemyT, debu, i, j);
		break;
	case 'Q': //鉄球
		addObject(new Hammer(), chipT, i, j);
		break;
	case 'R': //赤ワープ
		addObject(new Warp(entityNS::WARP_R), chipT, i, j);
		if (warp_r == -1) warp_r = obj_num - 1;
		else addWarp(warp_r, obj_num - 1);
		break;
	case 'S': //緑ワープ
		addObject(new Warp(entityNS::WARP_G), chipT, i, j);
		if (warp_g == -1) warp_g = obj_num - 1;
		else addWarp(warp_g, obj_num - 1);
		break;
	case 'T': //黄ワープ
		addObject(new Warp(entityNS::WARP_Y), chipT, i, j);
		if (warp_y == -1) warp_y = obj_num - 1;
		else addWarp(warp_y, obj_num - 1);
		break;
	case 'U': //空気箱
		addObject(new AirBox(), chipT, i, j);
		break;
	case 'V': //フレーム箱
		addObject(new FrameBox(), chipT, i, j);
		break;
	case 'X': //ゴール
		addObject(new Goal(), chipT, i, j);
		break;
	case 'Z': //デブ
		map[i][j] = 0;
		if (!debu->initialize(this, &debuT, input, i, j))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	default:
		map[i][j] = 0;
		break;
	}
}

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
	//全回復
	if (input->isKeyPressed('E') && (cheat1)){
		life = 100;
	}
	//デブがすごくなる
	if (str == "tabata") {
		cheat1 = !cheat1;
		input->clearTextIn();
	}
	//判定が見える
	else if (str == "mieru") {
		cheat2 = !cheat2;
		input->clearTextIn();
	}
}

//オブジェクトの追加
void AstralDebu::addObject(Entity *e, Texture &t, int i, int j){
	map[i][j] = 0;
	object[obj_num] = e;
	if (!e->initialize(this, &t, i, j))
		throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
	obj_num++;
}

//敵の追加　デブの位置を入れる
void AstralDebu::addEnemy(Enemy *e, Texture &t, Debu *d, int i, int j){
	map[i][j] = 0;
	object[obj_num] = e;
	if (!e->initialize(this, &t, d, i, j))
		throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
	obj_num++;
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
	if ((map[cursorX()][cursorY()] != 0) && (map[cursorX()][cursorY()] != CHIP_LADDER))  return;

	//存在しているオブジェクトを見る
	ALL_OBJ{
		if ((i != obj_hold) &&
		(object[i]->ChipCX() == cursorX()) &&
		(object[i]->ChipCY() == cursorY()) &&
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
		obj_hold = i;
		debu->setHold(true);

		e->setVelX(0.0f);
		e->setVelY(0.0f);
		moveHold(i);

		if (e->getType() == entityNS::HAMMER) ((Hammer*)e)->setHold(debu);
		else e->setState(entityNS::LOCK);

		switch (e->getType()){
		case entityNS::BOX_W:
		case entityNS::BOX_B:
		case entityNS::BOX_A:
			subLife(5);
			break;
		case entityNS::BOX_S:
		case entityNS::BOMB:
		case entityNS::HIBOMB:
			subLife(10);
			break;
		case entityNS::BOX_L:
		case entityNS::BOX_H:
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
	Entity *e = object[obj_hold];

	obj_hold = -1;
	debu->setHold(false);
	debu->setState(entityNS::STAND);

	if (e->getType() == entityNS::HAMMER){
		if (e->getDirect()) e->setPosX((debu->ChipCX() - 0.5f) * CHIP_SIZE);
		else e->setPosX((debu->ChipCX() + 1.5f) * CHIP_SIZE);
	}
	else {
		e->setPosX((cursorX() + 0.5f) * CHIP_SIZE);
	}
	e->setPosY((cursorY() + 0.5f) * CHIP_SIZE + DATA_LEN);
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
	if ((map[cursorNX][cursorY()] != 0) && (map[cursorNX][cursorY()] != CHIP_LADDER)) return;
	ALL_OBJ{
		if ((object[i]->ChipCX() == cursorNX) &&
		(object[i]->ChipCY() == cursorY()) &&
		canTouch(object[i])) return;
	}

	e->setPosX((cursorNX + 0.5f) * CHIP_SIZE);
	e->setPosY((cursorY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	e->setVelX(0.0f);
	e->setVelY(0.0f);
	e->setState(entityNS::STAND);
	e->setEdge();

	//体力減少
	switch (e->getType()){
	case entityNS::BOX_W:
	case entityNS::BOX_B:
	case entityNS::BOX_A:
		//木箱は体力減少なし
		break;
	case entityNS::BOX_S:
	case entityNS::BOMB:
	case entityNS::HIBOMB:
		subLife(5);
		break;
	case entityNS::BOX_L:
	case entityNS::BOX_H:
		subLife(10);
		break;
	}
}

//オブジェクトを投げる
void AstralDebu::throwObject(){
	Entity *e = object[obj_hold];

	obj_hold = -1;
	debu->setHold(false);
	debu->setState(entityNS::STAND);

	e->setPosX((cursorX() + 0.5f) * CHIP_SIZE);
	e->setPosY((cursorY() + 0.5f) * CHIP_SIZE + DATA_LEN);
	if (debu->getDirect()) e->setVelX(-VEL_THROW);
	else e->setVelX(VEL_THROW);
	e->setVelY(0.0f);
	e->setState(entityNS::KNOCK);
	e->setEdge();

	//体力減少
	switch (e->getType()){
	case entityNS::BOX_W:
	case entityNS::BOX_B:
	case entityNS::BOX_A:
		subLife(5);
		break;
	case entityNS::BOX_S:
	case entityNS::BOMB:
	case entityNS::HIBOMB:
		subLife(10);
		break;
	case entityNS::BOX_L:
	case entityNS::BOX_H:
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
	case entityNS::BOX_B:
	case entityNS::EN_M:
		//消去してから爆風を生成する
		SAFE_DELETE(e);
		object[i] = new Blast(false);
		if (!object[i]->initialize(this, &bombT, x, y))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	case entityNS::HIBOMB:
	case entityNS::BOX_H:
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
	int tmp = obj_num;
	switch (e->getType()){
	case entityNS::EN_3:
		//銃弾生成
		e->setAction(false);
		ALL_OBJ if ((object[i]->getType() == entityNS::NONE) && (object[i]->getState() == entityNS::EMPTY)) {
			tmp = i;
			break;
		}
		if (tmp == obj_num) obj_num++;
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
		ALL_OBJ if ((object[i]->getType() == entityNS::NONE) && (object[i]->getState() == entityNS::EMPTY)) {
			tmp = i;
			break;
		}
		if (tmp == obj_num) obj_num++;
		SAFE_DELETE(object[tmp]);
		object[tmp] = new Missile();
		if (!object[tmp]->initialize(this, &enemyT, e->ChipCX(), e->ChipCY()))
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		object[tmp]->setDirect(e->getDirect());
		object[tmp]->setState(entityNS::KNOCK);
		break;
	}
}
