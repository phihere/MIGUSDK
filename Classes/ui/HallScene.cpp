#include "HallScene.h"

#include <utility>

#include "../Login/LoginCenter.h"
#include "../Login/LoginSession.h"

MIGU_NS_COCOS

using namespace cocos2d;
using namespace std;

shared_ptr<LoginDataInfo> loginDataInfo();

const char* Lazi_On_Line = "migu_m1";
const char* HuanLe_On_Line = "migu_m2";
const char* Match_On_Line = "migu_m3";
const char* Bean_Count = "migu_bean_count";
const char* Add_Red_Point = "migu_add_red_point";
const char* Remove_Red_Point = "remove_red_point";
const char* Change_Nick_Name = "change_nick_name";
const char* Change_Gender = "change_gender";
const char* Change_Avatar = "change_avatar";

const int Red_Point_Tag = 3985723;

bool HallScene::init(){
	if (!Scene::init()){
        return false;
    }

	layer = Layer::create();
	layer->setPosition(Vec2::ZERO);
	addChild(layer);
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	eventDelegate = nullptr;
	infosMap = std::unordered_map<std::string, Label*>();
	redPointInfoMap = std::unordered_map<std::string, MenuItem*>();
	showMoreMenu = false;

	using namespace placeholders;
	auto eventCallBack = bind(&HallScene::onEvent, this, _1);
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(Add_Red_Point, eventCallBack);
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(Change_Nick_Name, eventCallBack);
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(Change_Gender, eventCallBack);
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(Change_Avatar, eventCallBack);

	showGameHall();
	startLogin();

	return true;
}

HallScene::~HallScene(){
	for (auto item : infosMap){
		string eventName = item.first;
		Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(eventName.c_str());
	}
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(Add_Red_Point);
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(Change_Nick_Name);
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(Change_Gender);
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(Change_Avatar);

	delete eventDelegate;
}

void HallScene::onEnter(){
	Scene::onEnter();
	if (eventDelegate){
		eventDelegate->onEnter();
	}

}
void HallScene::onExit(){
	Scene::onExit();
	if (eventDelegate){
		eventDelegate->onExit();
	}
}

void HallScene::startLogin(){
	using namespace std::placeholders;

	auto loginCallBack = bind(&HallScene::loginCallBack, this, _1, _2);
	LoginCenter::getInstance().registerLoginCallback(loginCallBack);
	LoginCenter::getInstance().registerLoginDataProvider(loginDataInfo);
	auto loginSession = LoginCenter::getInstance().newLoginSession(true);
	loginSession->login();
}

void HallScene::loginCallBack(int status, const std::string& loginInfo){
	if (status == 0){
		configureMoreMenu();
	}else{
	}

}

void HallScene::showGameHall(){
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
	configureHeadFrame();
	configureBottomFrame();
	configureMainFrame();
		
	EventCustom event(Lazi_On_Line);
}

shared_ptr<LoginDataInfo> loginDataInfo(){
	CCLOG("xmlData");
	shared_ptr<LoginDataInfo> info = make_shared<LoginDataInfo>();
	info->brand = "win";
	info->subChannelNo = 11;
	info->deviceID = "dd";
	info->thirdID = "13314234";
	info->mixThirdID = "123424";
	info->macID = "";
	info->iccid = "";
	info->imsi = "";
	
	return info;
}

void HallScene::configureBacground(){
	auto backSprite = Sprite::create("public_bg.jpg");
	CCLOG("%f",visibleSize.width);
	CCLOG("%f",visibleSize.height);
	
	backSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	layer->addChild(backSprite);
}


void HallScene::configureSideButton(){
	ContentService contentService("xml/miscellContent.xml");
	vector<HallContent> items = contentService.loadContent();
	if (items.size() == 0){
		return;
	}
	auto sideMenu = Menu::create();
	for (HallContent item : items)
	{
		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourceURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
		float anAnchorX = (item.getIndex() == 0 ? 0 : 1 );
		float positionX = (item.getIndex() == 0 ? 0 : visibleSize.width );
		menuItem->setAnchorPoint(Vec2(anAnchorX, 0.5));
		menuItem->setPosition(positionX , visibleSize.height / 2);
		sideMenu->addChild(menuItem);
	}

	sideMenu->setPosition(Vec2::ZERO);
	layer->addChild(sideMenu);
}

void HallScene::uiCallback(Ref* sender){
	int code = ((Node*)sender) -> getTag();
	CCLOG("tag is %d", code);
	if (code == int(ServiceCode::Menu_Service)){
		showMoreMenu = !showMoreMenu;
		showOrHideMoreMenu(showMoreMenu);
		return;
	}


	if (eventDelegate){
		eventDelegate->uiService((ServiceCode)code);
	}
}

