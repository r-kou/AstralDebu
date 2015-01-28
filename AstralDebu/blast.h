#ifndef _BLAST_H
#define _BLAST_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace blastNS{
	const int IMG_SIZE = 96;
	const int IMG_COL = 6;
	const int IMG_START = 0;
	const int IMG_END = 5;

	//������Ƃ��������������
	const int EDGE_SMALL = 45;
	const int EDGE_LARGE = 75;
	const int EDGE_MAR = 0;
}

class Blast : public Entity{
public:
	//�R���X�g���N�^
	Blast(bool b);

	//������
	bool initialize(Game *game, Texture *t, int i, int j);

	//�ړ�
	virtual void move(float frameTime);

	//�n�`�ւ̐ڐG�i�������Ȃ��j
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG�i�������Ȃ��j
	virtual void collideObj(Entity *e, UCHAR t);

};

#endif