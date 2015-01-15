#include "entity.h"

using namespace entityNS;

//�R���X�g���N�^
Entity::Entity(){
	state = EMPTY;
	type = NONE;
	pos.x = 0.0f;
	pos.y = 0.0f;
	vel.x = 0.0f;
	vel.y = 0.0f;
	direct = false;
	size = 2;
	img = 0;
	SETRECT(edge, 0, 0, 0, 0);
	edge_x = 0;
	edge_y = 0;
	margin_x = 0;
	margin_y = 0;
	col = 1;
	anim_interval = 0;
	warp_interval = 0;
	bottom_object[0] = false;
	bottom_object[1] = false;
	action = false;
	response = 0;
	FOR(32) {
		responseVC[i].x = 0;
		responseVC[i].y = 0;
	}
}

//�f�X�g���N�^
Entity::~Entity(){
}

//������
bool Entity::initialize(Game *game, Texture *t, int i, int j){
	try{
		//�O���t�B�b�N�Ƃ����낢�돉����
		graphics = game->getGraphics();
		texture = t;
		image.initialize(graphics, texture, size, size, col);
		image.setDelay(0.2f);
		setImage(img);
		pos.x = (float)(i + 0.5f)*CHIP_SIZE;
		pos.y = (float)(j + 0.5f)*CHIP_SIZE + DATA_LEN;
		vel.x = 0.0f;
		vel.y = 0.0f;
		direct = false;
		SETRECT(edge, (long)pos.x - edge_x, (long)pos.y - edge_y, edge_x * 2, edge_y * 2);
		state = STAND;
		anim_interval = 0;
		warp_interval = 0;
	}
	catch (...) { return false; }
	return true;
}

//�ړ�
void Entity::move(float frameTime){
	//���x�������ړ�
	pos.x += (vel.x * frameTime);
	pos.y += (vel.y * frameTime);

	//���E�̗������������
	bottom_object[0] = false;
	bottom_object[1] = false;

	//�C���^�[�o��������Ȃ猸�Z
	if (warp_interval > 0.0f) {
		warp_interval -= frameTime;
		if (warp_interval <= 0.0f) warp_interval = 0.0f;
	}
	if (anim_interval > 0.0f) {
		anim_interval -= frameTime;
		if (anim_interval <= 0.0f) anim_interval = 0.0f;
	}

	//�ړ���ɔ����ݒ�
	setEdge();
	//�A�j���[�V�����Ƃ����X�V
	image.update(frameTime);
}

//�n�`�ւ̐ڐG����
void Entity::touchMap(int map[MAP_COL][MAP_ROW]){
	UCHAR t = 0, t_o = 0;
	int c_l = ChipX((float)getLeft(false)), c_ln = ChipX((float)getLeft(true)),
		c_r = ChipX((float)getRight(false)), c_rn = ChipX((float)getRight(true)),
		c_t = ChipY((float)getTop(false)), c_tn = ChipY((float)getTop(true)),
		c_b = ChipY((float)getBottom(false)), c_bn = ChipY((float)getBottom(true));

	//��ʊO������
	if (pos.x <= edge_x) t_o |= LEFT;
	if (pos.x >= WINDOW_W - edge_x) t_o |= RIGHT;
	if (pos.y <= DATA_LEN + edge_y) t_o |= TOP;
	if (pos.y >= WINDOW_H - edge_y) t_o |= BOTTOM;

	//�l���̃}�b�v������
	if ((t_o & EDGE1) == 0) {
		t |= touchMapDirect(map[c_l][c_tn], LEFT);
		t |= touchMapDirect(map[c_ln][c_t], TOP);
	}
	if ((t_o & EDGE2) == 0) {
		t |= touchMapDirect(map[c_r][c_tn], RIGHT);
		t |= touchMapDirect(map[c_rn][c_t], TOP);
	}
	if ((t_o & EDGE3) == 0) {
		t |= touchMapDirect(map[c_r][c_bn], RIGHT);
		UCHAR tmp = touchMapDirect(map[c_rn][c_b], BOTTOM);
		if (tmp == BOTTOM) bottom_object[1] = true;
		t |= tmp;
	}
	if ((t_o & EDGE4) == 0) {
		t |= touchMapDirect(map[c_l][c_bn], LEFT);
		UCHAR tmp = touchMapDirect(map[c_ln][c_b], BOTTOM);
		if (tmp == BOTTOM) bottom_object[0] = true;;
		t |= tmp;
	}

	//�Փ˔���
	collideMap(t | t_o);
}

//�㉺���E�̏Փ˔���
UCHAR Entity::touchMapDirect(int c, UCHAR t){
	//�n�`�Ȃ�Փ�
	if ((c == 0) || (c == CHIP_LADDER)) return 0;
	else {
		return t;
	}
}

