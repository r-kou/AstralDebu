#ifndef _CONST_H
#define _CONST_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <string>

//マクロ
#define SAFE_DELETE(ptr) {if(ptr) {delete(ptr);(ptr) = NULL;}}
#define SAFE_RELEASE(ptr) {if(ptr) {(ptr)->Release();(ptr) = NULL;}}
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }

//やたら長いdirectXの名前を短縮
#define VC2 D3DXVECTOR2
#define LP_TEXTURE LPDIRECT3DTEXTURE9
#define LP_SPRITE   LPD3DXSPRITE
#define LP_DEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9
#define LP_VERTEX LPDIRECT3DVERTEXBUFFER9
#define LP_FONT   LPD3DXFONT
#define D3D_PARAMETERS D3DPRESENT_PARAMETERS
#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

//よく使うやつを短縮
#define ARGB DWORD
#define SETCOLOR(a,r,g,b) \
	((ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define TRANS SETCOLOR(0,255,0,255)
#define SETRECT(x,l,t,r,b) \
	x.top = t; \
	x.bottom = t + b; \
	x.left = l; \
	x.right = l + r
#define DT_CT DT_SINGLELINE | DT_CENTER | DT_TOP
#define DT_CC DT_SINGLELINE | DT_CENTER | DT_VCENTER
#define DT_CB DT_SINGLELINE | DT_CENTER | DT_BOTTOM
#define MAP_NAME(x) "map" + std::to_string(x)
#define FOR(x) for(int i = 0; i < x; i++)
#define FOR_D(x,y) for(int i = 0; i < x; i++) for(int j = i + 1; j < y; j++)
#define ALL_OBJ FOR(obj_num)
#define ALL_OBJ_D FOR_D(obj_num,obj_num)

//汎用定数
const int WINDOW_W = 800;
const int WINDOW_H = 640;
const double PI = 3.14159265;
const float FRAME_RATE = 60.0f;
const float FRAME_RATE_MIN = 10.0f;
const float FRAME_TIME_MIN = 1.0f / FRAME_RATE;
const float FRAME_TIME_MAX = 1.0f / FRAME_RATE_MIN;

//エリアの幅とか
const int DATA_LEN = 160;
const int MAP_COL = 25;
const int MAP_ROW = 15;
const int CHIP_SIZE = 32;

//ファイル名
const std::string CLS_NAME = "AstralDebu";
const std::string APP_TITLE = "アストラルデブ";
const std::string IMG_DIR = "image";
const std::string IMG_DEBU = "debu.png";
const std::string IMG_CHIP = "chip.png";
const std::string IMG_BOMB = "bomb.png";
const std::string IMG_ENEMY = "enemy.png";
const std::string IMG_TITLE = "title.png";
const std::string MAP_DIR = "map";
const std::string MAP_EXT = ".chip";

//汎用関数
#endif