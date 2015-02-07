#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <Mmsystem.h>
#include "const.h"
#include "gameerror.h"
#include "input.h"
#include "graphics.h"
#include "text.h"
#include "audio.h"

namespace gameNS {
	const std::string FONT = "Arial";
	const int POINT_SIZE = 14;
	const ARGB FONT_COLOR = graphicsNS::WHITE;
	const int DBG_MAR_X = 100;
	const int DBG_MAR_Y = 16;
}

class Game {
protected:
	//�O���t�B�b�N�X�Ƃ�
	Graphics *graphics;
	Input *input;
	Text *text;
	Audio *audio;
	//�E�B���h�E�n���h��
	HWND hwnd;
	HRESULT hr;
	//���ԊǗ�
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timeFreq;
	float frameTime;
	DWORD sleep;
	float fps;
	bool fpsOn;
	//��ʒ�~
	bool pause;
	//�������ς�
	bool initialized;
	//�f�o�b�O�R�}���h
	std::string command;
	bool cheat1, cheat2, cheat3;

public:
	//�R���X�g���N�^
	Game();
	//�f�X�g���N�^
	virtual ~Game();
	//���b�Z�[�W�n���h��
	LRESULT messageHandler(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
	//������
	virtual void initialize(HWND hw);
	//���s���[�v
	virtual void run(HWND hw);
	//�`��
	virtual void renderGame();
	//�f�o�C�X����ꂽ�����f����
	virtual void handleLostGraphicsDevice();
	//�|�C���^�̉���Ƃ�
	virtual void releaseAll();
	virtual void resetAll();
	virtual void deleteAll();

	//getter
	Graphics* getGraphics() { return graphics; }
	Audio* getAudio() { return audio; }
	Input* getInput() { return input; }
	//�I�����b�Z�[�W
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }
	//���z�֐�
	virtual void update() = 0;
	virtual void render() = 0;
};
#endif
