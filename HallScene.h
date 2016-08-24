#ifndef __HALL_SCENE_H
#define __HALL_SCENE_H

#include "cocos2d.h"

#include "PublicDef.h"

MIGU_NS_COCOS

struct LoginDataInfo;

class HallScene : public cocos2d::Layer{
public:
	virtual bool init();

	static cocos2d::Scene* createScene();

	CREATE_FUNC(HallScene);

private:
	void startLogin();
	void loginCallBack(int status, const std::string& loginInfo);

	void showGameHall();

	void applyGameList();
	void applyMatchList();
	void showHallFrame();

	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;

	void configureBacground();
	void configureSideButton();

	void uiCallback(cocos2d::Ref* sender);
};

MIGU_NS_COCOS_END

#endif;