#include "entity.h"

using namespace entityNS;

//�R���X�g���N�^
Entity::Entity(){
	graphics = NULL;
	audio = NULL;
	texture = NULL;
	state = ST_EMPTY;
	type = TY_NONE;
	pos.x = 0.0f;
	pos.y = 0.0f;
	vel.x = 0.0f;
	vel.y = 0.0f;
	mov.x = 0.0f;
	mov.y = 0.0f;
	frameTime = 0.0f;
	direct = false;
	size = 2;
	img = 0;
	SETRECT(edge, 0, 0, 0, 0);
	edgeX = 0;
	edgeY = 0;
	marginX = 0;
	marginY = 0;
	col = 1;
	animInterval = 0;
	warpInterval = 0;
	knockInterval = 0;
	bottomObj[0] = false;
	bottomObj[1] = false;
	action = false;
	trans = false;
	fall = false;
	response = 0;
	putSound = "";
	deadSound = "";
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
		audio = game->getAudio();
		texture = t;
		image.initialize(graphics, texture, size, size, col);
		image.setDelay(0.2f);
		setImage(img);
		pos.x = CHIP(i + 0.5f);
		pos.y = CHIP_D(j + 0.5f);
		vel.x = 0.0f;
		vel.y = 0.0f;
		mov.x = 0.0f;
		mov.y = 0.0f;
		frameTime = 0.0f;
		direct = false;
		SETRECT(edge, (long)pos.x - edgeX, (long)pos.y - edgeY, edgeX * 2, edgeY * 2);
		state = ST_STAND;
		animInterval = 0;
		warpInterval = 0;
	}
	catch (...) { return false; }
	return true;
}

//�ړ�
void Entity::move(float frameTime){
	this->frameTime = frameTime;
	if (fall){
		if (state == ST_JUMP || state == ST_DEAD) vel.y += GRAVITY_RATE * frameTime;
		if (vel.y > VEL_MAX) vel.y = VEL_MAX;
	}

	//���x�������ړ�
	mov.x = (vel.x * frameTime);
	mov.y = (vel.y * frameTime);
	pos.x += mov.x;
	pos.y += mov.y;

	//���E�̗������������
	bottomObj[0] = false;
	bottomObj[1] = false;

	//�C���^�[�o��������Ȃ猸�Z
	if (knockInterval > 0.0f) {
		knockInterval -= frameTime;
		if (knockInterval <= 0.0f) knockInterval = 0.0f;
	}
	if (warpInterval > 0.0f) {
		warpInterval -= frameTime;
		if (warpInterval <= 0.0f) warpInterval = 0.0f;
	}
	if (animInterval > 0.0f) {
		animInterval -= frameTime;
		if (animInterval <= 0.0f) animInterval = 0.0f;
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
	if (pos.x <= edgeX) {
		t_o |= LEFT;
		bottomObj[0] = true;
	}
	if (pos.x >= WINDOW_W - edgeX) {
		t_o |= RIGHT;
		bottomObj[1] = true;
	}
	if (pos.y < DATA_LEN + edgeY) t_o |= TOP;
	if (pos.y > WINDOW_H - edgeY) t_o |= BOTTOM;

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
		if (tmp == BOTTOM) bottomObj[1] = true;
		t |= tmp;
	}
	if ((t_o & EDGE4) == 0) {
		t |= touchMapDirect(map[c_l][c_bn], LEFT);
		UCHAR tmp = touchMapDirect(map[c_ln][c_b], BOTTOM);
		if (tmp == BOTTOM) bottomObj[0] = true;;
		t |= tmp;
	}

	//�Փ˔���
	collideMap(t | t_o);
}

//�n�`�ւ̐ڐG
void Entity::collideMap(UCHAR t){
	//�㉺���E�ɉ����Ĉʒu�Ƒ��x��␳
	if ((t & LEFT) && (vel.x <= 0.0f)) {
		setLeft(false);
		vel.x = 0.0f;
		if (state == ST_KNOCK) {
			setStand();
			playPut();
		}
	}
	if ((t & RIGHT) && (vel.x >= 0.0f)) {
		setRight(false);
		vel.x = 0.0f;
		if (state == ST_KNOCK) {
			setStand();
			playPut();
		}
	}
	if ((t & TOP) && (vel.y <= 0.0f)) {
		setTop(false);
		//��~�����ɗ������x�ɉ��Z
		if (vel.y < 0) vel.y += TOP_GRAVITY_RATE*frameTime;
		else vel.y = 0;
		if (state == ST_KNOCK) {
			setStand();
			playPut();
		}
	}
	if (t & BOTTOM) {
		//�󒆂ɂ����璅�n����
		if (((vel.y >= 0.0f) && (state == ST_JUMP || state == ST_LADDER)) ||
			((vel.y > 0.0f) && (state == ST_KNOCK || state == ST_DEAD))){
			if ((vel.y > 50) && ((state == ST_JUMP) || (state == ST_KNOCK))) playPut();
			//�����Ă��璅�n�@����ł����~
			if (state == ST_DEAD) vel.x = 0.0f;
			else setStand();
			setBottom(false);
			vel.y = 0.0f;
		}
	}
	else if ((state != ST_KNOCK) && (state != ST_DEAD)&&(state != ST_CLEAR)) {
		//���ɉ����Ȃ��Ȃ痎��
		state = ST_JUMP;
	}
}

