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

//�L�[���͂��󂯎��N���X
class Input {
private:
	//������Ă���L�[
	bool keysDown[inputNS::KEYS_ARRAY_LEN];
	//���܂��ɉ����ꂽ�L�[
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];
	//���͕�����R�}���h�p
	std::string textIn;
	//���͕���
	char charIn;
	//���s�̗L��
	bool newLine;
	//�}�E�X���W
	int mouseX, mouseY;
	//������Ă���}�E�X
	bool mouseLDown, mouseMDown, mouseRDown;
	//���܂��ɉ����ꂽ�}�E�X
	bool mouseLPressed, mouseMPressed, mouseRPressed;

	//�L�[�͈͓̔����𒲂ׂ�
	inline bool inKey(UCHAR k) const { return (k < inputNS::KEYS_ARRAY_LEN); }
	inline bool inKey(size_t k) const { return (k < inputNS::KEYS_ARRAY_LEN); }
public:
	//�R���X�g���N�^
	Input();
	//�f�X�g���N�^
	virtual ~Input();
	//������
	void initialize(HWND hwnd);

	//�L�[����
	void keyDown(WPARAM);
	void keyUp(WPARAM);
	void keyIn(WPARAM);
	//�L�[�̗L�����m�F
	bool isKeyDown(UCHAR) const;
	bool isKeyPressed(UCHAR) const;
	bool anyKeyPressed() const;
	//�}�E�X����
	void mouseIn(LPARAM);
	//���͏�����
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
