#ifndef _ENTITY_H
#define _ENTITY_H
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "graphics.h"
#include "input.h"
#include "texture.h"
#include "game.h"

namespace entityNS{
	enum ENTITY_TYPE {
		NONE, DEBU,
		BOX_W, BOX_S,
		BOX_L, BOX_B,
		BOX_H, BOX_A,
		BOX_F, BOMB,
		HIBOMB, ROCK,
		HAMMER, MEAT,
		HIMEAT, EN_1,
		EN_2, EN_3,
		EN_4, EN_5,
		EN_B, EN_M,
		WARP_R, WARP_G,
		WARP_Y, GOAL,
		BLAST
	};
	enum ENTITY_STATE {
		EMPTY, LOCK,
		STAND, JUMP,
		CLEAR, KNOCK,
		DEAD, LADDER,
		HOLD_HAMMER,
		ACT
	};
	//�Փ˔���̏㉺���E
	const UCHAR LEFT = 1;
	const UCHAR TOP = 2;
	const UCHAR RIGHT = 4;
	const UCHAR BOTTOM = 8;
	const UCHAR GETLADDER = 16;
	//�Փ˔���̎l��
	const UCHAR EDGE1 = LEFT + TOP;
	const UCHAR EDGE2 = RIGHT + TOP;
	const UCHAR EDGE3 = RIGHT + BOTTOM;
	const UCHAR EDGE4 = LEFT + BOTTOM;
	//�͂���
	const int CHIP_LADDER = 30;
	//edge�̍ő�l
	const int EDGE_MAX = 16;
	//�������x
	const float GRAVITY_RATE = 1000.0f;
	//�������x�̍ő�
	const float VEL_FALL_MAX = 300;

}

class Entity{
protected:
	//�O���t�B�b�N�X�|�C���^
	Graphics *graphics;
	//�g���摜
	Texture *texture;
	Image image;

	//���
	entityNS::ENTITY_STATE state;
	//����
	entityNS::ENTITY_TYPE type;

	//���W
	VC2 pos;
	//���x
	VC2 vel;
	//����
	bool direct;
	//�傫���@xy����
	int size;
	//�\������摜
	int img;
	//�Փ˔���p�̒Z�a
	RECT edge;
	//�Z�a�̕�
	int edge_x, edge_y;
	//���e�]��
	int margin_x, margin_y;
	//���[�v�̓]�ڐ�@���[�v��p
	int partnerX, partnerY;
	//�s��
	int col;
	//�ėp�C���^�[�o��
	float anim_interval;
	//�]�ڃC���^�[�o��
	float warp_interval;
	//��Ԃ��ύX���ꂽ��
	bool state_change;
	//���E�̏��ւ̏Փ˔���
	bool bottom_object[2];
	//����s����������
	bool action;
	//�I�u�W�F�N�g�ւ̔����̔���
	INT response;
	//�I�u�W�F�N�g�ւ̔����̃I�v�V����
	VC2 responseVC[32];

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
	//�I�u�W�F�N�g�����̉��Z
	UINT shift(int i) { return 1 << i; }
	const UINT getRes(int i) { return response & shift(i); }
	const float getResX(int i) { return responseVC[i].x; }
	const float getResY(int i) { return responseVC[i].y; }
	void setRes(int i) { response |= shift(i); }
	void setRes(int i, float x, float y) { response |= shift(i); responseVC[i].x = x; responseVC[i].y = y; }
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
	virtual UCHAR touchMapDirect(int c, UCHAR t);

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
	virtual bool isDead() { return (state == entityNS::DEAD) && (anim_interval == 0.0f); }

	//���̐ڒn��������߂�
	bool collideBottomLeft(Entity *e) { return (getLeft(true) >= e->getLeft(false)) && (getLeft(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//�E�̐ڒn��������߂�
	bool collideBottomRight(Entity *e) { return (getRight(true) >= e->getLeft(false)) && (getRight(true) <= e->getRight(false)) && (getBottom(false) >= e->getTop(false)); }

	//���E�̐ڒn����
	void bottomObj(bool d, Entity *e);

	//getter
	const entityNS::ENTITY_STATE getState() { return state; }
	const entityNS::ENTITY_TYPE getType() { return type; }
	const VC2* getPos(){ return &pos; }
	const VC2* getVel(){ return &vel; }
	const float getPosX() { return pos.x; }
	const float getPosY() { return pos.y; }
	const float getVelX() { return vel.x; }
	const float getVelY() { return vel.y; }
	const bool getDirect() { return direct; }
	const int getSize() { return size; }
	const RECT& getEdge() { return edge; }
	const int getEdgeX() { return edge_x; }
	const int getEdgeY() { return edge_y; }
	const bool getComplete() { return image.getComplete(); }
	const int getMarginX() { return margin_x; }
	const int getMarginY() { return margin_y; }
	const int getPartnerX() { return partnerX; }
	const int getPartnerY() { return partnerY; }
	const float getAnim() { return anim_interval; }
	const float getWarp() { return warp_interval; }
	const bool getAction() { return action; }
	const UINT getResponse() { return response; }

	//�}�b�v��ł̍��W��Ԃ�
	const int ChipX(float x) { return (int)x / CHIP_SIZE; }
	const int ChipY(float y) { return (int)(y - DATA_LEN) / CHIP_SIZE; }
	const int ChipCX() { return ChipX(pos.x); }
	const int ChipCY() { return ChipY(pos.y); }
	//�����蔻��̎������̂ŒZ�k�p
	const long getLeft(bool b) { return edge.left + (b ? margin_x : 0); }
	const long getRight(bool b) { return edge.right - (b ? margin_x : 0); }
	const long getTop(bool b) { return edge.top + (b ? margin_y : 0); }
	const long getBottom(bool b) { return edge.bottom - (b ? margin_y : 0); }

	//setter
	void setState(entityNS::ENTITY_STATE s) { state = s; }
	void setType(entityNS::ENTITY_TYPE t) { type = t; }
	void setPosX(float n) { pos.x = n; }
	void setPosY(float n) { pos.y = n; }
	void setVelX(float n) { vel.x = n; }
	void setVelY(float n) { vel.y = n; }
	void setDirect(bool d){ direct = d; }
	void setChange(bool s) { state_change = s; }
	void setPartnerX(int x) { partnerX = x; }
	void setPartnerY(int y) { partnerY = y; }
	void setAnim(float a) { anim_interval = a; }
	void setWarp(float i) { warp_interval = i; }
	void setAction(bool b) { action = b; }
	void resetResponse() { response = 0; }

	//�}�b�v�`�b�v�ɍ��킹��
	void setCX() { pos.x = (float)(ChipCX() + 0.5f) * CHIP_SIZE; }
	void setCY() { pos.y = (float)(ChipCY() + 0.5f) * CHIP_SIZE + DATA_LEN; }
	//��������肬��ɗ}����
	void setLeft(bool b) { pos.x = (float)ChipCX() * CHIP_SIZE + edge_x - (b ? margin_x : 0); }
	void setRight(bool b) { pos.x = (float)(ChipCX() + 1) * CHIP_SIZE - edge_x + (b ? margin_x : 0); }
	void setTop(bool b) { pos.y = (float)ChipCY() * CHIP_SIZE + DATA_LEN + edge_y - (b ? margin_y : 0); }
	void setBottom(bool b) { pos.y = (float)(ChipCY() + 1) * CHIP_SIZE + DATA_LEN - edge_y + (b ? margin_y : 0); }
};


#endif