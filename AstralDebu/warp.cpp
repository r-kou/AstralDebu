#include "warp.h"

using namespace entityNS;
using namespace warpNS;

//�R���X�g���N�^
Warp::Warp(ENTITY_TYPE t){
	state = STAND;
	//�I�������F�ɂȂ�
	type = t;
	renderOrder = RO_BASE;
	size = IMG_SIZE;
	col = IMG_COL;
	edgeX = EDGE_X;
	edgeY = EDGE_Y;
	marginX = EDGE_MAR_X;
	marginY = EDGE_MAR_Y;
	//�F��ς���
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

//�`��
void Warp::draw(){
	//��u��������������
	if (animInterval <= 0.1f){
		//���݂̏�Ԃ��Z�[�u
		int cur = image.getCurrentFrame();
		//�����摜�ɂ��ĕ`��
		image.setCurrentFrame(IMG_EMPTY_WARP);
		Entity::draw();
		//�摜��߂�
		image.setCurrentFrame(cur);
		if (animInterval <= 0.0f) animInterval = 0.4f;
	}
	else Entity::draw();
}

//����̈ʒu��ݒ�
void Warp::setEdge(){
	//���S����edge�������L�т�
	SETRECT(edge, (long)pos.x - edgeX, (long)pos.y - edgeY + EDGE_SLIDE, edgeX * 2, edgeY * 2 + 1);
}

//�R���X�g���N�^
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

//������
bool Goal::initialize(Game *game, Texture *t, int i, int j){
	bool ret = false;
	ret = Entity::initialize(game, t, i, j);
	//�摜�Z�b�g
	if (ret) setImage(IMG_GOAL_START, IMG_GOAL_END, true);
	return ret;
}

//�`��
void Goal::draw(){
	//�y����`��
	int cur = image.getCurrentFrame();
	image.setCurrentFrame(IMG_GOAL_BASE);
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
	image.setCurrentFrame(cur);

	Entity::draw();
}

//����̈ʒu��ݒ�
void Goal::setEdge(){
	//���S����edge�������L�т�
	SETRECT(edge, (long)pos.x - edgeX, (long)pos.y - edgeY + EDGE_SLIDE, edgeX * 2, edgeY * 2 + 1);
}
