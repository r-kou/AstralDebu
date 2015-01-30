#include "warp.h"

using namespace entityNS;
using namespace warpNS;

//コンストラクタ
Warp::Warp(ENTITY_TYPE t){
	state = STAND;
	//選択した色になる
	type = t;
	renderOrder = RO_BASE;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	//色を変える
	switch (type){
	case RED_WARP:
		img = IMG_RED_WARP;
		break;
	case GREEN_WARP:
		img = IMG_GREEN_WARP;
		break;
	case YELLOW_WARP:
		img = IMG_YELLOW_WARP;
		break;
	}

}

//描画
void Warp::draw(){
	//一瞬だけ灯が消える
	if (animInterval <= 0.1f){
		//現在の状態をセーブ
		int cur = image.getCurrentFrame();
		//消灯画像にして描画
		image.setCurrentFrame(IMG_EMPTY_WARP);
		Entity::draw();
		//画像を戻す
		image.setCurrentFrame(cur);
		if (animInterval <= 0.0f) animInterval = 0.4f;
	}
	else Entity::draw();
}

//判定の位置を設定
void Warp::setEdge(){
	//中心からedge分だけ伸びる
	SETRECT(edge, (long)pos.x - edgeX, (long)pos.y - edgeY + EDGE_SLIDE, edgeX * 2, edgeY * 2 + 1);
}

//コンストラクタ
Goal::Goal(){
	state = STAND;
	type = GOAL;
	renderOrder = RO_BASE;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
}

//初期化
bool Goal::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//画像セット
	if (ret) setImage(IMG_GOAL_START, IMG_GOAL_END, true);
	return ret;
}

//描画
void Goal::draw(){
	//土台も描画
	int cur = image.getCurrentFrame();
	image.setCurrentFrame(IMG_GOAL_BASE);
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	image.setCurrentFrame(cur);

	Entity::draw();
}

//判定の位置を設定
void Goal::setEdge(){
	//中心からedge分だけ伸びる
	SETRECT(edge, (long)pos.x - edgeX, (long)pos.y - edgeY + EDGE_SLIDE, edgeX * 2, edgeY * 2 + 1);
}
