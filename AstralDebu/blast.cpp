#include "blast.h"

using namespace entityNS;
using namespace blastNS;

//コンストラクタ
Blast::Blast(bool b){
	type = BLAST;
	state = STAND;
	size = IMG_SIZE;
	col = IMG_COL;
	if (b){
		//5*5の爆風
		edge_x = EDGE_2;
		edge_y = EDGE_2;
	}
	else {
		//3*3の爆風
		edge_x = EDGE_1;
		edge_y = EDGE_1;
	}
	margin_x = EDGE_MAR;
	margin_y = EDGE_MAR;
	//大小の爆風を判別
	state_change = b;
}

//初期化
bool Blast::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//画像セット
	image.setCurrentFrame(10);
	setImage(IMG_S, IMG_E, false);
	image.setDelay(0.1f);
	anim_interval = 0.7f;
	//爆風の大きさをセット
	if (state_change) image.setScale(1.66f);
	return ret;
}

//移動(アニメするだけ)
void Blast::move(float frameTime){
	if (anim_interval < 0.7f) state = DEAD;

	Entity::move(frameTime);
}

//地形への接触（何もしない）
void Blast::collideMap(UCHAR t){}

//他オブジェクトへの接触（何もしない）
void Blast::collideObj(Entity *e, UCHAR t){}
