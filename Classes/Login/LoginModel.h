#ifndef __LOGIN_MODEL_H
#define __LOGIN_MODEL_H


#include <string>

#include "../PublicDef.h"

MIGU_NS_COCOS

enum class Gender
{
	male = 1,
	female = 2
};

class LoginModel{
public:
	void init(const std::string& token, const std::string& username){this->token = token; this->miguID = username;}

	void update(const std::string& miguID, const std::string& serviceID, const std::string& nickname, const std::string &portraitUrl, Gender gender )
	{this->miguID = miguID; this->serviceID = serviceID, this->nickname = nickname; this->portraitUrl = portraitUrl; this->gender = gender;}

	void update(const std::string& nickname, Gender gender){this->nickname = nickname; this->gender = gender;};

	std::string getToken() const {return token;}
	std::string getPortraitUrl() const {return portraitUrl;}
	std::string getMiguID() const {return miguID;}
	std::string getMobilePseudoID() const {return mobilePseudoID;}
	std::string getServiceID() const {return serviceID;}
	std::string getNickname() const {return nickname;}
	Gender getGender() const {return gender;}

private:
	Gender gender;
	std::string portraitUrl;
	std::string miguID;
	std::string mobilePseudoID;
	std::string serviceID;
	std::string nickname;
	std::string token;
};

MIGU_NS_COCOS_END

#endif