//���I�u�W�F�N�g�ւ̐ڐG����
void Entity::touchObj(Entity *e){
	UCHAR t = 0, et = 0;

	//�݂��̈ʒu�ɍ��킹�Ďl���Ō���
	if (pos.y > e->getPosY()){
		if (pos.x > e->getPosX()){
			//edge1
			if (touchObjDirect(diffLeft(e, false), diffTop(e, true), checkObjectVelocity(e,diffVelX(e)))){
				t |= LEFT;
				et |= RIGHT;
			}
			if (touchObjDirect(diffLeft(e, true), diffTop(e, false), checkObjectVelocity(e, diffVelY(e)))){
				t |= TOP;
				et |= BOTTOM;
			}
		}
		else {
			//edge2
			if (touchObjDirect(-diffRight(e, false), diffTop(e, true), checkObjectVelocity(e, -diffVelX(e)))){
				t |= RIGHT;
				et |= LEFT;
			}
			if (touchObjDirect(-diffRight(e, true), diffTop(e, false), checkObjectVelocity(e, diffVelY(e)))){
				t |= TOP;
				et |= BOTTOM;
			}
		}
		if (e->collideBottomLeft(this))
			e->checkBottom(false, this);
		if (e->collideBottomRight(this))
			e->checkBottom(true, this);
	}
	else {
		if (pos.x > e->getPosX()){
			//edge4
			if (touchObjDirect(diffLeft(e, false), -diffBottom(e, true), checkObjectVelocity(e, diffVelX(e)))){
				t |= LEFT;
				et |= RIGHT;
			}
			if (touchObjDirect(diffLeft(e, true), -diffBottom(e, false), checkObjectVelocity(e, -diffVelY(e)))){
				t |= BOTTOM;
				et |= TOP;
			}
		}
		else {
			//edge3
			if (touchObjDirect(-diffRight(e, false), -diffBottom(e, true), checkObjectVelocity(e, -diffVelX(e)))){
				t |= RIGHT;
				et |= LEFT;
			}
			if (touchObjDirect(-diffRight(e, true), -diffBottom(e, false), checkObjectVelocity(e, -diffVelY(e)))){
				t |= BOTTOM;
				et |= TOP;
			}
		}
		if (collideBottomLeft(e))
			checkBottom(false, e);
		if (collideBottomRight(e))
			checkBottom(true, e);
	}

	//�Փ˔���
	if (t){
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
	case TY_ROCK:
		//����ǂȂ̂ŕǂƓ���
		if ((t & LEFT) && (diffVelX(e) < 0)) setResponse(RES_LEFT);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setResponse(RES_RIGHT);
		if ((t & TOP) && (diffVelY(e) < 0)) setResponse(RES_TOP);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == ST_JUMP || state == ST_LADDER)) ||
			((diffVelY(e) > 0) && (state == ST_KNOCK)))) setResponse(RES_BOTTOM); //�󒆂ɂ����璅�n����
		break;
	case TY_RED_WARP:
	case TY_GREEN_WARP:
	case TY_YELLOW_WARP:
		//���[�v�͔���������ԓ�����
		if (warpInterval == 0.0f) setResponse(RES_WARP,CHIP(e->getPartnerX() + 0.5f),CHIP_D(e->getPartnerY() + 0.5f));
	case TY_LADDER:
	case TY_GOAL:
		//�������ɂȂ�
		trans = true;
		break;

	}
}

