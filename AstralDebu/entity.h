#ifndef _ENTITY_H
#define _ENTITY_H
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "graphics.h"
#include "input.h"
#include "texture.h"
#include "audio.h"
#include "game.h"

namespace entityNS{
	enum ENTITY_TYPE {
		TY_NONE, TY_DEBU,
		TY_WOOD_BOX, TY_STEEL_BOX,
		TY_LEAD_BOX, TY_BOMB_BOX,
		TY_HIBOMB_BOX, TY_AIR_BOX,
		TY_FRAME_BOX, TY_GOAST_BOX,
		TY_BOMB, TY_HIBOMB,
		TY_MINE, TY_ROCK,
		TY_HAMMER, TY_MEAT,
		TY_HIMEAT, TY_ENEMY_1,
		TY_ENEMY_2, TY_ENEMY_3,
		TY_ENEMY_4, TY_ENEMY_5,
		TY_BULLET, TY_MISSILE,
		TY_RED_WARP, TY_GREEN_WARP,
		TY_YELLOW_WARP, TY_LADDER,
		TY_GOAL, TY_BLAST
	};
	enum ENTITY_STATE {
		ST_EMPTY, ST_LOCK,
		ST_STAND, ST_JUMP,
		ST_CLEAR, ST_KNOCK,
		ST_DEAD, ST_LADDER,
		ST_HAMMER, ST_ACTION
	};
	enum RENDER_ORDER {
		RO_BASE, RO_BOX,
		RO_OBJECT, RO_ENEMY,
		RO_BLAST, RO_HOLD,
		RO_DEBU, RO_HAMMER
	};
	//�Փ˔���̏㉺���E
	const UCHAR LEFT = 1;
	const UCHAR TOP = 2;
	const UCHAR RIGHT = 4;
	const UCHAR BOTTOM = 8;
	//�Փ˔���̎l��
	const UCHAR EDGE1 = LEFT + TOP;
	const UCHAR EDGE2 = RIGHT + TOP;
	const UCHAR EDGE3 = RIGHT + BOTTOM;
	const UCHAR EDGE4 = LEFT + BOTTOM;
	//�����摜�̃`�b�v�̑傫��
	const int BLAST_SIZE = 96;
	//�e�摜�̉��`�b�v��
	const int COL_CHIP = 8;
	const int COL_DEBU = 7;
	const int COL_BLAST = 6;
	const int COL_ENEMY = 7;
	//edge�̍ő�l
	const int EDGE_MAX = 16;
	//�������x
	const float GRAVITY_RATE = 1000.0f;
	//�Ԃ��������̗����␳
	const float TOP_GRAVITY_RATE = 1000.0f;
	//������ю��̔����ȕ���
	const float VEL_KNOCK_JUMP = 150;
	//������ю��̏�����ւ̍ő呬�x(KNOCK_JUMP������)
	const float VEL_MAX_JUMP = 250;
	//������ю��̍ő呬�x
	const float VEL_MAX = 400;
	//�����̑Ή��l
	const UINT RES_LEFT = 0;
	const UINT RES_RIGHT = 1;
	const UINT RES_TOP = 2;
	const UINT RES_BOTTOM = 3;
	const UINT RES_BOTTOM_CHIP = 4;
	const UINT RES_STOP = 5;
	const UINT RES_CHIP = 6;
	const UINT RES_COLLIDE = 7;
	const UINT RES_WARP = 8;
	const UINT RES_KNOCK = 9;
	const UINT RES_JUMP = 10;
	const UINT RES_DEAD = 11;
	const UINT RES_LADDER = 12;
	const UINT RES_CLEAR = 13;
}

class Entity{
protected:
	//�O���t�B�b�N�X�|�C���^
	Graphics *graphics;
	//�����|�C���^
	Audio *audio;
	//�g���摜
	Texture *texture;
	Image image;

	//���
	entityNS::ENTITY_STATE state;
	//����
	entityNS::ENTITY_TYPE type;
	//�`�揇
	entityNS::RENDER_ORDER renderOrder;
	//���W
	VC2 pos;
	//���x
	VC2 vel;
	//�ړ�����
	VC2 mov;
	//����
	bool direct;
	//�傫���@xy����
	int size;
	//�\������摜
	int img;
	//�Փ˔���p�̒Z�a
	RECT edge;
	//�Z�a�̕�
	int edgeX, edgeY;
	//���e�]��
	int marginX, marginY;
	//���[�v�̓]�ڐ�@���[�v��p
	int partnerX, partnerY;
	//�s��
	int col;
	//�t���[������
	float frameTime;
	//�ėp�C���^�[�o��
	float animInterval;
	//�]�ڃC���^�[�o��
	float warpInterval;
	//������їp�C���^�[�o��
	float knockInterval;
	//��Ԃ��ύX���ꂽ��
	bool stateChanged;
	//���E�̏��ւ̏Փ˔���
	bool bottomObj[2];
	//����s����������
	bool action;
	//�I�u�W�F�N�g�ւ̔����̔���
	int response;
	//�I�u�W�F�N�g�ւ̔����̃I�v�V����
	VC2 responseVC[32];
	//�u�������̉�
	std::string putSound;
	//�j�󎞂̉�
	std::string deadSound;
	//�������ɂ��邩
	bool trans;
	//���������
	bool fall;

