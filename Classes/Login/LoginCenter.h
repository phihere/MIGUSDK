#ifndef __LOGIN_CENTER_H
#define __LOGIN_CENTER_H

#include <memory>
#include <string>
#include <functional>

#include "../PublicDef.h"
#include "LoginModel.h"

MIGU_NS_COCOS

class LoginSession;
class LoginDataDelegate;

typedef std::function<void(int status, const std::string& loginInfo)> LoginCallback;

struct LoginDataInfo{
	int subChannelNo;  //��������  (����)
	std::string thirdID; //α��
	std::string mixThirdID; //��ϵ�����ID
	std::string deviceID; //�豸ID
	std::string imsi; //SIM ID
	std::string iccid; //SIM iccid
	std::string macID; //mac ��ַ
	std::string brand; //�ֻ�ƽ̨
	std::string model; //�ֻ��ͺ�
};

typedef std::function<std::shared_ptr<LoginDataInfo>()> LoginDataProvider;

class LoginCenter final{
public:
	static LoginCenter& getInstance();               

	std::shared_ptr<LoginSession> newLoginSession(bool isMobileUser = false);
	std::shared_ptr<LoginModel> getLoginModel() const {return model;};

	void loginResponse(int status, const std::string& loginInfo);

	bool LoginDone() const {return finishLogin;};

	void updateLoginModel(const std::string& miguID, const std::string& serviceID, const std::string& nickname, const std::string &portraitUrl, Gender gender );

	void registerLoginCallback(const LoginCallback& callback){loginCallback = callback;}
	void registerLoginDataProvider(const LoginDataProvider& provider) {loginDataProvider = provider;}

	std::string getXMLLoginData() const;
	std::string requestIPData() const;

	void startLogin(const std::string& ipport);

	void loginSuccess(const std::string& token, const std::string& username);

	void updateUserInfo(const std::string& nickname, Gender gender, const LoginCallback& callback);
	void updateUserInfoCallBack(int status, const std::string& info);
private:
	LoginCenter();

	LoginCallback loginCallback;
	LoginDataProvider loginDataProvider;
	LoginCallback updateUserInfoCallback;
	std::string serverIpPort;
	bool finishLogin;
	std::shared_ptr<LoginSession> nowSession;
	std::shared_ptr<LoginModel> model;
	std::shared_ptr<LoginDataDelegate> LoginDelegate;

};

MIGU_NS_COCOS_END

#endif