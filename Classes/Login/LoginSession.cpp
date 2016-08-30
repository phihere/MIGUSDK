#include "LoginSession.h"

#include "cocos2d.h"
#include "network/HttpClient.h"
#include "LoginCenter.h"
#include "tinyxml2\tinyxml2.h"

MIGU_NS_COCOS
   
using namespace std;
using namespace cocos2d;
using namespace network;
using namespace std::placeholders;

void receiveLoginHttpResponse(HttpClient* client, HttpResponse* response);
void receiveIPHttpResponse(HttpClient* client, HttpResponse* response);
void receiveUserInfoHttpResponse(HttpClient* client, HttpResponse* response);
void receiveUpdateUserInfoHttpResponse(HttpClient* client, HttpResponse* response);

const char *Ip_List_Server_Url = "http://121.15.156.172:45266/lobbyiplist";

void LoginSession::login(){
	CCLOG("Login");
	requestServiceIP();
}

void LoginSession::requestServiceIP(){
	auto *request = new HttpRequest();

	string requestData = LoginCenter::getInstance().requestIPData();
	string url = string(Ip_List_Server_Url) + requestData;
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::GET);

	CCLOG("申请服务ip:port");
	//CCLOG(request->getUrl());  
	
	auto ccCallBack = bind(&receiveIPHttpResponse, _1, _2);
	request->setResponseCallback(ccCallBack);
    HttpClient::getInstance()->send(request);
	request->release();
}

void receiveIPHttpResponse(HttpClient* client, HttpResponse* response){
	tinyxml2::XMLDocument doc;

	if (!response->isSucceed()){
		CCLOG("申请服务ip:port，失败");
		//CCLOG(response->getErrorBuffer());
		CCLOG("%d", (int)response->getResponseCode());
	}

	auto data = response->getResponseData();
	char* xmlChar = new char[data->size()];
	for(int i = 0; i < data->size(); ++i){
		xmlChar[i] = (*data)[i];
	}
	CCLOG("申请服务ip:port，成功");
	//CCLOG(xmlChar);
	
	doc.Parse(xmlChar, data->size());
	auto xmlRoot = doc.RootElement();
	auto dataElement = xmlRoot->FirstChildElement()->NextSiblingElement()->NextSiblingElement();
	auto elementElement = dataElement->FirstChildElement()->NextSiblingElement()->FirstChildElement();
	auto gateElement = elementElement->FirstChildElement()->NextSiblingElement();
	auto element1Element = gateElement->FirstChildElement();
	auto ipElement = element1Element->FirstChildElement();
	auto portElement = ipElement->NextSiblingElement();
	string serverIP = ipElement->GetText();
	string port = portElement->GetText();
	string ipPort = string("http://") + serverIP + string(":") + port;

	LoginCenter::getInstance().startLogin(ipPort);
	
	delete xmlChar;
}

void receiveLoginHttpResponse(HttpClient* client, HttpResponse* response){

	tinyxml2::XMLDocument doc;

	if (!response->isSucceed()){
		CCLOG("登录，失败");
		//CCLOG(response->getErrorBuffer());
		CCLOG("%d", (int)response->getResponseCode());
	}

	auto data = response->getResponseData();
	char* xmlChar = new char[data->size()];
	for(int i = 0; i < data->size(); ++i){

		xmlChar[i] = (*data)[i];
	}
	CCLOG("登录，成功");
	//CCLOG(xmlChar);

	doc.Parse(xmlChar, data->size());
	auto xmlRoot = doc.RootElement();
	auto codeStr = xmlRoot->Attribute("code");
	stringstream ss(codeStr);
	int code = 0;
	ss >> code;
	delete xmlChar;
	if (code == 0){
		std::string token = xmlRoot->Attribute("token");
		std::string username = xmlRoot->Attribute("user_name");
		LoginCenter::getInstance().loginSuccess(token, username);
	}else{
		LoginCenter::getInstance().loginResponse(1, "");
	}

}

