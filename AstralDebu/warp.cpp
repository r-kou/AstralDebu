#include "warp.h"

using namespace entityNS;
using namespace warpNS;

//�R���X�g���N�^
Warp::Warp(ENTITY_TYPE t){
	state = STAND;
	//�I�������F�ɂȂ�
	type = t;
	size = IMG_SIZE;
	col = IMG_COL;
	edge_x = EDGE_X;
	edge_y = EDGE_Y;
	margin_x = EDGE_MAR_X;
	margin_y = EDGE_MAR_Y;
	//�F��ς���
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

//�`��
void Warp::draw(){
	//��u��������������
	if (anim_interval <= 0.1f){
		//���݂̏�Ԃ��Z�[�u
		int cur = image.getCurrentFrame();
		//�����摜�ɂ��ĕ`��
		image.setCurrentFrame(IMG_E);
		Entity::draw();
		//�摜��߂�
		image.setCurrentFrame(cur);
		if (anim_interval <= 0.0f) anim_interval = 0.4f;
	}
	else Entity::draw();
}

//����̈ʒu��ݒ�
void Warp::setEdge(){
	//���S����edge�������L�т�
	SETRECT(edge, (long)pos.x - edge_x, (long)pos.y - edge_y + EDGE_SLIDE, edge_x * 2, edge_y * 2 + 1);
}

//�R���X�g���N�^
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

//������
bool Goal::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//�摜�Z�b�g
	if (ret) setImage(IMG_W_S, IMG_W_E, true);
	return ret;
}

//�`��
void Goal::draw(){
	//�y����`��
	int cur = image.getCurrentFrame();
	image.setCurrentFrame(IMG_W_D);
	image.draw();
	image.setCurrentFrame(cur);

	Entity::draw();
}

//����̈ʒu��ݒ�
void Goal::setEdge(){
	//���S����edge�������L�т�
	SETRECT(edge, (long)pos.x - edge_x, (long)pos.y - edge_y + EDGE_SLIDE, edge_x * 2, edge_y * 2 + 1);
}
