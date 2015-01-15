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
	const UCHAR TEXT_IN = 4;
	const UCHAR ALL = KEYS_DOWN + KEYS_PRESSED + TEXT_IN;
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
	char charIn;
	bool newLine;
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
	//入力情報消去
	void clearKeyPress(UCHAR);
	void clear(UCHAR);
	void clearAll() { clear(inputNS::ALL); }
	void clearTextIn() { textIn.clear(); }
	//文字列操作
	std::string getTextIn() { return textIn; };
	char getCharIn() { return charIn; }
};

#endif
