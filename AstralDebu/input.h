#ifndef _INPUT_H
#define _INPUT_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include "const.h"
#include "gameerror.h"

#ifndef HID_USAGE_PAGE_GENETIC
#define HID_USAGE_PAGE_GENETIC ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENETIC_MOUSE
#define HID_USAGE_GENETIC_MOUSE ((USHORT)0x02)
#endif

namespace inputNS{
	const int KEYS_ARRAY_LEN = 256;
	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR MOUSE_DOWN = 8;
	const UCHAR MOUSE_PRESSED = 16;
	const UCHAR TEXT_IN = 32;
	const UCHAR ALL = KEYS_DOWN + KEYS_PRESSED + MOUSE + MOUSE_DOWN + MOUSE_PRESSED + TEXT_IN;
}

//キー入力を受け取るクラス
class Input {
private:
	//押されているキー
	bool keysDown[inputNS::KEYS_ARRAY_LEN];
	//今まさに押されたキー
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];
	//入力文字列コマンド用
	std::string textIn;
	//入力文字
	char charIn;
	//改行の有無
	bool newLine;
	//マウス座標
	int mouseX, mouseY;
	//押されているマウス
	bool mouseLDown, mouseMDown, mouseRDown;
	//今まさに押されたマウス
	bool mouseLPressed, mouseMPressed, mouseRPressed;

	//キーの範囲内かを調べる
	inline bool inKey(UCHAR k) const { return (k < inputNS::KEYS_ARRAY_LEN); }
	inline bool inKey(size_t k) const { return (k < inputNS::KEYS_ARRAY_LEN); }
public:
	//コンストラクタ
	Input();
	//デストラクタ
	virtual ~Input();
	//初期化
	void initialize(HWND hwnd);

	//キー操作
	void keyDown(WPARAM);
	void keyUp(WPARAM);
	void keyIn(WPARAM);
	//キーの有無を確認
	bool isKeyDown(UCHAR) const;
	bool isKeyPressed(UCHAR) const;
	bool anyKeyPressed() const;
	//マウス操作
	void mouseIn(LPARAM);
	//入力情報消去
	void clearKeyPress(UCHAR);
	void clear(UCHAR);
	void clearAll() { clear(inputNS::ALL); }
	void clearTextIn() { textIn.clear(); }
	//getter
	int getMouseX() const { return mouseX; }
	int getMouseY() const { return mouseY; }
	int getMouseLDown() const { return mouseLDown; }
	int getMouseMDown() const { return mouseMDown; }
	int getMouseRDown() const { return mouseRDown; }
	int getMouseLPressed() const { return mouseLPressed; }
	int getMouseMPressed() const { return mouseMPressed; }
	int getMouseRPressed() const { return mouseRPressed; }
	std::string getTextIn() const { return textIn; };
	char getCharIn() const { return charIn; }

	//setter
	void setMouseLDown(bool b) { mouseLDown = b; }
	void setMouseMDown(bool b) { mouseMDown = b; }
	void setMouseRDown(bool b) { mouseRDown = b; }
	void setMouseLPressed(bool b) { mouseLPressed = b; }
	void setMouseMPressed(bool b) { mouseMPressed = b; }
	void setMouseRPressed(bool b) { mouseRPressed = b; }
};

#endif
