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
	if (cheat2) renderEdge();

	//速度表示（チート時）
	if (cheat4) renderVelocity();

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
	using namespace menuNS;
	using namespace menuNS::mainMenuNS;
	graphics->spriteBegin();

	drawFrame(REC_FRAME(true));

	numberF.printShadow("Ｚ：ジャンプ", REC_TEX(0, 0), 1.0f,BLACK,WHITE, DT_LC);
	numberF.printShadow("Ｘ：アイテムを持ち上げる", REC_TEX(0, 1),1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("　　持ち上げ時は置く", REC_TEX(0, 2), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("Ｃ：アイテムを押す", REC_TEX(0, 3), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("　　持ち上げ時は投げる", REC_TEX(0, 4), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("Ｑ：やり直す", REC_TEX(0, 5), 1.0f, BLACK,WHITE, DT_LC);
	numberF.printShadow("ゲームを続ける", TEX_MAR_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, 1.0f, BLACK, WHITE, DT_CC);
	numberF.printShadow("タイトルに戻る", TEX_MAR_X + TEX_SEP_X, TEX_MAR_Y + TEX_SEP_Y * 7, TEX_SEP_X, TEX_SEP_Y, 1.0f, BLACK, WHITE, DT_CC);

	//矢印描画
	drawArrowHorizontal(REC_ARR(count,0),false,RED);
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
	using namespace menuNS;
	using namespace menuNS::titleMenuNS;
	graphics->spriteBegin();
	//メニューを描画
	drawFrame(REC_FRAME(false));

	middleF.printShadow("はじめから", REC_TEX(0, 0),1.0f, TEXT_SHOW,TEXT_HIDE, DT_CC);
	middleF.printShadow("つづきから", REC_TEX(0, 1), 1.0f, menuText((clearedStage != 0) && (clearedStage != MAX_STAGE)), menuText((clearedStage == 0) || (clearedStage == MAX_STAGE)), DT_CC);
	middleF.printShadow("ステージセレクト", REC_TEX(0, 2), 1.0f, menuText(clearedStage != 0), menuText(clearedStage == 0), DT_CC);
	middleF.printShadow("オーディオ", REC_TEX(0, 3), 1.0f,TEXT_SHOW, TEXT_HIDE, DT_CC);
	middleF.printShadow("終了する", REC_TEX(0, 4), 1.0f,TEXT_SHOW, TEXT_HIDE, DT_CC);
	//矢印描画
	if ((stateNumber >= 2) && (stateNumber <= 3)){
		drawArrowHorizontal(REC_ARR(0,count),false,RED);
	}

	graphics->spriteEnd();
}

//ステージセレクトを描画
void AstralDebu::renderTitleSelect(){
	using namespace menuNS;
	using namespace menuNS::selectMenuNS;
	graphics->spriteBegin();
	//選択しているステージを描画
	middleF.printShadow(std::to_string(stage), REC_TEX(0,0),1.0f, WHITE,BLACK, DT_CC);
	//上下の矢印を描画
	drawTriangleVertical(REC_TRI(0, 0), true, menuText(stage < clearedStage));
	drawTriangleVertical(REC_TRI(0,1),false, menuText(stage > 1));

	graphics->spriteEnd();
}

//音量選択を描画
void AstralDebu::renderTitleVolume(){
	using namespace menuNS;
	using namespace menuNS::audioMenuNS;
	graphics->spriteBegin();

	drawFrame(REC_FRAME(false));

	middleF.printShadow("ＢＧＭ", REC_TEX(0, 0),1.0f, TEXT_SHOW, TEXT_HIDE, DT_CC);
	middleF.printShadow(doubleToString(bgmVolume), REC_TEX(2,0),1.0f, WHITE,BLACK, DT_CC);
	middleF.printShadow("効果音", REC_TEX(0, 1),1.0f, TEXT_SHOW, TEXT_HIDE, DT_CC);
	middleF.printShadow(doubleToString(soundVolume), REC_TEX(2,1),1.0f, WHITE,BLACK, DT_CC);
	middleF.printShadow("戻る", REC_TEX(1, 2),1.0f, TEXT_SHOW, TEXT_HIDE, DT_CC);

	drawArrowHorizontal(REC_ARR(0,count), false, RED);

	if (stateNumber == 5){
		drawTriangleVertical(REC_TRI(0,0), false, menuText(bgmVolume>0.0));
		drawTriangleVertical(REC_TRI(1,0), true, menuText(bgmVolume<1.0));
	}
	if (stateNumber == 6){
		drawTriangleVertical(REC_TRI(0, 1), false, menuText(soundVolume>0.0));
		drawTriangleVertical(REC_TRI(1, 1), true, menuText(soundVolume<1.0));
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

	bigF.printShadow(std::to_string(life),
		LIFE_MAR_X, LIFE_MAR_Y,
		LIFE_LEN_X, LIFE_LEN_Y,
		2.0f,WHITE,BLACK, DT_CC);

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

	switch (stage){
	case 1:
		drawPanel("Z", 3.5f, 10.25f, 1.0f, HINT_PANEL);
		drawArrowHorizontal(3.0f, 11.5f, false, HINT_ARROW);
		drawPanel("GOAL", 2.0f, 1.25f, 3.0f, HINT_PANEL);
		drawArrowHorizontal(2.5f, 2.5f, true, HINT_ARROW);
		drawPanel("↑", 19.5f, 5.75f, 1.0f, HINT_PANEL);
		drawArrowVertical(20.5f, 5.0f, true, HINT_ARROW);
		break;
	case 2:
		drawPanel("C", 4.0f, 11.25f, 1.0f, HINT_PANEL);
		drawArrowHorizontal(3.5f, 12.5f, false, HINT_ARROW);
		drawPanel("X", 12.0f, 9.25f, 1.0f, HINT_PANEL);
		drawArrowHorizontal(11.5f, 10.5f, false, HINT_ARROW);
		drawPanel("X", 16.0f, 11.25f, 1.0f, HINT_PANEL);
		drawArrowVertical(17.0f, 11.0f, false, HINT_ARROW);
		drawPanel("Z", 22.0f, 10.25f, 1.0f, HINT_PANEL);
		drawArrowVertical(21.0f, 9.5f, true, HINT_ARROW);
		break;
	case 3:
		drawPanel("X→C", 4.5f, 10.25f, 2.0f, HINT_PANEL);
		drawArrowHorizontal(4.5f, 11.5f, false, HINT_ARROW);
		drawPanel("X→C", 18.5f, 8.25f, 2.0f, HINT_PANEL);
		drawArrowHorizontal(18.5f, 9.5f, true, HINT_ARROW);
		drawPanel("X", 14.0f, 0.5f, 1.0f, HINT_PANEL);
		drawArrowVertical(13.0f, 0.25f, false, HINT_ARROW);
		drawPanel(CHIP_BOMB, 10.0f, 10.5f, HINT_PANEL);
		break;
	case 4:
		drawPanelD(CHIP_BOMB,CHIP_HIBOMB, 16.25f, 10.5f, HINT_PANEL);
		break;
	case 5:
		drawPanel("↓", 10.75f, 4.5f, 1.0f, HINT_PANEL);
		drawPanelD(CHIP_BOMB,CHIP_HIBOMB_BOX,18.0f,12.5f,HINT_PANEL);
		break;
	case 7:
		drawPanelD(CHIP_BOMB, CHIP_STEEL_BOX, 14.25f, 5.25f, HINT_PANEL);
		drawArrowHorizontal(14.5f,6.5f,false,HINT_ARROW);
		break;
	case 10:
		drawPanel(CHIP_WOOD_BOX, 6.0f, 10.25f, HINT_PANEL);
		drawArrowHorizontal(5.5f, 11.5f, false, HINT_ARROW);
		drawPanel(CHIP_LEAD_BOX, 15.0f, 8.25f, HINT_PANEL);
		drawArrowHorizontal(14.5f, 9.5f, false, HINT_ARROW);
		break;
	case 11:
		drawPanel("X+C", 20.5f, 4.25f, 2.0f, HINT_PANEL);
		drawArrowHorizontal(20.5f, 5.5f, true, HINT_ARROW);
		drawPanel(CHIP_HAMMER, 17.0f, 9.25f, HINT_PANEL);
		drawArrowHorizontal(16.5f, 10.5f, false, HINT_ARROW);
		break;
	case 15:
		drawArrowVertical(17.0f, 8.0f, false, HINT_ARROW);
		break;
	default:
		break;
	}

	graphics->spriteEnd();
}

//オブジェクトを描画
void AstralDebu::renderObject(){
	graphics->spriteBegin();

	//オブジェクトを描画
	for (entityNS::RENDER_ORDER ro = entityNS::RO_BASE; ro < entityNS::RO_DEBU; ro = static_cast<entityNS::RENDER_ORDER>(ro + 1)){
		ALL_OBJ if ((isMovable(object[i])&&(object[i]->isRenderOrder(ro)))) object[i]->draw();
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

//デバッグ用 判定を描画
void AstralDebu::renderEdge(){
	drawQuad((float)debu->getLeft(true), (float)debu->getTop(false),
		(float)debu->getRight(true) - debu->getLeft(true), (float)debu->getBottom(false) - debu->getTop(false), RED & graphicsNS::ALPHA50);
	drawQuad((float)debu->getLeft(false), (float)debu->getTop(true),
		(float)debu->getRight(false) - debu->getLeft(false), (float)debu->getBottom(true) - debu->getTop(true), BLUE & graphicsNS::ALPHA50);

	ALL_OBJ{
		Entity *e = getObject(i);
		if (isTouchable(e)){
			drawQuad((float)e->getLeft(true), (float)e->getTop(false),
				(float)e->getRight(true) - e->getLeft(true), (float)e->getBottom(false) - e->getTop(false), RED & graphicsNS::ALPHA50);
			drawQuad((float)e->getLeft(false), (float)e->getTop(true),
				(float)e->getRight(false) - e->getLeft(false), (float)e->getBottom(true) - e->getTop(true), BLUE & graphicsNS::ALPHA50);
		}
	}
}

//デバッグ用 判定を描画
void AstralDebu::renderVelocity(){
	const int BUF_SIZE = 10;
	static char buffer[BUF_SIZE];

	graphics->spriteBegin();
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getVelX());
	middleF.print(buffer, VEL_MAR_X, VEL_MAR_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getVelY());
	middleF.print(buffer, VEL_MAR_X + VEL_LEN_X, VEL_MAR_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getMovX());
	middleF.print(buffer, VEL_MAR_X, VEL_MAR_Y + VEL_LEN_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	_snprintf_s(buffer, BUF_SIZE, "%4.3f", debu->getMovY());
	middleF.print(buffer, VEL_MAR_X + VEL_LEN_X, VEL_MAR_Y + VEL_LEN_Y,
		VEL_LEN_X, VEL_LEN_Y, WHITE, DT_RC);
	graphics->spriteEnd();
}

//カーソルを描画
void AstralDebu::drawCursor(){
	chip.setX((float)CHIP(getCursorChipX(debu)));
	chip.setY((float)CHIP_D(getCursorChipY(debu)));

	chip.setCurrentFrame(CHIP_CURSOR);
	chip.draw();
}