void LoginSession::startLogin(const string& ipport){
	serverURL = ipport;

	auto *request = new HttpRequest();

	request->setUrl((ipport + string("/third_logon_mix.app")).c_str());
	request->setRequestType(HttpRequest::Type::POST);

	CCLOG("发起登录请求");
	string requestData = LoginCenter::getInstance().getXMLLoginData();
	//CCLOG(requestData.c_str());
	request->setRequestData(requestData.c_str(), requestData.size());
	//CCLOG(request->getUrl());
	
	auto ccCallBack = bind(&receiveLoginHttpResponse, _1, _2);
	request->setResponseCallback(ccCallBack);
    HttpClient::getInstance()->send(request);
	request->release();
}

void LoginSession::updateUserInfo(){
	auto infoUrl = serverURL + string("/get_user_info.app");

	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	auto root = doc.NewElement("post");
	doc.LinkEndChild(root);

	root->SetAttribute("fmt","xml");
	root->SetAttribute("app_id", "10001");
	root->SetAttribute("app_ver","1");
	root->SetAttribute("token", LoginCenter::getInstance().getLoginModel()->getToken().c_str());
	root->SetAttribute("sign","");

	auto print = new XMLPrinter();
	doc.Print(print);
	string result = string(print->CStr());
	delete print;

	//CCLOG(infoUrl.c_str());
	//CCLOG(result.c_str());

	CCLOG("获取用户信息");
	auto *request = new HttpRequest();

	request->setUrl(infoUrl.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setRequestData(result.c_str(), result.size());

	auto ccCallBack = bind(&receiveUserInfoHttpResponse, _1, _2);
	request->setResponseCallback(ccCallBack);
	HttpClient::getInstance()->send(request);

	request->release();
}

void receiveUserInfoHttpResponse(HttpClient* client, HttpResponse* response){
	tinyxml2::XMLDocument doc;

	if (!response->isSucceed()){
		CCLOG("获取用户信息, 失败");
		//CCLOG(response->getErrorBuffer());
		CCLOG("%d", (int)response->getResponseCode());
	}

	auto data = response->getResponseData();
	char* xmlChar = new char[data->size()];
	for(int i = 0; i < data->size(); ++i){

		xmlChar[i] = (*data)[i];
	}
	CCLOG("获取用户信息, 成功");
	//CCLOG(xmlChar);

	doc.Parse(xmlChar, data->size());
	auto xmlRoot = doc.RootElement();
	
	std::string guid = xmlRoot->Attribute("guid");
	std::string uid = xmlRoot->Attribute("uid");
	std::string username = xmlRoot->Attribute("user_name");
	std::string nickname = xmlRoot->Attribute("nick_name");
	std::string userAvatar = xmlRoot->Attribute("user_avatar");
	std::string sex = xmlRoot->Attribute("sex");
	stringstream ss(sex);
	int gender = 0;
	ss >> gender;

	LoginCenter::getInstance().updateLoginModel(guid, uid, nickname, userAvatar, Gender(gender));
	LoginCenter::getInstance().loginResponse(0, "");
}

void LoginSession::updateUserInfo(const string& info){
	string url = serverURL + string("/mod_user_info.app");

	auto *request = new HttpRequest();
	request->setUrl(url.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setRequestData(info.c_str(), info.size());

	auto ccCallBack = bind(&receiveUpdateUserInfoHttpResponse, _1, _2);
	request->setResponseCallback(ccCallBack);
	HttpClient::getInstance()->send(request);

	request->release();
}

void receiveUpdateUserInfoHttpResponse(HttpClient* client, HttpResponse* response){
	tinyxml2::XMLDocument doc;

	if (!response->isSucceed()){
		CCLOG("更新用户信息，失败");
		//CCLOG(response->getErrorBuffer());
		CCLOG("%d", (int)response->getResponseCode());
		LoginCenter::getInstance().updateUserInfoCallBack(4, "error");
		return;
	}

	auto data = response->getResponseData();
	char* xmlChar = new char[data->size()];
	for(int i = 0; i < data->size(); ++i){

		xmlChar[i] = (*data)[i];
	}
	CCLOG("更新用户信息，成功");
	//CCLOG(xmlChar);

	doc.Parse(xmlChar, data->size());
	auto xmlRoot = doc.RootElement();
	auto codeStr = xmlRoot->Attribute("code");
	stringstream ss(codeStr);
	int code = 0;
	ss >> code;
	delete xmlChar;
	LoginCenter::getInstance().updateUserInfoCallBack(code, "");
}




MIGU_NS_COCOS_END