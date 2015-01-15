#include "blast.h"

using namespace entityNS;
using namespace blastNS;

//�R���X�g���N�^
Blast::Blast(bool b){
	type = BLAST;
	state = STAND;
	size = IMG_SIZE;
	col = IMG_COL;
	if (b){
		//5*5�̔���
		edge_x = EDGE_2;
		edge_y = EDGE_2;
	}
	else {
		//3*3�̔���
		edge_x = EDGE_1;
		edge_y = EDGE_1;
	}
	margin_x = EDGE_MAR;
	margin_y = EDGE_MAR;
	//�召�̔����𔻕�
	state_change = b;
}

//������
bool Blast::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//�摜�Z�b�g
	image.setCurrentFrame(10);
	setImage(IMG_S, IMG_E, false);
	image.setDelay(0.1f);
	anim_interval = 0.7f;
	//�����̑傫�����Z�b�g
	if (state_change) image.setScale(1.66f);
	return ret;
}

//�ړ�(�A�j�����邾��)
void Blast::move(float frameTime){
	if (anim_interval < 0.7f) state = DEAD;

	Entity::move(frameTime);
}

//�n�`�ւ̐ڐG�i�������Ȃ��j
void Blast::collideMap(UCHAR t){}

//���I�u�W�F�N�g�ւ̐ڐG�i�������Ȃ��j
void Blast::collideObj(Entity *e, UCHAR t){}