//���I�u�W�F�N�g�ւ̔���
void Entity::responseObj(){
	if (getResponse(RES_LEFT)) {
		//���ɏՓ�
		setLeft(false);
		vel.x = 0.0f;
		if (!action&&type!=TY_DEBU) direct = false;
		if (state == ST_KNOCK) {
			setStand();
			playPut();
		}
	}
	if (getResponse(RES_RIGHT)) {
		//�E�ɏՓ�
		setRight(false);
		vel.x = 0.0f;
		if (!action && type != TY_DEBU) direct = true;
		if (state == ST_KNOCK) {
			setStand();
			playPut();
		}
	}
	if (getResponse(RES_TOP)) {
		//��ɏՓ�
		setTop(false);
		//��~�����ɗ������x�ɉ��Z
		if (vel.y < 0) vel.y += TOP_GRAVITY_RATE*frameTime;
		else vel.y = 0;
		if (state == ST_KNOCK) {
			setStand();
			playPut();
		}
	}
	if (getResponse(RES_BOTTOM)) {
		//���ɏՓ�
		setBottom(false);
		//�͂����̑��x�ł͂Ȃ�Ȃ�
		if (((vel.y > 50) && (vel.y != 100)) && ((state == ST_JUMP) || (state == ST_KNOCK))) playPut();
		vel.y = 0.0f;
		setStand();
	}
	if (getResponse(RES_BOTTOM_CHIP)) {
		//���ɏՓ� �}�X�ɂ҂�����
		setCX();
		setBottom(false);
		if ((vel.y > 50) && ((state == ST_JUMP) || (state == ST_KNOCK))) playPut();
		vel.y = 0.0f;
		vel.x = 0.0f;
		setStand();
	}
	if (getResponse(RES_STOP)) {
		//��~
		vel.x = 0.0f;
		vel.y = 0.0f;
		setStand();
	}
	if (getResponse(RES_CHIP)) {
		//��~ �}�X�ɂ҂�����
		setCX();
		setCY();
		vel.x = 0.0f;
		vel.y = 0.0f;
		setStand();
	}
	if (getResponse(RES_COLLIDE)){
		if (!action) direct = !direct;
	}
	if (getResponse(RES_WARP)){
		//���[�v
		warpInterval = 0.5f;
		pos.x = getResponseX(RES_WARP);
		pos.y = getResponseY(RES_WARP);
		audio->playCue(audioNS::WARP);
	}
	if (getResponse(RES_KNOCK)) {
		state = ST_KNOCK;
		vel.x = addLimit(vel.x, getResponseX(RES_KNOCK), VEL_MAX);
		vel.y = addLimit(vel.y, getResponseY(RES_KNOCK), VEL_MAX);
		audio->playCue(audioNS::KNOCK);
	}
	if (getResponse(RES_JUMP)){
		state = ST_JUMP;
		vel.x = addLimit(vel.x,getResponseX(RES_JUMP), VEL_MAX);
		vel.y = addLimit(vel.y,getResponseY(RES_JUMP), VEL_MAX_JUMP) - VEL_KNOCK_JUMP;
		audio->playCue(audioNS::KNOCK);
	}
	if (getResponse(RES_DEAD)){
		state = ST_DEAD;
		vel.x = addLimit(vel.x,getResponseX(RES_DEAD), VEL_MAX);
		vel.y = addLimit(vel.y,getResponseY(RES_DEAD), VEL_MAX_JUMP) - VEL_KNOCK_JUMP;
		playDead();
	}
	if (getResponse(RES_CLEAR)){
		state = ST_CLEAR;
		vel.x = 0.0f;
		vel.y = 0.0f;
		audio->playCue(audioNS::GOAL);
	}

}

//���[�v�̐���𔻒�
void Entity::collideWarp(Entity *e){
	switch (e->getType()){
	case TY_ROCK:
		resetResponse(RES_WARP);
		break;
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
	//�������u���b�N���A��낪��q�Ƃ��Ȃ甼����
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	if (trans) image.draw(graphicsNS::ALPHA75);
	else image.draw();
	trans = false;
}

//����̈ʒu��ݒ�
void Entity::setEdge(){
	//���S����edge�������L�т�
	SETRECT(edge, (long)pos.x - edgeX, (long)pos.y - edgeY, edgeX * 2, edgeY * 2);
}

//���E�̏��̔���
void Entity::checkBottom(bool d, Entity *e){
	switch (e->getType()){
	case TY_WOOD_BOX:
	case TY_STEEL_BOX:
	case TY_LEAD_BOX:
	case TY_BOMB_BOX:
	case TY_HIBOMB_BOX:
	case TY_AIR_BOX:
	case TY_FRAME_BOX:
	case TY_ROCK:
		if (d){
			bottomObj[1] = true;
		}
		else {
			bottomObj[0] = true;
		}
		break;
	}
}

//�e�l�̍ő�l�ȉ��ɂȂ�悤�ɉ��Z
float Entity::addLimit(float v, float n, float max) {
	//�����͖����@���˂͂���
	if (((v > n) && (n > 0.0f)) || ((v < n) && (n < 0.0f))) return v;
	if (v*n > 0) return setLimit(v + n, max);
	else return setLimit(n, max);
}

//���x�Ɉˑ����Ȃ��I�u�W�F�N�g�ւ̏Փ˔���
float Entity::checkObjectVelocity(Entity *e, float vel){
	switch (type){
	case TY_BLAST:
	case TY_GOAL:
	case TY_RED_WARP:
	case TY_GREEN_WARP:
	case TY_YELLOW_WARP:
		return 0;
		break;
	case TY_HAMMER:
		if (state == ST_HAMMER) return 0;
		break;
	}
	switch (e->getType()){
	case TY_BLAST:
	case TY_GOAL:
	case TY_RED_WARP:
	case TY_GREEN_WARP:
	case TY_YELLOW_WARP:
		return 0;
		break;
	case TY_HAMMER:
		if (e->getState() == ST_HAMMER) return 0;
		break;
	}
	return vel;
}