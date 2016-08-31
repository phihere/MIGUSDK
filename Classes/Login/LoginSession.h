#ifndef __LOGIN_SESSION_H
#define __LOGIN_SESSION_H


#include <memory>
#include <string>
#include <functional>

#include "../PublicDef.h"

MIGU_NS_COCOS

class LoginSession{
public:
	explicit LoginSession(bool isMobileUser):isMobileUser(isMobileUser){};

	void login();

	void updateUserInfo();

	void requestServiceIP();

	void startLogin(const std::string& ipport);

	bool mobileUser() const {return isMobileUser;}

	void updateUserInfo(const std::string& info);
private:

	std::string channelNo;
	std::string channelID;
	std::string serverURL;
	bool isMobileUser;
};

MIGU_NS_COCOS_END

#endif