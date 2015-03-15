#include "graphics.h"

//コンストラクタ　各値を初期化
Graphics::Graphics(){
	direct = NULL;
	device = NULL;
	sprite = NULL;
	width = WINDOW_W;
	height = WINDOW_H;
	back = graphicsNS::BACK_COLOR;
}

//デストラクタ
Graphics::~Graphics(){
	//ワンクッション置く
	releaseAll();
}

//directXの破棄
void Graphics::releaseAll(){
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(direct);
	SAFE_RELEASE(device);
}

//初期化
void Graphics::initialize(HWND hwnd, int width, int height){
	this->hwnd = hwnd;
	this->width = width;
	this->height = height;
	//directXの初期化
	direct = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct == NULL) throw(GameError(gameErrorNS::FATAL, "Error Initializing Direct3D."));
	//d3dppの初期化　長いので分離
	initD3Dpp();

	//グラフィックカードを使えるか確かめる
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	//デバイスの初期化
	result = direct->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		this->hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&device
		);
	if (FAILED(result)) throw(GameError(gameErrorNS::FATAL, "Error Creating Direct3D Device."));
	//スプライトの初期化
	result = D3DXCreateSprite(device, &sprite);
	if (FAILED(result)) throw(GameError(gameErrorNS::FATAL, "Error Creating Direct3D Sprite."));
	//アルファブレンドとかの設定
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//d3dppの初期化
void Graphics::initD3Dpp(){
	try{
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = true;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.EnableAutoDepthStencil = true;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	catch (...){
		throw(GameError(gameErrorNS::FATAL, "Error Initializing Direct3D Presentation Parameters."));
	}
}

//バックバッファの表示
HRESULT Graphics::showBackBuffer(){
	result = device->Present(NULL, NULL, NULL, NULL);
	return result;
}

//デバイスの取得
HRESULT Graphics::getDeviceState(){
	result = E_FAIL;
	if (device == NULL) return result;
	result = device->TestCooperativeLevel();
	return result;
}

//リセット
HRESULT Graphics::reset(){
	initD3Dpp();
	sprite->OnLostDevice();
	result = device->Reset(&d3dpp);

	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	sprite->OnResetDevice();
	return result;
}

//描画開始
HRESULT Graphics::beginScene(){
	result = E_FAIL;
	if (device == NULL) return result;
	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, back, 1.0F, 0);
	result = device->BeginScene();
	return result;
}

//描画終了
HRESULT Graphics::endScene(){
	result = E_FAIL;
	if (device == NULL) return result;
	result = device->EndScene();
	return result;
}

//画像データ読み込み
HRESULT Graphics::loadTexture(const char *filename, ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture){
	D3DXIMAGE_INFO info;
	result = E_FAIL;
	try{
		if (filename == NULL){
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK) return result;
		width = info.Width;
		height = info.Height;

		result = D3DXCreateTextureFromFileEx(
			device, filename,
			info.Width, info.Height,
			1, 0,
			D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
			D3DX_DEFAULT, D3DX_DEFAULT,
			transcolor, &info,
			NULL, &texture
			);
	}
	catch (...){
		throw(GameError(gameErrorNS::FATAL, "Error in Graphics::loadTexture."));
	}
	return result;
}

//画像データメインメモリに読み込み
HRESULT Graphics::loadTextureSystemMem(const char *filename, ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture){
	D3DXIMAGE_INFO info;
	result = E_FAIL;
	try{
		if (filename == NULL){
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK) return result;
		width = info.Width;
		height = info.Height;

		result = D3DXCreateTextureFromFileEx(
			device, filename,
			info.Width, info.Height,
			1, 0,
			D3DFMT_UNKNOWN, D3DPOOL_SYSTEMMEM,
			D3DX_DEFAULT, D3DX_DEFAULT,
			transcolor, &info,
			NULL, &texture);
	}
	catch (...){
		throw(GameError(gameErrorNS::FATAL, "Error in Graphics::loadTexture"));
	}
	return result;
}

//スプライト描画
void Graphics::drawSprite(const Sprite &spriteD, ARGB color){
	if (spriteD.texture == NULL) return;

	VC2 center = VC2((float)(spriteD.width / 2 * spriteD.scale),
		(float)(spriteD.height / 2 * spriteD.scale));

	VC2 translate = VC2((float)spriteD.x,
		(float)spriteD.y);

	VC2 scaling(spriteD.scale, spriteD.scale);

	if (spriteD.flipH){
		scaling.x *= -1;
		center.x -= (float)(spriteD.width * spriteD.scale);
		translate.x += (float)(spriteD.width * spriteD.scale);
	}

	if (spriteD.flipV){
		scaling.y *= -1;
		center.y -= (float)(spriteD.height* spriteD.scale);
		translate.y += (float)(spriteD.height * spriteD.scale);
	}

	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix, NULL,
		0.0f, &scaling,
		&center, (float)(spriteD.angle),
		&translate);

	sprite->SetTransform(&matrix);

	sprite->Draw(spriteD.texture, &spriteD.rect, NULL, NULL, color);
}

//四角の描画
bool Graphics::drawQuad(LP_VERTEX vertex){
	HRESULT result = E_FAIL;
	if (vertex == NULL) return false;

	device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	device->SetStreamSource(0, vertex, 0, sizeof(Vertex));
	device->SetFVF(D3DFVF_VERTEX);
	result = device->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	if (FAILED(result)) return false;
	return true;
}

//Vertexの作成
HRESULT Graphics::createVertexBuffer(Vertex verts[], UINT size, LP_VERTEX &vertex){
	HRESULT result = E_FAIL;
	result = device->CreateVertexBuffer(
		size, D3DUSAGE_WRITEONLY,
		D3DFVF_VERTEX, D3DPOOL_DEFAULT,
		&vertex, NULL);
	if (FAILED(result)) return result;
	void *ptr;
	result = vertex->Lock(0, size, (void**)&ptr, 0);
	if (FAILED(result)) return result;
	memcpy(ptr, verts, size);
	vertex->Unlock();
	return result;
}