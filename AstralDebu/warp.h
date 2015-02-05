#ifndef _WARP_H
#define _WARP_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace warpNS{
	const int IMG_RED_WARP = 48;
	const int IMG_GREEN_WARP = 49;
	const int IMG_YELLOW_WARP = 50;
	const int IMG_EMPTY_WARP = 51;
	const int IMG_GOAL_BASE = 52;
	const int IMG_GOAL_START = 53;
	const int IMG_GOAL_END = 55;

	const int EDGE_X = 2;
	const int EDGE_Y = 1;
	const int EDGE_MAR_X = 0;
	const int EDGE_MAR_Y = 0;
	const int EDGE_SLIDE = 12;
}

class Warp : public Entity{
public:
	//�R���X�g���N�^
	Warp(entityNS::ENTITY_TYPE t);

	//�`��
	virtual void draw();

	//����̈ʒu��ݒ�
	virtual void setEdge();
};

class Goal : public Entity{
public:
	//�R���X�g���N�^
	Goal();

	//������ �F�����߂�
	bool initialize(Game *game, Texture *t, int i, int j);

	//�`��
	virtual void draw();

	//����̈ʒu��ݒ�
	virtual void setEdge();
};

#endif