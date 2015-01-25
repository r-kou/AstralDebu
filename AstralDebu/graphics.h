#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

//デバッグ用
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include "const.h"
#include "gameerror.h"

//図形描画よう構造型
struct Vertex{
	float x, y, z;
	float rhw;
	unsigned long color;
};

struct Sprite{
	//大きさ
	int width, height;
	//位置
	float x, y;
	//元の画像とのサイズ比
	float scale;
	//角度
	float angle;
	//RECT構造
	RECT rect;
	//画像データ
	LP_TEXTURE texture;
	//上下左右反転
	bool flipH, flipV;
};

//よく使う色
namespace graphicsNS{
	const ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);

	const ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);
	const ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);
	const ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);
	const ARGB BACK_COLOR = NAVY;
}

//directXを用いて画像を使うクラス
class Graphics{
private:
	//directXのポインタとか
	LP_3D direct;
	LP_DEVICE device;
	LP_SPRITE sprite;
	D3D_PARAMETERS d3dpp;
	//その他メンバ
	HRESULT result;
	HWND hwnd;
	int width, height;
	ARGB back;
	//d3dppの初期化
	void initD3Dpp();
public:
	//コンストラクタ
	Graphics();
	//デストラクタ
	virtual ~Graphics();
	//全ポインタの破棄
	void releaseAll();
	//初期化
	void initialize(HWND hwnd, int width, int height);

	//バックバッファの表示
	HRESULT showBackBuffer();
	//デバイスの取得
	HRESULT getDeviceState();
	//リセット
	HRESULT reset();
	//描画開始
	HRESULT beginScene();
	//描画終了
	HRESULT endScene();

	//setter(バグるのでdirectx関連は無し)
	void setBackColor(ARGB c) { back = c; }

	//getter
	LP_3D get3D() { return direct; }
	LP_DEVICE getDevice() { return device; }
	LP_SPRITE getSprite() { return sprite; }
	HDC getDC() { return GetDC(hwnd); }
	ARGB getBackColor() { return back; }

	//スプライト描画開始
	void spriteBegin() { sprite->Begin(D3DXSPRITE_ALPHABLEND); }
	//スプライト描画終了
	void spriteEnd() { sprite->End(); }
	//画像データ読み込み
	HRESULT loadTexture(const char *filename, ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
	//画像データメインメモリに読み込み
	HRESULT loadTextureSystemMem(const char *filename, ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
	//スプライト描画
	void drawSprite(const Sprite &spriteD, ARGB color);
	//四角の描画
	bool drawQuad(LP_VERTEX vertex);
	//Vertexの作成
	HRESULT createVertexBuffer(Vertex verts[], UINT size, LP_VERTEX &vertex);

	DWORD pixelCollision(const Sprite &sprite1, const Sprite &sprite2);
	//ベクター演算
	static float VC2Length(const VC2 *v) { return D3DXVec2Length(v); }
	static float VC2Dot(const VC2 *v1, const VC2 *v2) { return D3DXVec2Dot(v1, v2); }
	static void VC2Normalize(VC2 *v) { D3DXVec2Normalize(v, v); }
	static VC2* VC2Transform(VC2 *v, D3DXMATRIX *m) { return D3DXVec2TransformCoord(v, v, m); }
};

#endif