	//���x�̍������߂�
	float diffVelX(Entity *e) { return vel.x - e->getVelX(); }
	float diffVelY(Entity *e) { return vel.y - e->getVelY(); }
	//����̍������߂�@true�Ȃ�}�[�W���L
	long diffLeft(Entity *e, bool m) { return getLeft(m) - e->getRight(m); }
	long diffRight(Entity *e, bool m) { return getRight(m) - e->getLeft(m); }
	long diffTop(Entity *e, bool m) { return getTop(m) - e->getBottom(m); }
	long diffBottom(Entity *e, bool m) { return getBottom(m) - e->getTop(m); }
	//�����łǂ��ɔ�Ԃ������߂�
	bool blastLeft(Entity *e) { return (getPosX() - e->getPosX() < -8); }
	bool blastRight(Entity *e) { return (getPosX() - e->getPosX() > 8); }
	bool blastTop(Entity *e) { return (getPosY() - e->getPosY() < -8); }
	bool blastBottom(Entity *e) { return (getPosY() - e->getPosY() > 8); }
	float blastX(Entity *e, float x) { if (blastLeft(e)) return -x; else if (blastRight(e)) return x; else return 0; }
	float blastY(Entity *e, float y) { if (blastTop(e)) return -y; else if (blastBottom(e)) return y; else return 0; }
	//�I�u�W�F�N�g�����̉��Z
	UINT shift(int i) { return 1 << i; }
	const UINT getRes(int i) { return response & shift(i); }
	const float getResX(int i) { return responseVC[i].x; }
	const float getResY(int i) { return responseVC[i].y; }
	void setRes(int i) { response |= shift(i); }
	void setRes(int i, float x, float y) { response |= shift(i); responseVC[i].x += x; responseVC[i].y += y; }
	//�e�l�̍ő�l��ݒ�
	const float setLimit(float n, float max) { return ((n>max) ? max : ((n<-max) ? -max : n)); }

public:
	//�R���X�g���N�^
	Entity();
	//�f�X�g���N�^
	virtual ~Entity();

	//������
	virtual bool initialize(Game *game, Texture *t, int i, int j);

	//�ړ�
	virtual void move(float frameTime);

	//�n�`�ւ̐ڐG����
	virtual void touchMap(int map[MAP_COL][MAP_ROW]);

	//�㉺���E�̏Փ˔���
	virtual UCHAR touchMapDirect(int c, UCHAR t) { return c ? t : 0; }

	//�n�`�ւ̐ڐG
	virtual void collideMap(UCHAR t);

	//���I�u�W�F�N�g�ւ̐ڐG����
	virtual void touchObj(Entity *e);

	//�e�����ւ̔��肪�d�Ȃ��Ă��邩�𒲂ׂ�
	virtual bool touchObjDirect(long x, long y, float v);

	//���I�u�W�F�N�g�ւ̐ڐG
	virtual void collideObj(Entity *e, UCHAR t);

	//���I�u�W�F�N�g�ւ̔���
	virtual void responseObj();

	//�`�悷��摜��ύX
	virtual void changeImage();

	//�摜�ύX�̈ꊇ�ݒ�
	virtual void setImage(int s, int e, bool l);

	//�ꖇ�G�̐ݒ�
	virtual void setImage(int c);

	//�`��
	virtual void draw();

	//����̈ʒu��ݒ�
	virtual void setEdge();

	//���S����
	virtual bool isDead() { return (state == entityNS::ST_DEAD) && (animInterval == 0.0f); }

