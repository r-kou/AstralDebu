#include "astraldebu.h"
using namespace astralNS;

//タイトル画面の描画
void AstralDebu::renderTitle(){
	//タイトルを描画
	renderTitleBack();
	//クリック前
	if (stateNumber <= 1) renderTitleStart();
	if (stateNumber >= 2) renderTitleMenu();
	if (stateNumber == 3) renderTitleSelect();
	if (stateNumber >= 4) renderTitleVolume();
}

//ステージ表示画面の描画
void AstralDebu::renderStage(){
	graphics->spriteBegin();

	drawQuad(0, 0, (float)WINDOW_W, (float)WINDOW_H, BLACK);

	bigF.print("STAGE" + std::to_string(stage),
		0, (WINDOW_H - 60) / 2,
		WINDOW_W, WINDOW_H,
		WHITE, DT_CT);

	graphics->spriteEnd();
}

//メインの描画
void AstralDebu::renderMain(){
	//背景塗りつぶし
	renderBack();

	//マップチップ塗りつぶし
	renderChip();

	//ヒントとか描画
	renderHint();

	//オブジェクト描画
	renderObject();

	//判定表示（チート時）
	if (cheat2) drawEdge();

	//メニュー表示（メニュー時）
	if (menu) renderMenu();
}

//クリア画面の描画
void AstralDebu::renderClear(){
	graphics->spriteBegin();

	graphics->spriteEnd();
}

