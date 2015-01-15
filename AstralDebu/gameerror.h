#ifndef _GAMEERROR_H
#define _GAMEERROR_H
#define WIN32_LEAN_AND_MEAN
#include <string>
#include <exception>

//�G���[�̎��
namespace gameErrorNS{
	const int FATAL = -1;
	const int WARNING = 1;
}

//�G���[��O��Ԃ��N���X
class GameError : public std::exception{
private:
	int errorCode;
	std::string message;
public:
	//���R���X�g���N�^
	GameError() throw() : errorCode(gameErrorNS::FATAL), message("Undefined Error in Game."){};
	//�R�s�[�R���X�g���N�^
	GameError(const GameError& e) throw() : errorCode(e.errorCode), message(e.message){};
	//�����t���R���X�g���N�^
	GameError(int code, const std::string &s) throw() : errorCode(code), message(s){};
	//�I�y���[�^
	GameError& operator = (const GameError& rhs) throw() {
		std::exception::operator=(rhs);
		this->errorCode = rhs.errorCode;
		this->message = rhs.message;
	}
	//�f�X�g���N�^�@���ɉ������Ȃ�
	virtual ~GameError() throw() {};
	//�ȍ~�I�[�o�[���C�h
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