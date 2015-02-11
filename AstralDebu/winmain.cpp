#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "const.h"
#include "gameerror.h"
#include "astraldebu.h"

//プロトタイプ
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

//グローバル
HWND hwnd = NULL;
Game *game = NULL;

//メインメソッド
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	//デバッグ用
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	//起動準備
	MSG msg;
	game = new AstralDebu();
	if (!CreateMainWindow(hwnd, hInstance, nCmdShow)) return 1;
	try{
		game->initialize(hwnd);
		//ウィンドウ表示
		ShowWindow(hwnd, nCmdShow);

		//QUITが出るまでぐるぐる回る
		int done = 0;
		while (!done){
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				if (msg.message == WM_QUIT) done = 1;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				game->run(hwnd);
			}
		}
		DestroyWindow(hwnd);
		SAFE_DELETE(game);
		return msg.wParam;
	}
	catch (const GameError &e){
		MessageBox(NULL, e.getMessage(), "Error", MB_OK);
		DestroyWindow(hwnd);
		game->deleteAll();
	}
	catch (...){
		MessageBox(NULL, "予期せぬエラーが発生しました", "Error", MB_OK);
		DestroyWindow(hwnd);
		game->deleteAll();
	}
	SAFE_DELETE(game);
	return 0;
}

//処理はゲームクラスに任せる
LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	return (game->messageHandler(hwnd, msg, wParam, lParam));
}

//ウィンドウ作成
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow){
	//ウィンドウクラス
	WNDCLASSEX wcx;
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = CLS_NAME.c_str();
	wcx.hIconSm = NULL;
	if (RegisterClassEx(&wcx) == 0) return false;
	//ウィンドウ作成
	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_DLGFRAME | WS_MINIMIZEBOX;
	hwnd = CreateWindow(CLS_NAME.c_str(), APP_TITLE.c_str(),
		style, CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_W, WINDOW_H,
		(HWND)NULL, (HMENU)NULL,
		hInstance, (LPVOID)NULL);
	if (!hwnd) return false;
	//枠のサイズを再設定
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	MoveWindow(hwnd, 0,
		0, WINDOW_W + (WINDOW_W - clientRect.right),
		WINDOW_H + (WINDOW_H - clientRect.bottom), TRUE);

	return true;
}