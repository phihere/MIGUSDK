#include "GameResourceManager.h"

MIGU_NS_COCOS

//const char* gameUrl = "http://download.cmgame.com:8080/entry?C=0300000001&ContentID=608716052919&F=5617688_5617689&T=c537609ca63a40e3bb1957d4fbdd154b&D=0&Y=2&H=10558000&M=0&P=1&G=0&U=0&E=6118&R=DL&A=608716052919&W=760000098765&CFM=1&GCT=0&FN=%5Bg.10086.cn%5DMGGD.apk&CPN=0&PLT=3&PLK=dde18a88cfbcf52ff461d2241d9b884c20160824180553&TIP=58.213.102.242&S=8eff43806c648c4ab4fcc3c6d57a1a52";
const char* gameUrl = "http://www.soideas.cn/uploads/allimg/120612/3-www.soideas.cn1206120HQ5.jpg";
const char* gameStorePath = "download_game/guandan.apk";
const char* gameDir = "download_game/";

using namespace cocos2d;
using namespace std;
USING_NS_CC_EXT;

void errorCallback(const Downloader::Error & error);
void progressCallback(double d0, double d1, const std::string &s0, const std::string &s1);
void successCallback(const std::string &s0, const std::string &s1, const std::string &s2);

GameResourceManager& GameResourceManager::getInstance() {
	static GameResourceManager instance;
	return instance;
}

GameResourceManager::GameResourceManager():downloading(false){
	 downloader = std::make_shared<Downloader>();
	 using namespace std::placeholders;
	 auto errorBack = bind(&errorCallback, _1);
	 auto progressBack = bind(&progressCallback, _1, _2, _3, _4);
	 auto successBack = bind(&successCallback, _1, _2, _3);
	 downloader->setErrorCallback(errorBack);
	 downloader->setProgressCallback(progressBack);
	 downloader->setSuccessCallback(successBack);
	 FileUtils::getInstance()->createDirectory(gameDir);
}

bool GameResourceManager::gameHasDownloaded(GameType type) {
	return FileUtils::getInstance()->isFileExist(gameStorePath);
}

void GameResourceManager::downloadGame(GameType type, const DownloadCallback& callback) {
	 downloader->downloadAsync(gameUrl, gameStorePath);	
	 downloading = true;
}

string GameResourceManager::getGameResourcePath(GameType type) {
	return FileUtils::getInstance()->fullPathForFilename(gameStorePath);
}

void errorCallback(const Downloader::Error & error){
	
	CCLOG("1");
}
void progressCallback(double d0, double d1, const std::string &s0, const std::string &s1){

	CCLOG("2");
}

void successCallback(const std::string &s0, const std::string &s1, const std::string &s2){

	CCLOG("3");
}



MIGU_NS_COCOS_END