//メニュー画面の描画
void AstralDebu::renderMenu(){
	using namespace mainMenuNS;
	graphics->spriteBegin();

	drawFrame(MAR_X,MAR_Y,LEN_X,LEN_Y,FRAME,SHADOW,BACK&graphicsNS::ALPHA90,BACK,BACK_SHADE,BACK_LIGHT);

	numberF.print("Ｚ：ジャンプ", TEX_MAR_X, TEX_MAR_Y, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("Ｘ：アイテムを持ち上げる", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("　　持ち上げ時は置く", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 2, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("Ｃ：アイテムを押す", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 3, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("　：持ち上げ時は投げる", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 4, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("Ｑ：やり直す", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 5, TEX_LEN_X, TEX_SEP_Y, BLACK, DT_LC);
	numberF.print("ゲームを続ける", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, BLACK, DT_CC);
	numberF.print("タイトルに戻る", TEX_MAR_X + TEX_SEP_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, BLACK, DT_CC);
	
	//矢印描画
	drawArrowHorizontal(ARR_MAR_X + (count ? TEX_SEP_X : 0),ARR_MAR_Y,CHIP(1.5f),CHIP(1.0f),false,RED);
	graphics->spriteEnd();
}

//タイトルと背景を描画
void AstralDebu::renderTitleBack(){
	graphics->spriteBegin();
	title.draw();

	bigF.print("アストラル・デブ",
		2, 122, WINDOW_W, WINDOW_H,
		WHITE, DT_CT);

	bigF.print("アストラル・デブ",
		0, 120, WINDOW_W, WINDOW_H,
		RED, DT_CT);

	middleF.print("－－その時デブは星になる－－",
		0, 260, WINDOW_W, WINDOW_H,
		WHITE, DT_CT);

	graphics->spriteEnd();
}

//スタートを描画
void AstralDebu::renderTitleStart(){
	graphics->spriteBegin();

	if (count < 20){
		bigF.print("「Ｚ」キーで　スタート",
			0, 400, WINDOW_W, WINDOW_H,
			RED, DT_CT);
	}
	graphics->spriteEnd();
}

//メニューを描画
void AstralDebu::renderTitleMenu(){
	graphics->spriteBegin();
	//メニューを描画
	drawFrame(CHIP(8), CHIP_D(7.5f), CHIP(9), CHIP(6),3.0f,MENU_BACK,BLACK);

	middleF.print("はじめから", CHIP(9), CHIP_D(8), CHIP(7),CHIP(1), MENU_TEXT, DT_CC);
	middleF.print("つづきから", CHIP(9), CHIP_D(9), CHIP(7), CHIP(1), menuText(clearedStage!=0), DT_CC);
	middleF.print("ステージセレクト", CHIP(9), CHIP_D(10), CHIP(7), CHIP(1), menuText(clearedStage != 0), DT_CC);
	middleF.print("音量設定", CHIP(9), CHIP_D(11), CHIP(7), CHIP(1), MENU_TEXT, DT_CC);
	middleF.print("終了する", CHIP(9), CHIP_D(12), CHIP(7), CHIP(1), MENU_TEXT, DT_CC);
	//矢印描画
	if ((stateNumber >= 2) && (stateNumber <= 3)){
		drawArrowHorizontal(CHIP(8.25f), CHIP_D(8.0f + count), CHIP(1.5f), CHIP(1.0f),false,RED);
	}

	graphics->spriteEnd();
}

//ステージセレクトを描画
void AstralDebu::renderTitleSelect(){
	graphics->spriteBegin();
	//選択しているステージを描画
	middleF.print(std::to_string(stage), CHIP(15), CHIP_D(10), CHIP(2), CHIP(1), WHITE, DT_CC);
	//上下の矢印を描画
	drawTriangle(CHIP(15.75f), CHIP_D(10.0f), CHIP(16.0f), CHIP_D(9.75f),
		CHIP(16.25f), CHIP_D(10.0f), MENU_TEXT);
	drawTriangle(CHIP(15.75f), CHIP_D(11.0f), CHIP(16.0f), CHIP_D(11.25f),
		CHIP(16.25f), CHIP_D(11.0f), MENU_TEXT);

	graphics->spriteEnd();
}

//音量選択を描画
void AstralDebu::renderTitleVolume(){
	graphics->spriteBegin();

	drawFrame(CHIP(7), CHIP_D(8.0f), CHIP(11), CHIP(4), 3.0f, MENU_BACK, BLACK);

	middleF.print("ＢＧＭ", CHIP(9), (int)CHIP_D(8.5f), CHIP(3), CHIP(1), MENU_TEXT, DT_CC);
	middleF.print(doubleToString(bgmVolume), CHIP(13), (int)CHIP_D(8.5f), CHIP(3), CHIP(1), WHITE, DT_CC);
	middleF.print("効果音", CHIP(9), (int)CHIP_D(9.5f), CHIP(3), CHIP(1), MENU_TEXT, DT_CC);
	middleF.print(doubleToString(soundVolume), CHIP(13), (int)CHIP_D(9.5f), CHIP(3), CHIP(1), WHITE, DT_CC);
	middleF.print("戻る", CHIP(9), (int)CHIP_D(10.5f), CHIP(7), CHIP(1), MENU_TEXT, DT_CC);

	drawArrowHorizontal(CHIP(7.25f), CHIP_D(8.5f+count), CHIP(1.5f), CHIP(1.0f), false, RED);

	if (stateNumber == 5){
		drawTriangleVertical(CHIP(13.0f), CHIP_D(8.75f), CHIP(0.5f), CHIP(0.5f), true, menuText(bgmVolume<1.0));
		drawTriangleVertical(CHIP(15.5f), CHIP_D(8.75f), CHIP(0.5f), CHIP(0.5f), false, menuText(bgmVolume>0.0));
	}
	if (stateNumber == 6){
		drawTriangleVertical(CHIP(13.0f), CHIP_D(9.75f), CHIP(0.5f), CHIP(0.5f), true, menuText(soundVolume<1.0));
		drawTriangleVertical(CHIP(15.5f), CHIP_D(9.75f), CHIP(0.5f), CHIP(0.5f), false, menuText(soundVolume>0.0));
	}

	graphics->spriteEnd();
}

//背景と上のを描画
void AstralDebu::renderBack(){
	graphics->spriteBegin();

	ARGB back;
	if ((stage <= 10) || (stage == 20)) back = STG1_BACK;
	else if (stage <= 19) back = STG2_BACK;
	else if (stage <= 30) back = STG3_BACK;
	else back = STG4_BACK;

	//背景塗りつぶし
	drawQuad(0, 0, (float)WINDOW_W, (float)WINDOW_H, back);

	//上部データ領域塗りつぶし
	drawQuad(0, 0, (float)WINDOW_W, (float)DATA_LEN, BLACK);

	//体力メータを描画
	//枠
	drawQuad(LIFE_MAR_X - DATA_MAR, LIFE_MAR_Y - DATA_MAR,
		LIFE_LEN_X + DATA_MAR * 2, LIFE_LEN_Y + DATA_MAR * 2, WHITE);
	drawQuad(LIFE_MAR_X, LIFE_MAR_Y,
		LIFE_LEN_X, LIFE_LEN_Y, BLACK);

	//体力
	drawQuad(LIFE_MAR_X, LIFE_MAR_Y,
		life*DATA_MAR, LIFE_LEN_Y, LIFE_GAUGE);

	//体力変化
	if (vitalLife > life)
		drawQuad(LIFE_MAR_X + life*DATA_MAR, LIFE_MAR_Y,
		(vitalLife - life)*DATA_MAR, LIFE_LEN_Y, LIFE_VITAL_MINUS);
	else if (vitalLife < life)
		drawQuad(LIFE_MAR_X + vitalLife*DATA_MAR, LIFE_MAR_Y,
		(life - vitalLife)*DATA_MAR, LIFE_LEN_Y, LIFE_VITAL_PLUS);

	//体力表示
	middleF.print("LIFE",
		WORD_MAR_X, WORD_MAR_Y,
		WORD_LEN_X, WORD_LEN_Y,
		WHITE, DT_CC);

	bigF.print(std::to_string(life),
		LIFE_MAR_X, LIFE_MAR_Y,
		LIFE_LEN_X, LIFE_LEN_Y,
		BLACK, DT_CC);

	bigF.print(std::to_string(life),
		LIFE_MAR_X - DATA_MAR / 2, LIFE_MAR_Y - DATA_MAR / 2,
		LIFE_LEN_X, LIFE_LEN_Y,
		WHITE, DT_CC);

	//ステージ表示

	numberF.print("STAGE" + std::to_string(stage),
		STG_MAR_X, STG_MAR_Y,
		STG_LEN_X, STG_LEN_Y,
		WHITE, DT_CC);

	graphics->spriteEnd();
}

//マップチップを描画
void AstralDebu::renderChip(){
	graphics->spriteBegin();

	//マップ描画
	for (int j = 0; j < MAP_ROW; j++){
		chip.setY((float)CHIP_D(j));
		for (int i = 0; i < MAP_COL; i++){
			if (map[i][j]){
				chip.setX((float)CHIP(i));
				if (map[i][j] <= 12) chip.setCurrentFrame(setChipImage(map[i][j]));
				else chip.setCurrentFrame(map[i][j]);
				if (stage >= 31) chip.draw(graphicsNS::ALPHA50);
				else chip.draw();
			}
		}
	}

	graphics->spriteEnd();
}

//ヒントとか描画
void AstralDebu::renderHint(){
	graphics->spriteBegin();

	if (stage == 1){
		drawPanel("Z", 3.5f, 10.5f, 1.0f, HINT_PANEL);

		drawArrowHorizontal(3.0f, 11.5f, false, HINT_ARROW);

		drawPanel("GOAL", 2.0f, 1.5f, 3.0f, HINT_PANEL);

		drawArrowHorizontal(2.5f, 2.5f, true, HINT_ARROW);

		drawArrowVertical(20.5f, 5.0f, true, HINT_ARROW);
	}
	else if (stage == 2){
		drawPanel("C", 4, 11.0f, 1.0f, HINT_PANEL);

		drawArrowHorizontal(3.5f, 12.0f, false, HINT_ARROW);

		drawPanel("X", 13.0f, 8.0f, 1.0f, HINT_PANEL);

		drawArrowHorizontal(12.5f, 9.0f, false, HINT_ARROW);

		drawPanel("X", 20.0f, 11.25f, 1.0f, HINT_PANEL);

		drawArrowVertical(21.0f, 11.0f, false, HINT_ARROW);
	}
	else if (stage == 3){
		drawPanel("X+C", 4.5f, 10.0f, 2.0f, HINT_PANEL);

		drawArrowHorizontal(4.5f, 11.0f, false, HINT_ARROW);

		drawPanel("X", 14.0f, 0.5f, 1.0f, HINT_PANEL);

		drawArrowVertical(13.0f, 0.25f, false, HINT_ARROW);

		drawPanel(CHIP_BOMB, 10.0f, 10.5f, HINT_PANEL);
	}
	else if (stage == 10){
		drawPanel(CHIP_WOOD_BOX, 6.0f, 10.5f, HINT_PANEL);

		drawArrowHorizontal(5.5f, 11.5f, false, HINT_ARROW);

		drawPanel(CHIP_LEAD_BOX, 15.0f, 8.5f, HINT_PANEL);

		drawArrowHorizontal(14.5f, 9.5f, false, HINT_ARROW);
	}
	
	graphics->spriteEnd();
}

//オブジェクトを描画
void AstralDebu::renderObject(){
	graphics->spriteBegin();

	//オブジェクトを描画
	for (entityNS::RENDER_ORDER ro = entityNS::RO_BASE; ro < entityNS::RO_DEBU; ro = static_cast<entityNS::RENDER_ORDER>(ro + 1)){
		ALL_OBJ if ((canMove(object[i])&&(object[i]->isRenderOrder(ro)))) object[i]->draw();
	}

	//持ち物を描画
	if ((objHolded >= 0)&&(object[objHolded]->isRenderOrder(entityNS::RO_HOLD))) object[objHolded]->draw();

	//デブを描画
	debu->draw();

	//左向きのハンマーをはデブの前に描画
	if ((objHolded >= 0) && (object[objHolded]->isRenderOrder(entityNS::RO_HAMMER))) object[objHolded]->draw();
	//カーソルを描画
	drawCursor();

	graphics->spriteEnd();
}

//カーソルを描画
void AstralDebu::drawCursor(){
	chip.setX((float)CHIP(getCursorChipX()));
	chip.setY((float)CHIP_D(getCursorChipY()));

	chip.setCurrentFrame(CHIP_CURSOR);
	chip.draw();
}