#include "warp.h"

using namespace entityNS;
using namespace warpNS;

//コンストラクタ
Warp::Warp(ENTITY_TYPE t){
	state = STAND;
	//選択した色になる
	type = t;
	size = IMG_SIZE;
	col = IMG_COL;
	edge_x = EDGE_X;
	edge_y = EDGE_Y;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
	//色を変える
	switch (type){
	case WARP_R:
		img = IMG_R;
		break;
	case WARP_G:
		img = IMG_G;
		break;
	case WARP_Y:
		img = IMG_Y;
		break;
	}

}

//描画
void Warp::draw(){
	//一瞬だけ灯が消える
	if (anim_interval <= 0.1f){
		//現在の状態をセーブ
		int cur = image.getCurrentFrame();
		//消灯画像にして描画
		image.setCurrentFrame(IMG_E);
		Entity::draw();
		//画像を戻す
		image.setCurrentFrame(cur);
		if (anim_interval <= 0.0f) anim_interval = 0.4f;
	}
	else Entity::draw();
}

//判定の位置を設定
void Warp::setEdge(){
	//中心からedge分だけ伸びる
	SETRECT(edge, (long)pos.x - edge_x, (long)pos.y - edge_y + EDGE_SLIDE, edge_x * 2, edge_y * 2 + 1);
}

//コンストラクタ
Goal::Goal(){
	state = STAND;
	type = GOAL;
	size = IMG_SIZE;
	col = IMG_COL;
	edge_x = EDGE_X;
	edge_y = EDGE_Y;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
}

//初期化
bool Goal::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//画像セット
	if (ret) setImage(IMG_W_S, IMG_W_E, true);
	return ret;
}

//描画
void Goal::draw(){
	//土台も描画
	int cur = image.getCurrentFrame();
	image.setCurrentFrame(IMG_W_D);
	image.draw();
	image.setCurrentFrame(cur);

	Entity::draw();
}

//判定の位置を設定
void Goal::setEdge(){
	//中心からedge分だけ伸びる
	SETRECT(edge, (long)pos.x - edge_x, (long)pos.y - edge_y + EDGE_SLIDE, edge_x * 2, edge_y * 2 + 1);
}
