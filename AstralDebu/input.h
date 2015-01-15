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

//�L�[���͂��󂯎��N���X
class Input {
private:
	//������Ă���L�[
	bool keysDown[inputNS::KEYS_ARRAY_LEN];
	//���܂��ɉ����ꂽ�L�[
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];
	//���͕�����R�}���h�p
	std::string textIn;
	char charIn;
	bool newLine;
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
	//���͏�����
	void clearKeyPress(UCHAR);
	void clear(UCHAR);
	void clearAll() { clear(inputNS::ALL); }
	void clearTextIn() { textIn.clear(); }
	//�����񑀍�
	std::string getTextIn() { return textIn; };
	char getCharIn() { return charIn; }
};

#endif
