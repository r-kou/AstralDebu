#include "astraldebu.h"

using namespace astralNS;

//タイトル画面の更新
void AstralDebu::updateTitle(){
	if (stateNumber == 0) updateTitle0();
	else if (stateNumber == 1) updateTitle1();
	else if (stateNumber == 2) updateTitle2();
	else if (stateNumber == 3) updateTitle3();
	else if (stateNumber == 4) updateTitle4();
	else if (stateNumber == 5) updateTitle5();
	else if (stateNumber == 6) updateTitle6();
}

//ステージ表示画面の更新
void AstralDebu::updateStage(){
	if (!read) {
		loadStage();
	}

	if (in1() && read){
		cheat = false;
		clear = false;
		life = 100;
		vitalLife = 100;
		state = S_MAIN;
		menu = false;
		timeout = TIME_OUT;
		clearTimeStart = timeGetTime();
		audio->setVolumeBgm(bgmVolume);
		audio->playCue(audioNS::OK);
		changeBgm(((stage - 1) / 10) + 1);
	}
}

//メインの更新
void AstralDebu::updateMain(){
	if (menu) {
		//メニュー時は更新しない
		updateMenu();
		return;
	}

	//自動停止のフラグを付ける
	if (timeout > 0.0f){
		timeout -= frameTime;
		if (timeout < 0.0f) timeout = 0.0f;
	}

	if (vitalLife > life) vitalLife--;
	else if (vitalLife < life) vitalLife++;

	if (cheat1 || cheat3) cheat = true;

	if (life == 0) debu->setState(entityNS::ST_DEAD);

	if ((debu->getState() == entityNS::ST_DEAD) && (debu->getComplete())) {
		state = S_STAGE;
		read = false;
	}

	if ((debu->getState() == entityNS::ST_CLEAR) && (debu->getComplete())) clear = true;

	if (clear) {
		clear = false;
		updateClearTime();
		if (stage>clearedStage) clearedStage = stage;
		saveData();
		stage++;
		if (stage > MAX_STAGE){
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
	ALL_OBJ{
		Entity *e = getObject(i);
		if (isMovable(e)) e->move(frameTime);
	}
		//所持オブジェクトはデブの前に移動
	if (objHolded >= 0) moveHold(objHolded);

		//地形への接触
	if (isMovable(debu))debu->touchMap(map);

	ALL_OBJ{
		Entity *e = getObject(i);
		if (isMovable(e)) e->touchMap(map);
	}

	debu->resetResponseAll();
	ALL_OBJ{
		Entity *e = getObject(i);
		e->resetResponseAll();
	}

		//他オブジェクトへの接触
		if (isTouchable(debu)){
		ALL_OBJ{
			Entity *e = getObject(i);
			if (isTouchable(e)) debu->touchObj(e);
		}
		}
	ALL_OBJ_EACH{
		Entity *ei = getObject(i);
		Entity *ej = getObject(j);
		if (isTouchable(ei) && isTouchable(ej))
			ei->touchObj(ej);
	}

	//ワープ判定
	if (isWarpable(debu)){
		ALL_OBJ{
			Entity *e = getObject(i);
			if (isTouchable(e) && ((debu->getResponseX(entityNS::RES_WARP) / CHIP_SIZE) == e->ChipCX())
				&& ((debu->getResponseY(entityNS::RES_WARP) / CHIP_SIZE) == e->ChipCY())) debu->collideWarp(e);
		}
	}
	ALL_OBJ_2{
		if (i == j) continue;
		Entity *ei = getObject(i);
		Entity *ej = getObject(j);
		if (!isWarpable(ei)) continue;
		if (isTouchable(ej) && ((int)(ei->getResponseX(entityNS::RES_WARP) / CHIP_SIZE) == ej->ChipCX())
			&& ((int)((ei->getResponseY(entityNS::RES_WARP) - DATA_LEN) / CHIP_SIZE) == ej->ChipCY())) ei->collideWarp(ej);
	}

		//オブジェクトへの反応実行
	if (isTouchable(debu)) debu->responseObj();
	ALL_OBJ{
		Entity *e = getObject(i);
		if (isTouchable(e))
			e->responseObj();
	}

		ALL_OBJ{
		Entity *e = getObject(i);
		if (e->getAction()) actionObject(i);
	}

		//オブジェクト消滅処理
		ALL_OBJ{
		Entity *e = getObject(i);
		if (e->isDead())
			deadObject(i);
	}

		//描画する画像を変更
	debu->changeImage();
	ALL_OBJ{
		Entity *e = getObject(i);
		e->changeImage();
	}
}

//クリア画面の更新
void AstralDebu::updateClear(){

}

//メニュー画面の更新
void AstralDebu::updateMenu(){
	if (input->isKeyPressed(VK_SPACE) || in2()) {
		menu = false;
		audio->setVolumeBgm(bgmVolume);
		audio->playCue(audioNS::CANCEL);
		timeout = TIME_OUT;
	}
	if (inHorizontal()) {
		count = (count ? 0 : 1);
		audio->playCue(audioNS::SELECT);
	}
	if (in1()){
		if (count) {
			resetObject();
			stateNumber = 1;
			state = S_TITLE;
			stopBgm();
			audio->setVolumeBgm(bgmVolume);
			audio->playCue(audioNS::OK);
			stage = 0;
			changeBgm(0);
		}
		else {
			menu = false;
			audio->setVolumeBgm(bgmVolume);
			audio->playCue(audioNS::CANCEL);
			timeout = TIME_OUT;
		}
	}
}

//セーブデータの読み込み
void AstralDebu::loadData(){
	std::ifstream load;
	int bufStage;
	double bufVolume;
	double bufTime;
	try{
		load.open(DATA(SAV_FILE), std::ios::in | std::ios::binary);
		if (load){
			//クリアしたステージを読み込み
			load.read((char *)&bufStage, sizeof(int));
			if ((bufStage < 0) || (bufStage > 40))
				throw(GameError(gameErrorNS::FATAL, "セーブデータの読み込みに失敗しました\nデータが壊れている可能性があります"));
			clearedStage = bufStage;

			//音量を読み込み
			load.read((char *)&bufVolume, sizeof(double));
			if ((bufVolume < 0.0f) || (bufVolume > 1.0f))
				throw(GameError(gameErrorNS::FATAL, "セーブデータの読み込みに失敗しました\nデータが壊れている可能性があります"));
			bgmVolume = bufVolume;
			audio->setVolumeBgm(bgmVolume);

			load.read((char *)&bufVolume, sizeof(double));
			if ((bufVolume < 0.0f) || (bufVolume > 1.0f))
				throw(GameError(gameErrorNS::FATAL, "セーブデータの読み込みに失敗しました\nデータが壊れている可能性があります"));
			soundVolume = bufVolume;
			audio->setVolumeSound(soundVolume);
			//クリア時間を読み込み
			FOR(STG_SIZE) {
				load.read((char *)&bufTime, sizeof(double));
				clearTime[i] = bufTime;
			}
		}
		else {
			//クリアしたステージを初期化
			clearedStage = 0;
			//音量を初期化
			bgmVolume = 1.0f;
			audio->setVolumeBgm(bgmVolume);
			soundVolume = 1.0f;
			audio->setVolumeSound(soundVolume);
			//クリア時間を初期化
			FOR(STG_SIZE) clearTime[i] = 0;
		}
		load.close();
	}
	catch (...){
		load.close();
		throw(GameError(gameErrorNS::FATAL, "セーブデータの読み込みに失敗しました\nデータが壊れている可能性があります"));
	}
	stateNumber = 1;
}

//セーブデータの書き込み
void AstralDebu::saveData(){
	std::ofstream save;
	int bufStage;
	double bufVolume;
	try{
		save.open(DATA(SAV_FILE), std::ios::out | std::ios::binary | std::ios::trunc);
		if (save){
			//クリアしたステージを書き込み
			bufStage = clearedStage;
			save.write((char *)&bufStage, sizeof(int));
			//音量を書き込み
			bufVolume = bgmVolume;
			save.write((char *)&bufVolume, sizeof(double));
			bufVolume = soundVolume;
			save.write((char *)&bufVolume, sizeof(double));
			//クリア時間を書き込み
			FOR(STG_SIZE) {
				save.write((char *)&clearTime[i], sizeof(double));
			}
			save.close();
		}
		else {
			save.close();
			throw(GameError(gameErrorNS::FATAL, "セーブデータの書き込みに失敗しました．"));
		}
	}
	catch (...){
		save.close();
		throw(GameError(gameErrorNS::FATAL, "セーブデータの書き込みに失敗しました．"));
	}

}


//ステージの読み込み
void AstralDebu::loadStage(){
	short buf[MAP_COL];
	std::string fileName = MAP(stage);
	std::ifstream file;

	try{
		file.open(fileName, std::ios::in | std::ios::binary);
		if (!file){
			file.close();
			throw(GameError(gameErrorNS::FATAL, ("マップデータの読み込みに失敗しました")));
		}

		resetObject();

		for (int j = 0; j < MAP_ROW; j++){
			file.read((char *)&buf, sizeof(short) * MAP_COL);
			for (int i = 0; i < MAP_COL; i++){
				loadChip(i, j, decodeChip(buf[i], i, j));
			}
		}
		file.close();
		read = true;
	}
	catch (...){
		file.close();
		throw;
	}
}

//チップ割り当て
void AstralDebu::loadChip(int i, int j, short c){
	switch (c){
	case 0:
		map[i][j] = 0;
		break;
	case 1:
		map[i][j] = 1;
		break;
	case 2:
		map[i][j] = 2;
		break;
	case 3:
		map[i][j] = 3;
		break;
	case 4:
		map[i][j] = 4;
		break;
	case 5:
		map[i][j] = 9;
		break;
	case 6:
		map[i][j] = 10;
		break;
	case 7:
		map[i][j] = 11;
		break;
	case 8:
		map[i][j] = 12;
		break;
	case 9: //岩
		addObject(new Rock(stage), chipT, i, j);
		break;
	case 10: //木箱
		addObject(new WoodBox(), chipT, i, j);
		break;
	case 11: //鉄箱
		addObject(new SteelBox(), chipT, i, j);
		break;
	case 12: //鉛箱
		addObject(new LeadBox(), chipT, i, j);
		break;
	case 13: //爆弾箱
		addObject(new BombBox(), chipT, i, j);
		break;
	case 14: //超爆弾箱
		addObject(new HibombBox(), chipT, i, j);
		break;
	case 15: //空気箱
		addObject(new AirBox(), chipT, i, j);
		break;
	case 16: //フレーム箱
		addObject(new FrameBox(), chipT, i, j);
		break;
	case 17: //霊はこ
		addObject(new GoastBox(), chipT, i, j);
		break;
	case 18: //はしご
		addObject(new Ladder(), chipT, i, j);
		break;
	case 19: //爆弾
		addObject(new Bomb(), chipT, i, j);
		break;
	case 20: //超爆弾
		addObject(new Hibomb(), chipT, i, j);
		break;
	case 21: //機雷
		addObject(new Mine(), chipT, i, j);
		break;
	case 22: //鉄球
		addObject(new Hammer(), chipT, i, j);
		break;
	case 23: //にく
		addObject(new Meat(), chipT, i, j);
		break;
	case 24: //超にく
		addObject(new Himeat(), chipT, i, j);
		break;
	case 25: //敵(歩くだけ)
		addEnemy(new Enemy1(), enemyT, debu, i, j);
		break;
	case 26: //敵(こっちに向かってくる)
		addEnemy(new Enemy2(), enemyT, debu, i, j);
		break;
	case 27: //敵(動かずに弾を撃つ)
		addEnemy(new Enemy3(), enemyT, debu, i, j);
		break;
	case 28: //敵(箱を見ると押す)
		addEnemy(new Enemy4(), enemyT, debu, i, j);
		break;
	case 29: //敵(動かずにミサイル)
		addEnemy(new Enemy5(), enemyT, debu, i, j);
		break;
	case 30: //赤ワープ
		addObject(new Warp(entityNS::TY_RED_WARP), chipT, i, j);
		if (warpRed == -1) warpRed = objMax - 1;
		else addWarp(warpRed, objMax - 1);
		break;
	case 31: //緑ワープ
		addObject(new Warp(entityNS::TY_GREEN_WARP), chipT, i, j);
		if (warpGreen == -1) warpGreen = objMax - 1;
		else addWarp(warpGreen, objMax - 1);
		break;
	case 32: //黄ワープ
		addObject(new Warp(entityNS::TY_YELLOW_WARP), chipT, i, j);
		if (warpYellow == -1) warpYellow = objMax - 1;
		else addWarp(warpYellow, objMax - 1);
		break;
	case 33: //ゴール
		addObject(new Goal(), chipT, i, j);
		break;
	case 34: //デブ
		map[i][j] = 0;
		if (!debu->initialize(this, &debuT, input, i, j))
			throw(GameError(gameErrorNS::FATAL, "オブジェクトの生成に失敗しました"));
		break;
	default:
		map[i][j] = 0;
		break;
	}
}

//クリア時間を記録
void AstralDebu::updateClearTime(){
	//チート時は記録しない
	if (cheat) return;
	//終了時刻を取得
	clearTimeEnd = timeGetTime();
	double total_time = (double)(clearTimeEnd - clearTimeStart) / 1000;
	//未記録なら更新
	if (clearTime[stage - 1] == 0) clearTime[stage - 1] = total_time;
	//以前の記録より早いなら更新
	else if (total_time < clearTime[stage - 1]) clearTime[stage - 1] = total_time;
}

//タイトル画面を更新
void AstralDebu::updateTitle0() {
	audio->playCue(audioNS::BGM_TITLE);
	bgm = true;
	loadData();
}

//タイトル画面を更新
void AstralDebu::updateTitle1(){
	count = count++ % 30;
	if (in1()){
		if (clearedStage == 0)count = 0;
		else if(clearedStage == MAX_STAGE) count = 2;
		else count = 1;
		stateNumber = 2;
		audio->playCue(audioNS::OK);
	}
}

//タイトル画面を更新
void AstralDebu::updateTitle2(){
	if (inCursor()) audio->playCue(audioNS::SELECT);
	if (clearedStage == 0){
		if (inUp() || inLeft()) count -= (count == 0) ? -4 : (count == 3) ? 3 : 1;
		if (inDown() || inRight()) count += (count == 4) ? -4 : (count == 0) ? 3 : 1;
	}
	else if (clearedStage == MAX_STAGE){
		if (inUp() || inLeft()) count -= (count == 0) ? -4 : (count == 2) ? 2 : 1;
		if (inDown() || inRight()) count += (count == 4) ? -4 : (count == 0) ? 2 : 1;
	}
	else {
		if (inUp() || inLeft()) count -= (count == 0) ? -4 : 1;
		if (inDown() || inRight()) count += (count == 4) ? -4 : 1;
	}
	if (in1()){
		switch (count){
		case 0:
			state = S_STAGE;
			stage = 1;
			read = false;
			stopBgm();
			break;
		case 1:
			state = S_STAGE;
			stage = clearedStage + 1;
			read = false;
			stopBgm();
			break;
		case 2:
			stateNumber = 3;
			stage = clearedStage;
			break;
		case 3:
			stateNumber = 4;
			count = 0;
			break;
		case 4:
			stopBgm();
			PostQuitMessage(0);
			break;
		}
		audio->playCue(audioNS::OK);
	}
	if (in2()) {
		stateNumber = 1;
		audio->playCue(audioNS::CANCEL);
	}
}

//タイトル画面を更新
void AstralDebu::updateTitle3(){
	if (inCursor()) {
		audio->playCue(audioNS::SELECT);
		if (inUp()) stage += 1;
		if (inDown()) stage -= 1;
		if (inLeft()) stage -= 5;
		if (inRight()) stage += 5;
		if (stage < 1) stage = 1;
		if (stage > clearedStage) stage = clearedStage;
	}
	if (in1()) {
		state = S_STAGE;
		read = false;
		stopBgm();
		audio->playCue(audioNS::OK);
	}
	if (in2()) {
		stateNumber = 2;
		audio->playCue(audioNS::CANCEL);
	}
}

//タイトル画面を更新
void AstralDebu::updateTitle4() {
	if (inCursor()) audio->playCue(audioNS::SELECT);
	if (inUp() || inLeft()) count -= (count != 0) ? 1 : -2;
	if (inDown() || inRight()) count += (count != 2) ? 1 : -2;
	if (in1()) {
		switch (count){
		case 0:
			stateNumber = 5;
			fCount = 0.0f;
			audio->playCue(audioNS::OK);
			break;
		case 1:
			stateNumber = 6;
			fCount = 0.0f;
			audio->playCue(audioNS::OK);
			break;
		case 2:
			count = 3;
			stateNumber = 2;
			audio->playCue(audioNS::CANCEL);
			break;
		}
	}
	if (in2()) {
		count = 3;
		stateNumber = 2;
		audio->playCue(audioNS::CANCEL);
	}
}

//タイトル画面を更新
void AstralDebu::updateTitle5() {
	if (fCount == 0.0f){
		if (downUp()) bgmVolume += 0.01;
		if (downDown()) bgmVolume -= 0.01;
		if (downLeft()) bgmVolume -= 0.1;
		if (downRight()) bgmVolume += 0.1;
		if (downCursor()) {
			audio->playCue(audioNS::SELECT);
			fCount = 0.05f;
			if (bgmVolume < 0.005) bgmVolume = 0.0f;
			if (bgmVolume > 0.995) bgmVolume = 1.0f;
		}
	}
	else {
		fCount -= frameTime;
		if (fCount < 0.0f) fCount = 0.0f;
	}

	audio->setVolumeBgm(bgmVolume);
	if (in1() || in2()) {
		stateNumber = 4;
		saveData();
		audio->playCue(audioNS::CANCEL);
	}
}

//タイトル画面を更新
void AstralDebu::updateTitle6(){
	if (fCount == 0.0f){
		if (downUp()) soundVolume += 0.01;
		if (downDown()) soundVolume -= 0.01;
		if (downLeft()) soundVolume -= 0.1;
		if (downRight()) soundVolume += 0.1;
		if (downCursor()) {
			audio->playCue(audioNS::SELECT);
			fCount = 0.05f;
			if (soundVolume < 0.005) soundVolume = 0.0f;
			if (soundVolume > 0.995) soundVolume = 1.0f;
		}
	}
	else {
		fCount -= frameTime;
		if (fCount < 0.0f) fCount = 0.0f;
	}

	audio->setVolumeBgm(soundVolume);
	if (in1() || in2()) {
		stateNumber = 4;
		saveData();
		audio->playCue(audioNS::CANCEL);
	}
}
