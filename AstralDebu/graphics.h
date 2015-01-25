#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

//�f�o�b�O�p
#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include "const.h"
#include "gameerror.h"

//�}�`�`��悤�\���^
struct Vertex{
	float x, y, z;
	float rhw;
	unsigned long color;
};

struct Sprite{
	//�傫��
	int width, height;
	//�ʒu
	float x, y;
	//���̉摜�Ƃ̃T�C�Y��
	float scale;
	//�p�x
	float angle;
	//RECT�\��
	RECT rect;
	//�摜�f�[�^
	LP_TEXTURE texture;
	//�㉺���E���]
	bool flipH, flipV;
};

//�悭�g���F
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

//directX��p���ĉ摜���g���N���X
class Graphics{
private:
	//directX�̃|�C���^�Ƃ�
	LP_3D direct;
	LP_DEVICE device;
	LP_SPRITE sprite;
	D3D_PARAMETERS d3dpp;
	//���̑������o
	HRESULT result;
	HWND hwnd;
	int width, height;
	ARGB back;
	//d3dpp�̏�����
	void initD3Dpp();
public:
	//�R���X�g���N�^
	Graphics();
	//�f�X�g���N�^
	virtual ~Graphics();
	//�S�|�C���^�̔j��
	void releaseAll();
	//������
	void initialize(HWND hwnd, int width, int height);

	//�o�b�N�o�b�t�@�̕\��
	HRESULT showBackBuffer();
	//�f�o�C�X�̎擾
	HRESULT getDeviceState();
	//���Z�b�g
	HRESULT reset();
	//�`��J�n
	HRESULT beginScene();
	//�`��I��
	HRESULT endScene();

	//setter(�o�O��̂�directx�֘A�͖���)
	void setBackColor(ARGB c) { back = c; }

	//getter
	LP_3D get3D() { return direct; }
	LP_DEVICE getDevice() { return device; }
	LP_SPRITE getSprite() { return sprite; }
	HDC getDC() { return GetDC(hwnd); }
	ARGB getBackColor() { return back; }

	//�X�v���C�g�`��J�n
	void spriteBegin() { sprite->Begin(D3DXSPRITE_ALPHABLEND); }
	//�X�v���C�g�`��I��
	void spriteEnd() { sprite->End(); }
	//�摜�f�[�^�ǂݍ���
	HRESULT loadTexture(const char *filename, ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
	//�摜�f�[�^���C���������ɓǂݍ���
	HRESULT loadTextureSystemMem(const char *filename, ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);
	//�X�v���C�g�`��
	void drawSprite(const Sprite &spriteD, ARGB color);
	//�l�p�̕`��
	bool drawQuad(LP_VERTEX vertex);
	//Vertex�̍쐬
	HRESULT createVertexBuffer(Vertex verts[], UINT size, LP_VERTEX &vertex);

	DWORD pixelCollision(const Sprite &sprite1, const Sprite &sprite2);
	//�x�N�^�[���Z
	static float VC2Length(const VC2 *v) { return D3DXVec2Length(v); }
	static float VC2Dot(const VC2 *v1, const VC2 *v2) { return D3DXVec2Dot(v1, v2); }
	static void VC2Normalize(VC2 *v) { D3DXVec2Normalize(v, v); }
	static VC2* VC2Transform(VC2 *v, D3DXMATRIX *m) { return D3DXVec2TransformCoord(v, v, m); }
};

#endif