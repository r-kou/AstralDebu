#include "astraldebu.h"

using namespace astralNS;

//タイトル画面の更新
void AstralDebu::updateTitle(){
	if (stateNumber == 0) loadData();
	else if (stateNumber == 1){
		count = count++ % 30;
		if (input->isKeyPressed('Z')){
			if(clearedStage == 0)count = 0;
			else count = 1;
			stateNumber = 2;
		}
	} else if (stateNumber == 2){
		if (clearedStage){
			if (input->isKeyPressed(VK_UP)) count -= (count != 0)?1:-2;
			if (input->isKeyPressed(VK_DOWN)) count += (count != 2)? 1: -2;
		}
		if (input->isKeyPressed('Z')){
			switch (count){
			case 0:
				state = S_STAGE;
				stage = 1;
				read = false;
				break;
			case 1:
				state = S_STAGE;
				stage = clearedStage+1;
				read = false;
				break;
			case 2:
				stateNumber = 3;
				stage = clearedStage;
				break;
			case 3:
				break;
			}
		}
	}
	else if (stateNumber == 3) {
		if (input->isKeyPressed(VK_UP)) stage += (stage != clearedStage)  ? 1 : -clearedStage + 1;
		if (input->isKeyPressed(VK_DOWN)) stage -= (stage != 1) ? 1 : -clearedStage + 1;
		if (input->isKeyPressed('Z')) {
			state = S_STAGE;
			read = false;
		}
		if (input->isKeyPressed('X')) stateNumber = 2;
	}
}

//ステージ表示画面の更新
void AstralDebu::updateStage(){
	if (!read) loadStage();

	if (input->isKeyPressed('Z') && read){
		cheat = false;
		clear = false;
		life = 100;
		vitalLife = 100;
		state = S_MAIN;
		clearTimeStart = timeGetTime();
	}
}

//メインの更新
void AstralDebu::updateMain(){
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
	if (objHolded >= 0) moveHold(objHolded);

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
	ALL_OBJ_EACH if (canTouch(object[i]) && canTouch(object[j]))
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

//セーブデータの読み込み
void AstralDebu::loadData(){
	std::ifstream save(SAV_FILE, std::ios::in | std::ios::binary);
	int buf_stage;
	double buf_time;
	if (save){
		try{
			//クリアしたステージを読み込み
			save.read((char *)&buf_stage, sizeof(int));
			clearedStage = buf_stage;
			//クリア時間を読み込み
			FOR(STG_SIZE) {
				save.read((char *)&buf_time, sizeof(double));
				clearTime[i] = buf_time;
			}
			save.close();
		}
		catch (...){
			save.close();
			throw(GameError(gameErrorNS::FATAL, "Error loading data."));
		}
	}
	else {
		//クリアしたステージを初期化
		clearedStage = 0;
		//クリア時間を初期化
		FOR(STG_SIZE) clearTime[i] = 0;
	}
	stateNumber = 1;
}

//セーブデータの書き込み
void AstralDebu::saveData(){
	std::ofstream save(SAV_FILE, std::ios::out | std::ios::binary | std::ios::trunc);
	int buf_stage = clearedStage;
	if (save){
		//クリアしたステージを読み込み
		save.write((char *)&buf_stage, sizeof(int));
		//クリア時間を読み込み
		FOR(STG_SIZE) {
			save.write((char *)&clearTime[i], sizeof(double));
		}
		save.close();
	}
	else throw(GameError(gameErrorNS::FATAL, "Error saving data."));
}


//ステージの読み込み
void AstralDebu::loadStage(){
	short buf[MAP_COL];
	std::ifstream file(MAP_FILE_DIR + "\\" + MAP_NAME(stage) + MAP_FILE_EXT, std::ios::in | std::ios::binary);

	if (file.fail())
		throw(GameError(gameErrorNS::FATAL, "Error reading map file"));

	//古いオブジェクトをすべて消去
	ALL_OBJ	SAFE_DELETE(object[i]);
	objMax = 0;
	objHolded = -1;
	warpRed = -1;
	warpGreen = -1;
	warpYellow = -1;

	for (int j = 0; j < MAP_ROW; j++){
		file.read((char *)&buf, sizeof(short) * MAP_COL);
		for (int i = 0; i < MAP_COL; i++){
			loadChip(i, j, decodeChip(buf[i],i,j));
		}
	}
	file.close();
	read = true;
}

//チップ割り当て
void AstralDebu::loadChip(int i,int j,short c){
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
		addObject(new Ladder(),chipT,i,j);
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
		//addEnemy(new Enemy4(), enemyT, debu, i, j);
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
			throw(GameError(gameErrorNS::FATAL, "Error initializing entity."));
		break;
	default:
		map[i][j] = 0;
		break;
	}
}

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