#include "input.h"

//コンストラクタ　各値を初期化
Input::Input(){
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)  keysDown[i] = false;
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)  keysPressed[i] = false;
	newLine = true;
	textIn = "";
	charIn = 0;
	mouseX = 0;
	mouseY = 0;
	mouseLDown = false;
	mouseMDown = false;
	mouseRDown = false;
	mouseLPressed = false;
	mouseMPressed = false;
	mouseRPressed = false;
}

//デストラクタ　特に何もしない
Input::~Input(){

}

//初期化　キーボードしか使わないので何もしない
void Input::initialize(HWND hwnd){}

//キーが押された時
void Input::keyDown(WPARAM wp){
	if (inKey(wp)){
		if (!keysDown[wp]){
			keysDown[wp] = true;
			keysPressed[wp] = true;
		}
	}
}

//キーが離された時
void Input::keyUp(WPARAM wp){
	if (inKey(wp)){
		keysDown[wp] = false;
		//pressedはすぐにfalseになるので無視
	}
}

//キー入力
void Input::keyIn(WPARAM wp){
	//改行
	if (newLine) {
		textIn.clear();
		newLine = false;
	}
	//戻る
	if (wp == '\b') {
		if (textIn.length() > 0) textIn.erase(textIn.size() - 1);
		//それ以外なら追加
	}
	else {
		textIn += wp;
		charIn = wp;
	}
	//改行（Enter）を入力
	if (wp == '\r' || wp == 'z' || wp == 'x' || wp == 'c') newLine = true;
}

//マウス操作
void Input::mouseIn(LPARAM lp){
	mouseX = GET_X_LPARAM(lp);
	mouseY = GET_Y_LPARAM(lp);
}

//キーが押されているか
bool Input::isKeyDown(UCHAR k) const {
	if (inKey(k)) return keysDown[k];
	else return false;
}

//キーが今押されたか
bool Input::isKeyPressed(UCHAR k) const {
	if (inKey(k)) return keysPressed[k];
	else return false;
}

//いずれかのキーが押されたか
bool Input::anyKeyPressed() const {
	for (size_t i = 0; inKey(i); i++){
		if (keysPressed[i]) return true;
	}
	return false;
}

//Pressの初期化
void Input::clearKeyPress(UCHAR k){
	if (inKey(k)) keysPressed[k] = false;
}

//初期化
void Input::clear(UCHAR w){
	if (w&inputNS::KEYS_DOWN){
		for (size_t i = 0; inKey(i); i++) keysDown[i] = false;
	}
	if (w&inputNS::KEYS_PRESSED){
		for (size_t i = 0; inKey(i); i++) keysPressed[i] = false;
	}
	if (w & inputNS::MOUSE) {
		mouseX = 0;
		mouseY = 0;
	}
	if (w & inputNS::MOUSE_DOWN) {
		mouseLDown = false;
		mouseMDown = false;
		mouseRDown = false;
	}
	if (w & inputNS::MOUSE_PRESSED) {
		mouseLPressed = false;
		mouseMPressed = false;
		mouseRPressed = false;
	}
	if (w&inputNS::TEXT_IN) clearTextIn();
}