void HallScene::configureMainFrame(){
	ContentService contentService("xml/mainItem.xml");
	vector<HallContent> items = contentService.loadContent();

	auto mainMenu = Menu::create();
	for (HallContent item : items)
	{

		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourceURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
		if (!item.getEventKey().empty()){
			
			auto label = Label::createWithSystemFont("0", "fonts/arial.ttf", 20,
                                          Size(200, 20), TextHAlignment::CENTER);
			label->setPosition(140, 27);
			menuItem->addChild(label);
			addEventListener(item.getEventKey(), label);
		}
		if (item.getName() == string("miguMatchItem")){
				menuItem->setEnabled(false);
		}
		menuItem->setTag((int)item.getServiceCode());
		float positionX = item.getIndex() * 300 + 200;
		menuItem->setPosition(positionX , visibleSize.height / 2);
		mainMenu->addChild(menuItem);
	}

	mainMenu->setPosition(Vec2::ZERO);
	layer->addChild(mainMenu);
}

void HallScene::configureHeadFrame(){
	auto headFrame = Sprite::create("hall_frame_top.png");
	headFrame->setAnchorPoint(Vec2(0.5, 1));
	headFrame->setPosition(visibleSize.width / 2, visibleSize.height);
	layer->addChild(headFrame);

	auto playerPortrait = Sprite::create("playerInfo_propBg.png");
	playerPortrait->setAnchorPoint(Vec2(0,1));
	playerPortrait->setPosition(Vec2(0, visibleSize.height));
	layer->addChild(playerPortrait);

	auto portraitCircle = Sprite::create("lb_shape_headIcon.png");
	portraitCircle->setPosition(60,80);
	playerPortrait->addChild(portraitCircle);

	auto beanCount = Sprite::create("hall_frame_bean.png");
	beanCount->setPosition(Vec2(250, 50));
	headFrame->addChild(beanCount);

	auto beanLabel = Label::createWithSystemFont("0", "fonts/arial.ttf", 22,
                                          Size(200, 20), TextHAlignment::CENTER);
	beanLabel->setPosition(120, 25);
	beanCount->addChild(beanLabel);
	addEventListener(Bean_Count, beanLabel);

	nicknameLabel  = Label::createWithSystemFont("", "fonts/arial.ttf", 23,
                                          Size(200, 20), TextHAlignment::CENTER);
	nicknameLabel->setPosition(220, 92);
	headFrame->addChild(nicknameLabel);

	avatarItem = MenuItem::create(CC_CALLBACK_1(HallScene::uiCallback, this));
	avatarItem->setContentSize(Size(60, 60));
	avatarItem->setTag(1002);
	auto userInfoMenu =  Menu::create(avatarItem, nullptr);
	userInfoMenu->setPosition(Vec2::ZERO);
	userInfoMenu->setPosition(Vec2(58, 60));
	portraitCircle->addChild(userInfoMenu);
	

	auto addBeanItem = MenuItemImage::create("hall_btn_add.png", "hall_btn_add.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	auto addBeanMenu = Menu::create(addBeanItem, nullptr);
	addBeanMenu->setPosition(Vec2::ZERO);
	addBeanItem->setPosition(Vec2(230, 25));
	beanCount->addChild(addBeanMenu);
	  

	auto gameTitleBack = Sprite::create("hall_titile_bg.png");
	gameTitleBack->setAnchorPoint(Vec2(0.5, 1));
	gameTitleBack->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	layer->addChild(gameTitleBack);

	auto gameTitle = Sprite::create("game_title.png");
	gameTitle->setAnchorPoint(Vec2(0.5, 1));
	gameTitle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	layer->addChild(gameTitle);

	ContentService contentService("xml/topItem.xml");
	vector<HallContent> items = contentService.loadContent();

	auto topMenu = Menu::create();
	for (HallContent item : items)
	{
		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourceURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
		addRedPoingListener(item.getName(), menuItem);
		menuItem->setTag((int)item.getServiceCode());
		float positionX = item.getIndex() * 120 + 850;
		menuItem->setPosition(positionX , 60);
		topMenu->addChild(menuItem);
	}

	topMenu->setPosition(Vec2::ZERO);
	headFrame->addChild(topMenu);

	hallHead = headFrame;
}

void HallScene::configureBottomFrame(){
	auto bottomFrame = Sprite::create("hall_frame_bottom.png");
	bottomFrame->setAnchorPoint(Vec2(0.5, 0));
	bottomFrame->setPosition(Vec2(visibleSize.width / 2, 0));
	layer->addChild(bottomFrame);

	ContentService contentService("xml/bottomItem.xml");
	vector<HallContent> items = contentService.loadContent();

	auto bottomMenu = Menu::create();
	for (HallContent item : items)
	{
		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourceURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
		addRedPoingListener(item.getName(), menuItem);
		float positionX = item.getIndex() * 150 + 80; 
		menuItem->setPosition(positionX, 60);
		bottomMenu->addChild(menuItem);
	}

	bottomMenu->setPosition(Vec2::ZERO);
	bottomFrame->addChild(bottomMenu);

	auto quickStart = MenuItemImage::create("quick_start.png","quick_start.png", CC_CALLBACK_1(HallScene::uiCallback, this)); 
	quickStart->setAnchorPoint(Vec2(1, 0));
	quickStart->setPosition(visibleSize.width, 0);
	layer->addChild(quickStart);
}

void HallScene::addEventListener(const std::string &event, cocos2d::Label *label){
	using namespace placeholders;
	auto eventCallBack = bind(&HallScene::onEvent, this, _1);
	Director::getInstance()->getEventDispatcher()->addCustomEventListener(event.c_str(), eventCallBack);
	infosMap.insert(pair<string, Label*>(event, label));
}


void HallScene::onEvent(EventCustom* event){
	char* userData = (char*)(event->getUserData());
	std::string eventName = event->getEventName();
	
	if (eventName == std::string(Add_Red_Point) || eventName == std::string(Remove_Red_Point)){
		bool isAdd = (eventName == std::string(Add_Red_Point));
		auto pair = redPointInfoMap.find(userData);
		if (pair == redPointInfoMap.end()){
			return;
		}
		auto item = pair->second;
		configureRedPoint(item, isAdd);
		return;
	}

	if (eventName == std::string(Change_Avatar)){
		changeAvatar(userData);
		return;
	}

	if(eventName == std::string(Change_Nick_Name)){
		auto model = LoginCenter::getInstance().getLoginModel();
		model->update(userData, model->getGender());
		nicknameLabel->setString(string(userData));
		return;
	}

	if (eventName == std::string(Change_Gender)){
		stringstream ss(userData);
		int gender = 0;
		ss >> gender;
		auto model = LoginCenter::getInstance().getLoginModel();
		model->update(model->getNickname(), (Gender)gender);
		return;
	}

	auto infoPair = infosMap.find(eventName);
	if (infoPair == infosMap.end()){
		return;
	}
	auto label = infoPair->second;
	label->setString(userData);
}

void HallScene::addRedPoingListener(const std::string &itemName, cocos2d::MenuItem* item){
	redPointInfoMap.insert(pair<string, MenuItem*>(itemName, item));
}

void HallScene::configureRedPoint(cocos2d::MenuItem* item, bool add){
	if(add){
		Rect rect =	item->rect();
		auto redPoint = Sprite::create("redPoint.png");
		redPoint->setTag(Red_Point_Tag);
		redPoint->setPosition(rect.size.width - 5, rect.size.height - 5);
		item->addChild(redPoint);
	}else{
		item->removeChildByTag(Red_Point_Tag);
	}
	
}

void HallScene::changeAvatar(const std::string &path){
	auto avatar = Sprite::create(path);
	avatar->setScale(0.5);
	float radius = 60.0f;
	auto clipNode = ClippingNode::create();
	auto circleNode = DrawNode::create();
    const int maxTrangle = 360;
    Vec2 circleVec2[maxTrangle];
    for (int i=0; i<maxTrangle; i++)
    {
        float x = cosf(i*(M_PI/180.f))*radius;
        float y = sinf(i*(M_PI/180.f))*radius;
        circleVec2[i] = Vec2(x, y);
    }
    auto circleColor = Color4F(0, 1, 0, 1);
    circleNode->drawPolygon(circleVec2, maxTrangle, circleColor, 1, circleColor);

	clipNode->setStencil(circleNode);
	clipNode->addChild(avatar);

	avatarItem->setPosition(30,30);
	avatarItem->addChild(clipNode);
}


void HallScene::configureMoreMenu(){
	auto menuBg = Sprite::create("more_btn_bg.png");
	menuBg->setPosition(1208, -100);
	hallHead->addChild(menuBg);

	auto placehole = MenuItemImage::create("top3.png", "top3.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	settingItem = MenuItemImage::create("hall_btn_setting.png", "hall_btn_setting.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	toolkitItem = MenuItemImage::create("hall_btn_pack.png", "hall_btn_pack.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	placehole->setTag((int)ServiceCode::Menu_Service);
	settingItem->setTag((int)ServiceCode::Setting_Service);
	toolkitItem->setTag((int)ServiceCode::ToolKit_Service);
	placehole->setPosition(42, 362);
	settingItem->setPosition(42, 362);
	toolkitItem->setPosition(42, 362);
	auto menu = Menu::create(settingItem, toolkitItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	menuBg->addChild(placehole);
	menuBg->addChild(menu);
	menuBg->setVisible(false);
	moreMenu = menuBg;
}

void HallScene::showOrHideMoreMenu(bool show){

	if (show){
		moreMenu->setVisible(true);
		auto moveDown1 = MoveBy::create(0.15, Vec2(0, -90));
		auto moveDown2 = MoveBy::create(0.15, Vec2(0, -180));
		settingItem->runAction(moveDown1);
		toolkitItem->runAction(moveDown2);
	}else{
		auto moveup1 = MoveBy::create(0.15, Vec2(0, 90));
		auto moveup2 = MoveBy::create(0.15, Vec2(0, 180));
		settingItem->runAction(moveup1);
		toolkitItem->runAction(moveup2);
		scheduleOnce(CC_SCHEDULE_SELECTOR(HallScene::doHideMoreMenu), 0.15);
	}
}

void HallScene::doHideMoreMenu(float t){
	moreMenu->setVisible(false);
}

MIGU_NS_COCOS_END
