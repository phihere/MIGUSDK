#ifndef __GAME_RESOURCE_MANAGER_H
#define __GAME_RESOURCE_MANAGER_H

#include <functional>
#include <string>
#include <memory>

#include "../PublicDef.h"
#include "../ui/HallEventDelegate.h"

#include "cocos2d.h"
#include "assets-manager\AssetsManager.h"
#include "cocos-ext.h"

MIGU_NS_COCOS

extern const char* downloadGameEvent;

typedef std::function<void(int process)> DownloadCallback;

class GameResourceManager {
public:
	static GameResourceManager& getInstance();

	bool gameHasDownloaded(GameType type);

	void downloadGame(GameType type, const DownloadCallback& callback);

	std::string getGameResourcePath(GameType type);

	bool isDownloading() const { return downloading; }
	void setDownloading(bool downloading){this->downloading = downloading;}

private:
	GameResourceManager();
	std::shared_ptr<cocos2d::extension::Downloader> downloader;
	bool downloading;

};

MIGU_NS_COCOS_END

#endif // !__GAME_RESOURCE_MANAGER_H
