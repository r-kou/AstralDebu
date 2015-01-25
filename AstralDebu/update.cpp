#include "astraldebu.h"

using namespace astralNS;

//タイトル画面の更新
void AstralDebu::updateTitle(){
	if (state_num == 0) loadData();
	else if (state_num == 1){
		count = count++ % 30;
		if (input->isKeyPressed('Z')){
			if(stage_max == 0)count = 0;
			else count = 1;
			state_num = 2;
		}
	} else if (state_num == 2){
		if (stage_max != 0){
			if (input->isKeyPressed(VK_UP)) count -= (count != 0) ? 1 : -3;
			if (input->isKeyPressed(VK_DOWN)) count += (count != 3) ? 1 : -3;
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
				stage = stage_max+1;
				read = false;
				break;
			case 2:
				state_num = 3;
				stage = stage_max;
				break;
			case 3:
				break;
			}
		}
	}
	else if (state_num == 3) {
		if (input->isKeyPressed(VK_UP)) stage += (stage != stage_max)  ? 1 : -stage_max + 1;
		if (input->isKeyPressed(VK_DOWN)) stage -= (stage != 1) ? 1 : -stage_max + 1;
		if (input->isKeyPressed('Z')) {
			state = S_STAGE;
			read = false;
		}
		if (input->isKeyPressed('X')) state_num = 2;
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
		stage_start = timeGetTime();
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
		updateClearTime();
		if (stage>stage_max) stage_max = stage;
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

//セーブデータの読み込み
void AstralDebu::loadData(){
	std::ifstream save(SAV_NAME, std::ios::in | std::ios::binary);
	int buf_stage;
	double buf_time;
	if (save){
		try{
			//クリアしたステージを読み込み
			save.read((char *)&buf_stage, sizeof(int));
			stage_max = buf_stage;
			//クリア時間を読み込み
			FOR(STG_SIZE) {
				save.read((char *)&buf_time, sizeof(double));
				clear_time[i] = buf_time;
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
		stage_max = 0;
		//クリア時間を初期化
		FOR(STG_SIZE) clear_time[i] = 0;
	}
	state_num = 1;
}

//セーブデータの書き込み
void AstralDebu::saveData(){
	std::ofstream save(SAV_NAME, std::ios::out | std::ios::binary | std::ios::trunc);
	int buf_stage = stage_max;
	if (save){
		//クリアしたステージを読み込み
		save.write((char *)&buf_stage, sizeof(int));
		//クリア時間を読み込み
		FOR(STG_SIZE) {
			save.write((char *)&clear_time[i], sizeof(double));
		}
		save.close();
	}
	else throw(GameError(gameErrorNS::FATAL, "Error saving data."));
}


//ステージの読み込み
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

void AstralDebu::updateClearTime(){
	//チート時は記録しない
	if (cheat) return;
	//終了時刻を取得
	stage_end = timeGetTime();
	double total_time = (double)(stage_end - stage_start) / 1000;
	//未記録なら更新
	if (clear_time[stage - 1] == 0) clear_time[stage - 1] = total_time;
	//以前の記録より早いなら更新
	else if (total_time < clear_time[stage - 1]) clear_time[stage - 1] = total_time;
}