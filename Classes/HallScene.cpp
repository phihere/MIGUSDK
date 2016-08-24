#include "HallScene.h"

#include "Login/LoginCenter.h"
#include "Login/LoginSession.h"

MIGU_NS_COCOS

LoginDataInfo loginDataInfo();

using namespace cocos2d;

bool HallScene::init(){
	if (!Layer::init()){
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	startLogin();

	return true;
}

Scene* HallScene::createScene(){
    auto scene = Scene::create();
    auto layer = HallScene::create();
    scene->addChild(layer);
    return scene;
}

void HallScene::startLogin(){
	using namespace std::placeholders;

	auto loginCallBack = bind(&HallScene::loginCallBack, this, _1, _2);
	LoginCenter::getInstance().registerLoginCallback(loginCallBack);
	auto loginSession = LoginCenter::getInstance().newLoginSession(true);
	loginSession->login();
}

void HallScene::loginCallBack(int status, const std::string& loginInfo){
	if (status == 0){
		showGameHall();
	}else{
	}

}

void HallScene::showGameHall(){
	CCLOG("showHall");

	applyGameList();
	applyMatchList();
	showHallFrame();
}

void HallScene::applyGameList(){
}
void HallScene::applyMatchList(){
}

void HallScene::showHallFrame(){
	configureBacground();
	configureSideButton();
}

LoginDataInfo loginDataInfo(){
	LoginDataInfo info;
	info.brand = "win";
	info.channelID = "ss";

	return info;
}

void HallScene::configureBacground(){
	auto backSprite = Sprite::create("public_bg.jpg");
	CCLOG("%f",visibleSize.width);
	CCLOG("%f",visibleSize.height);
	
	backSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(backSprite);

}


void HallScene::configureSideButton(){
	auto chatItem = MenuItemImage::create("chatEntryFrame1.png","chatEntryFrame1.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	auto singleItem = MenuItemImage::create("single_btn.png","single_btn.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	chatItem->setAnchorPoint(Vec2(0,0.5));
	singleItem->setAnchorPoint(Vec2(1, 0.5));
	chatItem->setPosition(0, visibleSize.height / 2);
	singleItem->setPosition(visibleSize.width, visibleSize.height / 2);

	auto sideMenu = Menu::create(chatItem, singleItem, nullptr);
	sideMenu->setPosition(Vec2::ZERO);
	addChild(sideMenu);
}

void HallScene::uiCallback(Ref* sender){
}


MIGU_NS_COCOS_END
