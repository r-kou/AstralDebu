#include "blast.h"

using namespace entityNS;
using namespace blastNS;

//コンストラクタ
Blast::Blast(bool b){
	type = BLAST;
	state = STAND;
	renderOrder = RO_BLAST;
	size = IMG_SIZE;
	col = IMG_COL;
	if (b){
		//5*5の爆風
		edgeX = EDGE_LARGE;
		edgeY = EDGE_LARGE;
	}
	else {
		//3*3の爆風
		edgeX = EDGE_SMALL;
		edgeY = EDGE_SMALL;
	}
	marginX = EDGE_MAR;
	marginY = EDGE_MAR;
	//大小の爆風を判別
	stateChanged = b;
}

//初期化
bool Blast::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//画像セット
	image.setCurrentFrame(10);
	setImage(IMG_START, IMG_END, false);
	image.setDelay(0.1f);
	animInterval = 0.7f;
	//爆風の大きさをセット
	if (stateChanged) image.setScale(1.66f);
	return ret;
}

//移動(アニメするだけ)
void Blast::move(float frameTime){
	if (animInterval < 0.7f) state = DEAD;

	Entity::move(frameTime);
}

//地形への接触（何もしない）
void Blast::collideMap(UCHAR t){}

//他オブジェクトへの接触（何もしない）
void Blast::collideObj(Entity *e, UCHAR t){}
