#include "blast.h"

using namespace entityNS;
using namespace blastNS;

//�R���X�g���N�^
Blast::Blast(bool b){
	type = TY_BLAST;
	state = ST_STAND;
	renderOrder = RO_BLAST;
	size = BLAST_SIZE;
	col = COL_BLAST;
	if (b){
		//5*5�̔���
		edgeX = EDGE_LARGE;
		edgeY = EDGE_LARGE;
	}
	else {
		//3*3�̔���
		edgeX = EDGE_SMALL;
		edgeY = EDGE_SMALL;
	}
	marginX = EDGE_MAR;
	marginY = EDGE_MAR;
	//�召�̔����𔻕�
	stateChanged = b;
}

//������
bool Blast::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//�摜�Z�b�g
	image.setCurrentFrame(10);
	setImage(IMG_START, IMG_END, false);
	image.setDelay(0.1f);
	animInterval = 0.7f;
	//�����̑傫�����Z�b�g
	if (stateChanged) image.setScale(1.66f);
	return ret;
}

//�ړ�(�A�j�����邾��)
void Blast::move(float frameTime){
	if (animInterval < 0.7f) state = ST_DEAD;

	Entity::move(frameTime);
}