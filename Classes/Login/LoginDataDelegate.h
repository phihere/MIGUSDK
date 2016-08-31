#ifndef __LOGIN_DATA_DELEGATE_H
#define __LOGIN_DATA_DELEGATE_H

#include <string>

#include "../PublicDef.h"

MIGU_NS_COCOS

class LoginDataDelegate{
	virtual std::string getChannelNo() = 0;

	virtual std::string getChannelID() = 0;
	
	virtual bool isMobileUser() = 0;
};


MIGU_NS_COCOS_END


#endif