#include "LoginCenter.h"

#include <ctime>
#include <sstream>

#include "cocos2d.h"

#include "LoginSession.h"
#include "LoginModel.h"

#include "../md5/migu_md5.h"

#include "tinyxml2\tinyxml2.h"


MIGU_NS_COCOS

using namespace std;

const char* Secrect = "2e3aa31da2886b5be5cbf7c104a0da37";

LoginCenter::LoginCenter(){
	model = make_shared<LoginModel>();
}

LoginCenter& LoginCenter::getInstance(){
	static LoginCenter instance;
	return instance;
}

shared_ptr<LoginSession> LoginCenter::newLoginSession(bool isMobileUser){
	auto loginSession = make_shared<LoginSession>(isMobileUser);
	nowSession = loginSession;
	return loginSession;
}

void LoginCenter::updateLoginModel(const std::string& miguID, const std::string& serviceID, const std::string& nickname, const std::string &portraitUrl, Gender gender ){
	model->update(miguID, serviceID, nickname, portraitUrl, gender);
}

void LoginCenter::loginResponse(int status, const std::string& loginInfo){
	loginCallback(status, loginInfo);
}

void LoginCenter::startLogin(const string& ipport){
	serverIpPort = ipport;
	nowSession->startLogin(ipport);
}

std::string LoginCenter::getXMLLoginData() const{
	auto info = loginDataProvider();
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	auto root = doc.NewElement("post");
	doc.LinkEndChild(root);
	
	root->SetAttribute("fmt","xml");
	root->SetAttribute("login_type","0");
	root->SetAttribute("app_id", "2");
	root->SetAttribute("app_ver", "0");
	if (nowSession->mobileUser()){
		root->SetAttribute("third_user_id", info->thirdID.c_str());
		root->SetAttribute("third_mix_user_id", info->mixThirdID.c_str());
		root->SetAttribute("third_type", 1);
	}else{
		root->SetAttribute("third_user_id", info->thirdID.c_str());
		root->SetAttribute("third_mix_user_id", "");
		root->SetAttribute("third_type", 2);
	}

	root->SetAttribute("client_id",10001);
	root->SetAttribute("sub_client_id",info->subChannelNo);
	root->SetAttribute("os", 1);
	root->SetAttribute("os_ver", "0");
	root->SetAttribute("imei", info->deviceID.c_str());
	root->SetAttribute("imsi",info->imsi.c_str());
	root->SetAttribute("iccid",info->iccid.c_str());
	root->SetAttribute("brand", info->brand.c_str());
	root->SetAttribute("model",info->model.c_str());
	root->SetAttribute("longtude", "");
	root->SetAttribute("latitude", "");
	root->SetAttribute("sign", "");
	
	auto print = new XMLPrinter();
	doc.Print(print);
	string result = string(print->CStr());

	delete print;

	return result;
}

std::string LoginCenter::requestIPData() const{
	auto info = loginDataProvider();
	time_t timeStamp;
	time(&timeStamp);
	stringstream ss;
	ss<<timeStamp; 
	string timeString = ss.str();

	stringstream ss1;
	ss1 << info->subChannelNo;
	string subChannelNoStr = ss1.str();

	string origin = string("format=xmlop=") + timeString + string("secret=") + string(Secrect);
	CCLOG("md5");
	//CCLOG(origin.c_str());
	MD5 md5(origin);
	auto result =  string("?cid=10001&sid=") + subChannelNoStr + string("&ver=4&os=1") + string("&format=xml&op=") + timeString + string("&sign=") + md5.toStr();

	return result;
}

void LoginCenter::loginSuccess(const std::string& token, const std::string& username){
	model->init(token, username);
	nowSession->updateUserInfo();

}


void LoginCenter::updateUserInfo(const std::string& nickname, Gender gender, const LoginCallback& callback){
	tinyxml2::XMLDocument doc;
	auto root = doc.NewElement("post");
	doc.LinkEndChild(root);
	root->SetAttribute("fmt","xml");
	root->SetAttribute("app_id", "2");
	root->SetAttribute("app_ver", "0");
	root->SetAttribute("token", model->getToken().c_str());
	root->SetAttribute("nick_name", model->getNickname().c_str());
	root->SetAttribute("sex", (int)gender);
	root->SetAttribute("sign", "");

	auto print = new tinyxml2::XMLPrinter();
	doc.Print(print);
	string result = string(print->CStr());

	delete print;
	nowSession->updateUserInfo(result);

	updateUserInfoCallback = callback;
}

void LoginCenter::updateUserInfoCallBack(int status, const string& info){
	updateUserInfoCallback(status, info);
}


MIGU_NS_COCOS_END