//�n�`�ւ̐ڐG
void Entity::collideMap(UCHAR t){
	//�㉺���E�ɉ����Ĉʒu�Ƒ��x��␳
	if ((t & LEFT) && (vel.x <= 0.0f)) {
		setLeft(false);
		vel.x = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if ((t & RIGHT) && (vel.x >= 0.0f)) {
		setRight(false);
		vel.x = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if ((t & TOP) && (vel.y <= 0.0f)) {
		setTop(false);
		vel.y = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if (t & BOTTOM) {
		//�󒆂ɂ����璅�n����
		if (((vel.y >= 0.0f) && (state == JUMP || state == LADDER)) ||
			((vel.y > 0.0f) && (state == KNOCK || state == DEAD))){
			//�����Ă��璅�n�@����ł����~
			if (state == DEAD) vel.x = 0.0f;
			else state = STAND;
			setBottom(false);
			vel.y = 0.0f;
		}
	}
	else if ((state != KNOCK) && (state != DEAD)) {
		//���ɉ����Ȃ��Ȃ痎��
		state = JUMP;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG����
void Entity::touchObj(Entity *e){
	UCHAR t = 0, et = 0;

	//�݂��̈ʒu�ɍ��킹�Ďl���Ō���
	if (pos.y > e->getPosY()){
		if (pos.x > e->getPosX()){
			//edge1
			if (touchObjDirect(diffLeft(e, false), diffTop(e, true), diffVelX(e))){
				t |= LEFT;
				et |= RIGHT;
			}
			if (touchObjDirect(diffLeft(e, true), diffTop(e, false), diffVelY(e))){
				t |= TOP;
				et |= BOTTOM;
			}
		}
		else {
			//edge2
			if (touchObjDirect(-diffRight(e, false), diffTop(e, true), -diffVelX(e))){
				t |= RIGHT;
				et |= LEFT;
			}
			if (touchObjDirect(-diffRight(e, true), diffTop(e, false), diffVelY(e))){
				t |= TOP;
				et |= BOTTOM;
			}
		}
		if (e->collideBottomLeft(this))
			e->bottomObj(false, this);
		if (e->collideBottomRight(this))
			e->bottomObj(true, this);
	}
	else {
		if (pos.x > e->getPosX()){
			//edge4
			if (touchObjDirect(diffLeft(e, false), -diffBottom(e, true), diffVelX(e))){
				t |= LEFT;
				et |= RIGHT;
			}
			if (touchObjDirect(diffLeft(e, true), -diffBottom(e, false), -diffVelY(e))){
				t |= BOTTOM;
				et |= TOP;
			}
		}
		else {
			//edge3
			if (touchObjDirect(-diffRight(e, false), -diffBottom(e, true), -diffVelX(e))){
				t |= RIGHT;
				et |= LEFT;
			}
			if (touchObjDirect(-diffRight(e, true), -diffBottom(e, false), -diffVelY(e))){
				t |= BOTTOM;
				et |= TOP;
			}
		}
		if (collideBottomLeft(e))
			bottomObj(false, e);
		if (collideBottomRight(e))
			bottomObj(true, e);
	}

	//�Փ˔���
	if (t != 0){
		collideObj(e, t);
		e->collideObj(this, et);
	}
}

//�e�����ւ̔��肪�d�Ȃ��Ă��邩�𒲂ׂ�
bool Entity::touchObjDirect(long x, long y, float v){
	return ((x <= 0) && (y <= 0) && (v <= 0));
}

//���I�u�W�F�N�g�ւ̐ڐG
void Entity::collideObj(Entity *e, UCHAR t){
	switch (e->getType()){
	case ROCK:
		//����ǂȂ̂ŕǂƓ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(0);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(1);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(2);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == JUMP || state == LADDER)) ||
			((diffVelY(e) > 0) && (state == KNOCK)))) setRes(3); //�󒆂ɂ����璅�n����
		break;
	case WARP_R:
	case WARP_G:
	case WARP_Y:
		//���[�v�͔���������ԓ�����
		if (warp_interval == 0.0f) setRes(5,(e->getPartnerX() + 0.5f) * CHIP_SIZE,(e->getPartnerY() + 0.5f) * CHIP_SIZE + DATA_LEN);
		break;
	}
}

//���I�u�W�F�N�g�ւ̔���
void Entity::responseObj(){
	if (getRes(0)) {
		//���ɏՓ�
		setLeft(false);
		vel.x = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if (getRes(1)) {
		//�E�ɏՓ�
		setRight(false);
		vel.x = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if (getRes(2)) {
		//��ɏՓ�
		setTop(false);
		vel.y = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if (getRes(3)) {
		//���ɏՓ�
		setBottom(false);
		vel.y = 0.0f;
		state = STAND;
	}
	if (getRes(4)) {
		//���ɏՓˁ@�}�X�ɂ҂�����
		setCX();
		setCY();
		vel.y = 0.0f;
		state = STAND;
	}
	if (getRes(5)){
		//���[�v
		warp_interval = 0.5f;
		pos.x = getResX(5);
		pos.y = getResY(5);
	}
	if (getRes(6)){
		//���S
		state = DEAD;
	}
}


//�`�悷��摜��ύX
void Entity::changeImage(){}

//�摜�ύX�̈ꊇ�ݒ�
void Entity::setImage(int s, int e, bool l){
	//�摜��ݒ肷��
	if (image.getCurrentFrame() < s ||
		image.getCurrentFrame() > e){
		image.setFrames(s, e);
		image.setCurrentFrame(s);
		image.setLoop(l);
		image.setComplete(false);
	}
}

//�ꖇ�G�̐ݒ�
void Entity::setImage(int c){
	//�摜��ݒ肷��
	if (image.getCurrentFrame() != c){
		image.setFrames(c, c);
		image.setCurrentFrame(c);
		image.setLoop(false);
		image.setComplete(false);
	}
}

//�`��
void Entity::draw(){
	//����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
}

//����̈ʒu��ݒ�
void Entity::setEdge(){
	//���S����edge�������L�т�
	SETRECT(edge, (long)pos.x - edge_x, (long)pos.y - edge_y, edge_x * 2, edge_y * 2);
}

//���E�̏��̔���
void Entity::bottomObj(bool d, Entity *e){
	switch (e->getType()){
	case BOX_W:
	case BOX_S:
	case BOX_L:
	case BOX_B:
	case BOX_H:
	case ROCK:
		if (d){
			bottom_object[1] = true;
		}
		else {
			bottom_object[0] = true;
		}
		break;
	}
}