	//���̐ڒn��������߂�
	bool collideBottomLeft(Entity *e) { return (getLeft(true) >= e->getLeft(false)) && (getLeft(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//�E�̐ڒn��������߂�
	bool collideBottomRight(Entity *e) { return (getRight(true) >= e->getLeft(false)) && (getRight(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//���E�̐ڒn����
	void checkBottom(bool d, Entity *e);

	//���n����炷
	void playPut() { if (putSound != "") audio->playCue(putSound.c_str()); }

	//���S����炷
	void playDead() { if (deadSound != "") audio->playCue(deadSound.c_str()); }

	//getter
	const entityNS::ENTITY_STATE getState() { return state; }
	const entityNS::ENTITY_TYPE getType() { return type; }
	const entityNS::RENDER_ORDER getRenderOrder() { return renderOrder; }
	const VC2* getPos(){ return &pos; }
	const VC2* getVel(){ return &vel; }
	const VC2* getMov(){ return &mov; }
	const float getPosX() { return pos.x; }
	const float getPosY() { return pos.y; }
	const float getVelX() { return vel.x; }
	const float getVelY() { return vel.y; }
	const float getMovX() { return mov.x; }
	const float getMovY() { return mov.y; }
	const bool getDirect() { return direct; }
	const int getSize() { return size; }
	const RECT& getEdge() { return edge; }
	const int getEdgeX() { return edgeX; }
	const int getEdgeY() { return edgeY; }
	const bool getComplete() { return image.getComplete(); }
	const int getMarginX() { return marginX; }
	const int getMarginY() { return marginY; }
	const int getPartnerX() { return partnerX; }
	const int getPartnerY() { return partnerY; }
	const float getAnim() { return animInterval; }
	const float getWarp() { return warpInterval; }
	const bool getAction() { return action; }
	const int getResponse() { return response; }
	const std::string getPut() { return putSound; }

	//�}�b�v��ł̍��W��Ԃ�
	const int ChipX(float x) { return (int)x / CHIP_SIZE; }
	const int ChipY(float y) { return (int)(y - DATA_LEN) / CHIP_SIZE; }
	const int ChipCX() { return ChipX(pos.x); }
	const int ChipCY() { return ChipY(pos.y); }
	//�����蔻��̎������̂ŒZ�k�p
	const long getLeft(bool b) { return edge.left + (b ? marginX - (int)mov.x : 0); }
	const long getRight(bool b) { return edge.right - (b ? marginX + (int)mov.x : 0); }
	const long getTop(bool b) { return edge.top + (b ? marginY - (int)mov.y : 0); }
	const long getBottom(bool b) { return edge.bottom - (b ? marginY + (int)mov.y : 0); }

	//setter
	void setState(entityNS::ENTITY_STATE s) { state = s; }
	void setType(entityNS::ENTITY_TYPE t) { type = t; }
	void setRenderOrder(entityNS::RENDER_ORDER o) { renderOrder = o; }
	void setPosX(float n) { pos.x = n; }
	void setPosY(float n) { pos.y = n; }
	void setVelX(float n) { vel.x = n; }
	void setVelY(float n) { vel.y = n; }
	void setMovX(float n) { mov.x = n; }
	void setMovY(float n) { mov.y = n; }
	void setDirect(bool d){ direct = d; }
	void setChange(bool s) { stateChanged = s; }
	void setPartnerX(int x) { partnerX = x; }
	void setPartnerY(int y) { partnerY = y; }
	void setAnim(float a) { animInterval = a; }
	void setWarp(float i) { warpInterval = i; }
	void setAction(bool b) { action = b; }
	//������������
	void resetResponse() { response = 0; FOR(entityNS::RES_CLEAR){ responseVC[i].x = 0; responseVC[i].y = 0; } }
	//���[�v�����ݒ�
	void setpartner(Entity *e) {partnerX = e->ChipCX(); partnerY = e->ChipCY();	}

	//�`�揇���w��
	bool isRenderOrder(entityNS::RENDER_ORDER ro){ return renderOrder == ro; }
	bool inChip(int x, int y) { return ((ChipCX() == x) && (ChipCY() == y)); }
	//�}�b�v�`�b�v�ɍ��킹��
	void setCX() { pos.x = (float)CHIP(ChipCX() + 0.5f); }
	void setCY() { pos.y = (float)CHIP_D(ChipCY() + 0.5f); }
	//��������肬��ɗ}����
	void setLeft(bool b) { pos.x = (float)CHIP(ChipCX()) + edgeX - (b ? marginX : 0); }
	void setRight(bool b) { pos.x = (float)CHIP(ChipCX() + 1) - edgeX + (b ? marginX : 0); }
	void setTop(bool b) { pos.y = (float)CHIP_D(ChipCY()) + edgeY - (b ? marginY : 0); }
	void setBottom(bool b) { pos.y = (float)CHIP_D(ChipCY() + 1) - edgeY + (b ? marginY : 0); }
};


#endif