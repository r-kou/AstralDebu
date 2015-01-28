#ifndef _WARP_H
#define _WARP_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace warpNS{
	const int IMG_RED_WARP = 35;
	const int IMG_GREEN_WARP = 36;
	const int IMG_YELLOW_WARP = 37;
	const int IMG_EMPTY_WARP = 38;
	const int IMG_GOAL_BASE = 41;
	const int IMG_GOAL_START = 42;
	const int IMG_GOAL_END = 44;

	const int IMG_SIZE = 32;
	const int IMG_COL = 10;
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