#ifndef _GAMEERROR_H
#define _GAMEERROR_H
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <exception>

//エラーの種類
namespace gameErrorNS{
	const int FATAL = -1;
	const int WARNING = 1;
}

//エラー例外を返すクラス
class GameError : public std::exception{
private:
	int errorCode;
	std::string message;
public:
	//無コンストラクタ
	GameError() throw() : errorCode(gameErrorNS::FATAL), message("Undefined Error in Game."){};
	//コピーコンストラクタ
	GameError(const GameError& e) throw() : errorCode(e.errorCode), message(e.message){};
	//引数付きコンストラクタ
	GameError(int code, const std::string &s) throw() : errorCode(code), message(s){};
	//オペレータ
	GameError& operator = (const GameError& rhs) throw() {
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}
	//デストラクタ　特に何もしない
	virtual ~GameError() throw() {};
	//以降オーバーライド
	virtual const char* what() const throw() {
		return this->getMessage();
	}
	const char* getMessage() const throw() {
		return message.c_str();
	}
	int getErrorCode() const throw() {
		return errorCode;
	}
};

#endif