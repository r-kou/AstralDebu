#include "entity.h"

using namespace entityNS;

//コンストラクタ
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

//デストラクタ
Entity::~Entity(){
}

//初期化
bool Entity::initialize(Game *game, Texture *t, int i, int j){
	try{
		//グラフィックとかいろいろ初期化
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

//移動
void Entity::move(float frameTime){
	//速度分だけ移動
	pos.x += (vel.x * frameTime);
	pos.y += (vel.y * frameTime);

	//左右の落下判定を消去
	bottom_object[0] = false;
	bottom_object[1] = false;

	//インターバルがあるなら減算
	if (warp_interval > 0.0f) {
		warp_interval -= frameTime;
		if (warp_interval <= 0.0f) warp_interval = 0.0f;
	}
	if (anim_interval > 0.0f) {
		anim_interval -= frameTime;
		if (anim_interval <= 0.0f) anim_interval = 0.0f;
	}

	//移動先に判定を設定
	setEdge();
	//アニメーションとかを更新
	image.update(frameTime);
}

//地形への接触判定
void Entity::touchMap(int map[MAP_COL][MAP_ROW]){
	UCHAR t = 0, t_o = 0;
	int c_l = ChipX((float)getLeft(false)), c_ln = ChipX((float)getLeft(true)),
		c_r = ChipX((float)getRight(false)), c_rn = ChipX((float)getRight(true)),
		c_t = ChipY((float)getTop(false)), c_tn = ChipY((float)getTop(true)),
		c_b = ChipY((float)getBottom(false)), c_bn = ChipY((float)getBottom(true));

	//画面外か検査
	if (pos.x <= edge_x) t_o |= LEFT;
	if (pos.x >= WINDOW_W - edge_x) t_o |= RIGHT;
	if (pos.y <= DATA_LEN + edge_y) t_o |= TOP;
	if (pos.y >= WINDOW_H - edge_y) t_o |= BOTTOM;

	//四隅のマップを検査
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

	//衝突判定
	collideMap(t | t_o);
}

//上下左右の衝突判定
UCHAR Entity::touchMapDirect(int c, UCHAR t){
	//地形なら衝突
	if ((c == 0) || (c == CHIP_LADDER)) return 0;
	else {
		return t;
	}
}

//地形への接触
void Entity::collideMap(UCHAR t){
	//上下左右に応じて位置と速度を補正
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
		//空中にいたら着地判定
		if (((vel.y >= 0.0f) && (state == JUMP || state == LADDER)) ||
			((vel.y > 0.0f) && (state == KNOCK || state == DEAD))){
			//生きてたら着地　死んでたら停止
			if (state == DEAD) vel.x = 0.0f;
			else state = STAND;
			setBottom(false);
			vel.y = 0.0f;
		}
	}
	else if ((state != KNOCK) && (state != DEAD)) {
		//下に何もないなら落下
		state = JUMP;
	}
}

//他オブジェクトへの接触判定
void Entity::touchObj(Entity *e){
	UCHAR t = 0, et = 0;

	//互いの位置に合わせて四隅で検査
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

	//衝突判定
	if (t != 0){
		collideObj(e, t);
		e->collideObj(this, et);
	}
}

//各方向への判定が重なっているかを調べる
bool Entity::touchObjDirect(long x, long y, float v){
	return ((x <= 0) && (y <= 0) && (v <= 0));
}

//他オブジェクトへの接触
void Entity::collideObj(Entity *e, UCHAR t){
	switch (e->getType()){
	case ROCK:
		//壊れる壁なので壁と同じ
		if ((t & LEFT) && (diffVelX(e) < 0)) setRes(0);
		if ((t & RIGHT) && (diffVelX(e) > 0)) setRes(1);
		if ((t & TOP) && (diffVelY(e) < 0)) setRes(2);
		if ((t & BOTTOM) && (((diffVelY(e) >= 0) && (state == JUMP || state == LADDER)) ||
			((diffVelY(e) > 0) && (state == KNOCK)))) setRes(3); //空中にいたら着地判定
		break;
	case WARP_R:
	case WARP_G:
	case WARP_Y:
		//ワープは発動後一定期間内無効
		if (warp_interval == 0.0f) setRes(5,(e->getPartnerX() + 0.5f) * CHIP_SIZE,(e->getPartnerY() + 0.5f) * CHIP_SIZE + DATA_LEN);
		break;
	}
}

//他オブジェクトへの反応
void Entity::responseObj(){
	if (getRes(0)) {
		//左に衝突
		setLeft(false);
		vel.x = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if (getRes(1)) {
		//右に衝突
		setRight(false);
		vel.x = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if (getRes(2)) {
		//上に衝突
		setTop(false);
		vel.y = 0.0f;
		if (state == KNOCK) state = STAND;
	}
	if (getRes(3)) {
		//下に衝突
		setBottom(false);
		vel.y = 0.0f;
		state = STAND;
	}
	if (getRes(4)) {
		//下に衝突　マスにぴったり
		setCX();
		setCY();
		vel.y = 0.0f;
		state = STAND;
	}
	if (getRes(5)){
		//ワープ
		warp_interval = 0.5f;
		pos.x = getResX(5);
		pos.y = getResY(5);
	}
	if (getRes(6)){
		//死亡
		state = DEAD;
	}
}


//描画する画像を変更
void Entity::changeImage(){}

//画像変更の一括設定
void Entity::setImage(int s, int e, bool l){
	//画像を設定する
	if (image.getCurrentFrame() < s ||
		image.getCurrentFrame() > e){
		image.setFrames(s, e);
		image.setCurrentFrame(s);
		image.setLoop(l);
		image.setComplete(false);
	}
}

//一枚絵の設定
void Entity::setImage(int c){
	//画像を設定する
	if (image.getCurrentFrame() != c){
		image.setFrames(c, c);
		image.setCurrentFrame(c);
		image.setLoop(false);
		image.setComplete(false);
	}
}

//描画
void Entity::draw(){
	//書く
	image.setX(pos.x - size / 2 * image.getScale());
	image.setY(pos.y - size / 2 * image.getScale());
	image.draw();
}

//判定の位置を設定
void Entity::setEdge(){
	//中心からedge分だけ伸びる
	SETRECT(edge, (long)pos.x - edge_x, (long)pos.y - edge_y, edge_x * 2, edge_y * 2);
}

//左右の床の判定
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