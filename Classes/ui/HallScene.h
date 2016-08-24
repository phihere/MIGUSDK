#ifndef __HALL_SCENE_H
#define __HALL_SCENE_H

#include <vector>
#include <map>

#include "cocos2d.h"

#include "PublicDef.h"
#include "../service/ContentService.h"
#include "../service/HallContent.h"
#include "HallEventDelegate.h"

MIGU_NS_COCOS

struct LoginDataInfo;

extern const char* Lazi_On_Line;
extern const char* HuanLe_On_Line;
extern const char* Match_On_Line;
extern const char* Bean_Count;
extern const char* Add_Red_Point;//�¼�������Ҫ�Ӻ��Ľڵ������
extern const char* Remove_Red_Point; 
extern const char* Change_Nick_Name;
extern const char* Change_Gender; //2 ��Ů��1����
extern const char* Change_Avatar;


class HallScene : public cocos2d::Scene{
public:

	virtual bool init();
	virtual ~HallScene();

	CREATE_FUNC(HallScene);

	void printLog(const char* s){CCLOG(s);}

	void setEventDelegate(HallEventDelegate* delegate){eventDelegate = delegate;};

	virtual void onEnter();
	virtual void onExit();
private:
	HallEventDelegate* eventDelegate;

	std::unordered_map<std::string, cocos2d::Label*> infosMap;
	std::unordered_map<std::string, cocos2d::MenuItem*> redPointInfoMap;

	cocos2d::Layer *layer;
	cocos2d::Label *nicknameLabel;
	cocos2d::MenuItem *avatarItem;

	cocos2d::Sprite *moreMenu;
	cocos2d::MenuItem *settingItem;
	cocos2d::MenuItem *toolkitItem;

	//cocos2d::MenuItem *miguMatchItem;

	cocos2d::Sprite *hallHead;

	bool showMoreMenu;

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
	void configureHeadFrame();
	void configureBottomFrame();
	void configureMainFrame();

	void uiCallback(cocos2d::Ref* sender);

	void addEventListener(const std::string &event, cocos2d::Label* label);
	void onEvent(cocos2d::EventCustom *event);

	void addRedPoingListener(const std::string &itemName, cocos2d::MenuItem* item);

	void configureRedPoint(cocos2d::MenuItem* item, bool add);

	void changeAvatar(const std::string &path);

	void configureMoreMenu();
	void showOrHideMoreMenu(bool show);
	void doHideMoreMenu(float t);
};

MIGU_NS_COCOS_END

#endif;