#include "game.h"

//�R���X�g���N�^
Game::Game(){
	graphics = NULL;
	input = NULL;
	text = NULL;
	audio = NULL;
	pause = false;
	fps = 100;
	fpsOn = false;
	initialized = false;
}

//�f�X�g���N�^
Game::~Game(){
	deleteAll();
}

//���b�Z�[�W�擾
LRESULT Game::messageHandler(HWND hw, UINT msg, WPARAM wp, LPARAM lp){
	if (initialized){
		switch (msg){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN: case WM_SYSKEYDOWN:
			input->keyDown(wp);
			return 0;
		case WM_KEYUP: case WM_SYSKEYUP:
			input->keyUp(wp);
			return 0;
		case WM_CHAR:
			input->keyIn(wp);
			return 0;
		case WM_MOUSEMOVE:
			input->mouseIn(lp);
			return 0;
		case WM_LBUTTONDOWN:
			if (!input->getMouseLDown()) input->setMouseLPressed(true);
			input->setMouseLDown(true);
			input->mouseIn(lp);
			return 0;
		case WM_LBUTTONUP:
			input->setMouseLDown(false);
			input->mouseIn(lp);
			return 0;
		case WM_MBUTTONDOWN:
			if (!input->getMouseMDown()) input->setMouseMPressed(true);
			input->setMouseMDown(true);
			input->mouseIn(lp);
			return 0;
		case WM_MBUTTONUP:
			input->setMouseMDown(false);
			input->mouseIn(lp);
			return 0;
		case WM_RBUTTONDOWN:
			if (!input->getMouseRDown()) input->setMouseRPressed(true);
			input->setMouseRDown(true);
			input->mouseIn(lp);
			return 0;
		case WM_RBUTTONUP:
			input->setMouseRDown(false);
			input->mouseIn(lp);
			return 0;
		}
	}
	return DefWindowProc(hw, msg, wp, lp);
}

//������
void Game::initialize(HWND hw){
	hwnd = hw;

	//�O���t�B�b�N�X������
	graphics = new Graphics();
	graphics->initialize(hwnd, WINDOW_W, WINDOW_H);

	//���͏�����
	input = new Input();
	input->initialize(hwnd);

	//����������

	audio = new Audio();
	if (FAILED(hr = audio->initialize())){
		if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			throw(GameError(gameErrorNS::FATAL, "�����t�@�C����������܂���ł����D"));
		else
			throw(GameError(gameErrorNS::FATAL, "�����̏������Ɏ��s���܂���"));
	}

	//�ėp�t�H���g������
	text = new Text();
	if (text->initialize(
		graphics, gameNS::POINT_SIZE,
		false, false, gameNS::FONT) == false)
		throw(GameError(gameErrorNS::FATAL, "�t�H���g�̏������Ɏ��s���܂���"));
	text->setFontColor(gameNS::FONT_COLOR);

	//�^�C�}�[������
	if (QueryPerformanceFrequency(&timeFreq) == false)
		throw(GameError(gameErrorNS::FATAL, "�^�C�}�̏������Ɏ��s���܂���"));
	QueryPerformanceCounter(&timeStart);
	initialized = true;
}

//���s���[�v
void Game::run(HWND hw){
	if (graphics == NULL) return;
	//���s���Ԃ𒲂ׂ�
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) /
		(float)timeFreq.QuadPart;

	//�Z������Ȃ�҂�
	if (frameTime < FRAME_TIME_MIN){
		sleep = (DWORD)((FRAME_TIME_MIN - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleep);
		timeEndPeriod(1);
		return;
	}
	//fps�𑀍�
	if (frameTime > 0.0) fps = (fps*0.99f) + (0.01f / frameTime);

	//��������Ȃ�؂�̂Ă�
	if (frameTime > FRAME_TIME_MAX) frameTime = FRAME_TIME_MAX;
	timeStart = timeEnd;

	//�����X�V
	if (!pause){
		update();
	}

	//�`��
	renderGame();

	//����
	audio->run();

	//���͂�������
	input->clear(inputNS::KEYS_PRESSED + inputNS::MOUSE_PRESSED);
}

//�`��
void Game::renderGame(){
	const int BUF_SIZE = 20;
	static char buffer[BUF_SIZE];

	//�`��J�n
	if (SUCCEEDED(graphics->beginScene())){
		//�`��
		render();
		//�f�o�b�O����\��
		graphics->spriteBegin();
		if (fpsOn){
			_snprintf_s(buffer, BUF_SIZE, "FPS : %3.3f", fps);
			text->print(buffer, WINDOW_W - gameNS::DBG_MAR_X, 0, gameNS::DBG_MAR_X, gameNS::DBG_MAR_Y, graphicsNS::WHITE, DT_CC);
		}
		//�`��I��
		graphics->spriteEnd();
		graphics->endScene();
	}
	//�f�o�C�X�������m�F
	handleLostGraphicsDevice();
	//�o�b�t�@�ɕ\��
	graphics->showBackBuffer();
}


//�f�o�C�X����ꂽ�����f����
void Game::handleLostGraphicsDevice(){
	hr = graphics->getDeviceState();
	if (FAILED(hr)){
		//�����̑����Ȃ�҂�
		if (hr == D3DERR_DEVICELOST){
			Sleep(100);
			return;
		}
		//�C���s�\�Ȃ�ċN��
		else if (hr == D3DERR_DEVICENOTRESET){
			releaseAll();
			hr = graphics->reset();
			if (FAILED(hr)) return;
			resetAll();
		}
		else return;

	}
}

void Game::releaseAll(){
	SAFE_ON_LOST_DEVICE(text);
}

void Game::resetAll(){
	SAFE_ON_RESET_DEVICE(text);
}

void Game::deleteAll(){
	releaseAll();
	SAFE_DELETE(audio);
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	SAFE_DELETE(text);
	initialized = false;
}