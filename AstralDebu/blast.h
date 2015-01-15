#ifndef _BLAST_H
#define _BLAST_H
#define WIN32_LEAN_AND_MEAN

#include "entity.h"

namespace blastNS{
	const int IMG_SIZE = 96;
	const int IMG_COL = 6;
	const int IMG_S = 0;
	const int IMG_E = 5;

	//ちょっとだけ判定を小さく
	const int EDGE_1 = 45;
	const int EDGE_2 = 75;
	const int EDGE_MAR = 0;
}

class Blast : public Entity{
public:
	//コンストラクタ
	Blast(bool b);

	//初期化
	bool initialize(Game *game, Texture *t, int i, int j);

	//移動
	virtual void move(float frameTime);

	//地形への接触（何もしない）
	virtual void collideMap(UCHAR t);

	//他オブジェクトへの接触（何もしない）
	virtual void collideObj(Entity *e, UCHAR t);

};

#endif