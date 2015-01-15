#include "input.h"

//�R���X�g���N�^�@�e�l��������
Input::Input(){
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)  keysDown[i] = false;
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)  keysPressed[i] = false;
	newLine = true;
	textIn = "";
	charIn = 0;
}

//�f�X�g���N�^�@���ɉ������Ȃ�
Input::~Input(){

}

//�������@�L�[�{�[�h�����g��Ȃ��̂ŉ������Ȃ�
void Input::initialize(HWND hwnd){}

//�L�[�������ꂽ��
void Input::keyDown(WPARAM wp){
	if (inKey(wp)){
		if (!keysDown[wp]){
			keysDown[wp] = true;
			keysPressed[wp] = true;
		}
	}
}

//�L�[�������ꂽ��
void Input::keyUp(WPARAM wp){
	if (inKey(wp)){
		keysDown[wp] = false;
		//pressed�͂�����false�ɂȂ�̂Ŗ���
	}
}

//�L�[����
void Input::keyIn(WPARAM wp){
	//���s
	if (newLine) {
		textIn.clear();
		newLine = false;
	}
	//�߂�
	if (wp == '\b') {
		if (textIn.length() > 0) textIn.erase(textIn.size() - 1);
		//����ȊO�Ȃ�ǉ�
	}
	else {
		textIn += wp;
		charIn = wp;
	}
	//���s�iEnter�j�����
	if (wp == '\r' || wp == 'z' || wp == 'x' || wp == 'c') newLine = true;
}

//�L�[��������Ă��邩
bool Input::isKeyDown(UCHAR k) const {
	if (inKey(k)) return keysDown[k];
	else return false;
}

//�L�[���������ꂽ��
bool Input::isKeyPressed(UCHAR k) const {
	if (inKey(k)) return keysPressed[k];
	else return false;
}

//�����ꂩ�̃L�[�������ꂽ��
bool Input::anyKeyPressed() const {
	for (size_t i = 0; inKey(i); i++){
		if (keysPressed[i]) return true;
	}
	return false;
}

//Press�̏�����
void Input::clearKeyPress(UCHAR k){
	if (inKey(k)) keysPressed[k] = false;
}

//������
void Input::clear(UCHAR w){
	if (w&inputNS::KEYS_DOWN){
		for (size_t i = 0; inKey(i); i++) keysDown[i] = false;
	}
	if (w&inputNS::KEYS_PRESSED){
		for (size_t i = 0; inKey(i); i++) keysPressed[i] = false;
	}
	if (w&inputNS::TEXT_IN) clearTextIn();
}