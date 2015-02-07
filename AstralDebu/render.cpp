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

	if (cheat2) drawEdge();
}

//クリア画面の描画
void AstralDebu::renderClear(){
	graphics->spriteBegin();

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
	drawQuad(8 * CHIP_SIZE, 7.5f*CHIP_SIZE + DATA_LEN, 9 * CHIP_SIZE, 4 * CHIP_SIZE, BLACK);
	drawQuad(8 * CHIP_SIZE + 3, 7.5f*CHIP_SIZE + DATA_LEN + 3, 9 * CHIP_SIZE - 6, 4 * CHIP_SIZE - 6, MENU_BACK);
	middleF.print("はじめから", (int)(9 * CHIP_SIZE), (int)(8 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, MENU_TEXT, DT_CC);
	middleF.print("つづきから", (int)(9 * CHIP_SIZE), (int)(9 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, (clearedStage == 0) ? MENU_HIDE : MENU_TEXT, DT_CC);
	middleF.print("ステージセレクト", (int)(9 * CHIP_SIZE), (int)(10 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, (clearedStage == 0) ? MENU_HIDE : MENU_TEXT, DT_CC);
	//middleF.print("クリアタイム", (int)(9 * CHIP_SIZE), (int)(11 * CHIP_SIZE + DATA_LEN), (int)(7 * CHIP_SIZE), CHIP_SIZE, (clearedStage == 0) ? MENU_HIDE : MENU_TEXT, DT_CC);
	//矢印描画
	drawQuad(8.25f*CHIP_SIZE, (8.25f+count)*CHIP_SIZE + DATA_LEN, (float)CHIP_SIZE, (0.5f*CHIP_SIZE), BLACK);
	drawTriangle((9.25f)*CHIP_SIZE, (8.0f + count)*CHIP_SIZE + DATA_LEN, (9.75f)*CHIP_SIZE, (8.5f + count)*CHIP_SIZE + DATA_LEN,
		(9.25f)*CHIP_SIZE, (9.0f+count)*CHIP_SIZE + DATA_LEN, BLACK);
	drawQuad(8.25f*CHIP_SIZE + 2, (8.25f+count)*CHIP_SIZE + DATA_LEN + 2, (float)CHIP_SIZE, (0.5f*CHIP_SIZE) - 4, RED);
	drawTriangle((9.25f)*CHIP_SIZE + 2, (8.0f + count)*CHIP_SIZE + DATA_LEN + 4, (9.75f)*CHIP_SIZE - 3, (8.5f+count)*CHIP_SIZE + DATA_LEN,
		(9.25f)*CHIP_SIZE + 2, (9.0f + count)*CHIP_SIZE + DATA_LEN - 4, RED);

	graphics->spriteEnd();
}

//ステージセレクトを描画
void AstralDebu::renderTitleSelect(){
	graphics->spriteBegin();
	//選択しているステージを描画
	middleF.print(std::to_string(stage), (int)(15.0f * CHIP_SIZE), (int)(10 * CHIP_SIZE + DATA_LEN), (int)(2.0f * CHIP_SIZE), CHIP_SIZE, WHITE, DT_CC);
	//上下の矢印を描画
	drawTriangle((15.75f)*CHIP_SIZE, (10.0f)*CHIP_SIZE + DATA_LEN, (16.0f)*CHIP_SIZE, (9.75f)*CHIP_SIZE + DATA_LEN,
		(16.25f)*CHIP_SIZE, (10.0f)*CHIP_SIZE + DATA_LEN, MENU_TEXT);
	drawTriangle((15.75f)*CHIP_SIZE, (11.0f)*CHIP_SIZE + DATA_LEN, (16.0f)*CHIP_SIZE, (11.25f)*CHIP_SIZE + DATA_LEN,
		(16.25f)*CHIP_SIZE, (11.0f)*CHIP_SIZE + DATA_LEN, MENU_TEXT);

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
	drawQuad((float)LIFE_MAR_X - DATA_MAR, (float)LIFE_MAR_Y - DATA_MAR,
		(float)LIFE_LEN_X + DATA_MAR * 2, (float)LIFE_LEN_Y + DATA_MAR * 2, WHITE);
	drawQuad((float)LIFE_MAR_X, (float)LIFE_MAR_Y,
		(float)LIFE_LEN_X, (float)LIFE_LEN_Y, BLACK);

	//体力
	drawQuad((float)LIFE_MAR_X, (float)LIFE_MAR_Y,
		(float)life*DATA_MAR, (float)LIFE_LEN_Y, LIFE_GAUGE);

	//体力変化
	if (vitalLife > life)
		drawQuad((float)LIFE_MAR_X + life*DATA_MAR, (float)LIFE_MAR_Y,
		(float)(vitalLife - life)*DATA_MAR, (float)LIFE_LEN_Y, LIFE_VITAL_MINUS);
	else if (vitalLife < life)
		drawQuad((float)LIFE_MAR_X + vitalLife*DATA_MAR, (float)LIFE_MAR_Y,
		(float)(life - vitalLife)*DATA_MAR, (float)LIFE_LEN_Y, LIFE_VITAL_PLUS);

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
		chip.setY((float)(j*CHIP_SIZE) + DATA_LEN);
		for (int i = 0; i < MAP_COL; i++){
			if (map[i][j]){
				chip.setX((float)i*CHIP_SIZE);
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
	chip.setX((float)(getCursorChipX() * CHIP_SIZE));
	chip.setY((float)(getCursorChipY() * CHIP_SIZE) + DATA_LEN);

	chip.setCurrentFrame(CHIP_CURSOR);
	chip.draw();
}