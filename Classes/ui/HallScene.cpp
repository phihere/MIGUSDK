#include "HallScene.h"

#include <utility>

#include "../Login/LoginCenter.h"
#include "../Login/LoginSession.h"
#include "HallDialog.h"
//#include "../service/GameResourceManager.h"

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
	configureMoreMenu();
	startLogin();

	



// 	auto pListener1 = EventListenerKeyboard::create();
// 	pListener1->onKeyReleased = CC_CALLBACK_2(HallScene::onKeyReleased, this);
// 	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(pListener1, this);

// 	auto	listener = EventListenerTouchOneByOne::create();
// 	listener->onTouchBegan = CC_CALLBACK_2(HallScene::onTouchBegan, this);
// 	listener->setSwallowTouches(true);
// 	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
// 
// 	m_pListener = EventListenerKeyboard::create();
// 	m_pListener->onKeyReleased = CC_CALLBACK_2(HallScene::onTouchEnded, this);
// 	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_pListener, this);

	auto _touchListener = EventListenerTouchOneByOne::create();
	_touchListener->setSwallowTouches(true);
	_touchListener->onTouchBegan = CC_CALLBACK_2(HallScene::onTouchBegan, this);
	_touchListener->onTouchMoved = CC_CALLBACK_2(HallScene::onTouchMoved, this);
	_touchListener->onTouchEnded = CC_CALLBACK_2(HallScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

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
	//LoginCenter::getInstance().registerLoginCallback(loginCallBack);
	//LoginCenter::getInstance().registerLoginDataProvider(loginDataInfo);
	auto loginSession = LoginCenter::getInstance().newLoginSession(true);
	loginSession->login();
}

void HallScene::loginCallBack(int status, const std::string& loginInfo){
	if (status == 0){
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
	auto backSprite = Sprite::create("public/public_bg.jpg");
	CCLOG("%f",visibleSize.width);
	CCLOG("%f",visibleSize.height);
	
	backSprite->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	layer->addChild(backSprite);
}


void HallScene::configureSideButton(){
	ContentService contentService("XML/miscellContent.xml");
	vector<HallContent> items = contentService.loadContent();
	if (items.size() == 0){
		return;
	}
	auto sideMenu = Menu::create();
	for (HallContent item : items)
	{
		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourcePURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
		float anAnchorX = (item.getIndex() == 0 ? 0 : 1 );
		float positionX = (item.getIndex() == 0 ? 0 : visibleSize.width );
		menuItem->setAnchorPoint(Vec2(anAnchorX, 0.5));
		menuItem->setPosition(positionX , visibleSize.height / 2);
		menuItem->setTag(static_cast<int>(item.getServiceCode()));
		sideMenu->addChild(menuItem);
	}

	sideMenu->setPosition(Vec2::ZERO);
	layer->addChild(sideMenu);
}

void HallScene::uiCallback(Ref* sender){
	if (!eventDelegate) {
		return;
	}

	int code = ((Node*)sender) -> getTag();
	CCLOG("tag is %d", code);
	if (code == int(ServiceCode::Menu_Service)){
		showMoreMenu = !showMoreMenu;
		showOrHideMoreMenu(showMoreMenu);
		return;
	}
#if 0
	if (code == int(ServiceCode::Promotion1_Service)){
		if (GameResourceManager::getInstance().gameHasDownloaded(GameType::Migu_GuanDan)){
			CCLOG("game has downloaded lanuch");
			//eventDelegate->lanuchOtherGame(GameType::Migu_GuanDan, GameResourceManager::getInstance().getGameResourcePath(GameType::Migu_GuanDan));
			return;
		}
		if (GameResourceManager::getInstance().isDownloading()){
			return;
		}

		auto dialog = HallDialog::create();
		using namespace placeholders;
		dialog->setContent("Please download game, press comfirm if you want to play");
		dialog->showInLayer(layer);
	}
#endif
	eventDelegate->uiService((ServiceCode)code);
}

void HallScene::configureMainFrame(){
	ContentService contentService("XML/mainItem.xml");
	vector<HallContent> items = contentService.loadContent();

	auto mainMenu = Menu::create();
	for (HallContent item : items)
	{

		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourcePURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
		if (!item.getEventKey().empty()){ 
			
			auto label = Label::createWithSystemFont("0", "fonts/arial.ttf", 20,
                                          Size(200, 25), TextHAlignment::CENTER);
			label->setPosition(140, 24);
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
	auto headFrame = Sprite::create("hall/hall_frame_top.png");
	headFrame->setAnchorPoint(Vec2(0.5, 1));
	headFrame->setPosition(visibleSize.width / 2, visibleSize.height);
	layer->addChild(headFrame);

	auto playerPortrait = Sprite::create("hall/playerInfo_propBg.png");
	playerPortrait->setAnchorPoint(Vec2(0,1));
	playerPortrait->setPosition(Vec2(0, visibleSize.height));
	layer->addChild(playerPortrait);

	avatarSprite = Sprite::create("hall/lb_shape_headIcon.png");
	avatarSprite->setPosition(60,80);
	playerPortrait->addChild(avatarSprite);

	auto beanCount = Sprite::create("hall/hall_frame_bean.png");
	beanCount->setPosition(Vec2(250, 50));
	headFrame->addChild(beanCount);

	auto beanLabel = Label::createWithSystemFont("0", "Arial", 22,
                                          Size(200, 20), TextHAlignment::CENTER);
	beanLabel->setPosition(120, 25);
	beanCount->addChild(beanLabel);
	addEventListener(Bean_Count, beanLabel);

	nicknameLabel  = Label::createWithSystemFont("", "Arial", 23,
                                          Size(200, 30), TextHAlignment::CENTER);
	nicknameLabel->setPosition(220, 92);
	headFrame->addChild(nicknameLabel);

#if 0
	avatarItem = MenuItem::create(CC_CALLBACK_1(HallScene::uiCallback, this));
	avatarItem->setContentSize(Size(240, 240));
	avatarItem->setTag(1002);
	auto userInfoMenu =  Menu::create(avatarItem, nullptr);
	userInfoMenu->setPosition(Vec2::ZERO);
	userInfoMenu->setPosition(Vec2(58, 60));
	portraitCircle->addChild(userInfoMenu);
#endif
	

	auto addBeanItem = MenuItemImage::create("hall/hall_btn_add.png", "hall/hall_btn_add_p.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	auto addBeanMenu = Menu::create(addBeanItem, nullptr);
	addBeanMenu->setPosition(Vec2::ZERO);
	addBeanItem->setPosition(Vec2(230, 25));
	addBeanItem->setTag(static_cast<int>(ServiceCode::Add_Bean_Service));
	beanCount->addChild(addBeanMenu);
	  

	auto gameTitleBack = Sprite::create("hall/hall_titile_bg.png");
	gameTitleBack->setAnchorPoint(Vec2(0.5, 1));
	gameTitleBack->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	layer->addChild(gameTitleBack);

	auto gameTitle = Sprite::create("hall/game_title.png");
	gameTitle->setAnchorPoint(Vec2(0.5, 1));
	gameTitle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	layer->addChild(gameTitle);

	ContentService contentService("XML/topItem.xml");
	vector<HallContent> items = contentService.loadContent();

	auto topMenu = Menu::create();
	for (HallContent item : items)
	{
		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourcePURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
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
	auto bottomFrame = Sprite::create("hall/hall_frame_bottom.png");
	bottomFrame->setAnchorPoint(Vec2(0.5, 0));
	bottomFrame->setPosition(Vec2(visibleSize.width / 2, 0));
	layer->addChild(bottomFrame);

	ContentService contentService("XML/bottomItem.xml");
	vector<HallContent> items = contentService.loadContent();

	auto bottomMenu = Menu::create();
	for (HallContent item : items)
	{
		auto menuItem = MenuItemImage::create(item.getResourceURL(), item.getResourcePURL(),CC_CALLBACK_1(HallScene::uiCallback, this));
		addRedPoingListener(item.getName(), menuItem);
		float positionX = item.getIndex() * 150 + 80; 
		menuItem->setPosition(positionX, 60);
		menuItem->setTag(static_cast<int>(item.getServiceCode()));
		bottomMenu->addChild(menuItem);
	}

	bottomMenu->setPosition(Vec2::ZERO);
	bottomFrame->addChild(bottomMenu);

	auto quickStart = MenuItemImage::create("hall/quick_start.png", "hall/quick_start_p.png", CC_CALLBACK_1(HallScene::uiCallback, this)); 
	quickStart->setAnchorPoint(Vec2(1, 0));
	quickStart->setPosition(visibleSize.width, 0);
	quickStart->setTag(static_cast<int>(ServiceCode::Quick_Start_Service));
	bottomMenu->addChild(quickStart);
	//addChild(quickStart);
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
		auto redPoint = Sprite::create("hall/redPoint.png");
		redPoint->setTag(Red_Point_Tag);
		redPoint->setPosition(rect.size.width - 5, rect.size.height - 5);
		item->addChild(redPoint);
	}else{
		item->removeChildByTag(Red_Point_Tag);
	}
	
}

void HallScene::changeAvatar(const std::string &path){
	avatarSprite->removeAllChildren();

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
	clipNode->setPosition(60, 60);

	auto avatarMenuItem = MenuItem::create(CC_CALLBACK_1(HallScene::uiCallback, this));
	//auto avatarMenuItem = MenuItemImage::create(path, path, CC_CALLBACK_1(HallScene::uiCallback, this));
	avatarMenuItem->setContentSize(Size(120, 120));
	avatarMenuItem->setPosition(Vec2(0, 0));
	avatarMenuItem->setTag((int)ServiceCode::Tap_User_Info);
	auto userInfoMenu = Menu::create(avatarMenuItem, nullptr);
	userInfoMenu->setPosition(Vec2::ZERO);
	clipNode->addChild(userInfoMenu);

	avatarSprite->addChild(clipNode);

}


void HallScene::configureMoreMenu(){
	auto menuBg = Sprite::create("hall/more_btn_bg.png");
	menuBg->setPosition(1208, -100);
	hallHead->addChild(menuBg);

	auto placehole = MenuItemImage::create("hall/top3.png", "hall/top3.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	settingItem = MenuItemImage::create("hall/hall_btn_setting.png", "hall/hall_btn_setting.png", CC_CALLBACK_1(HallScene::uiCallback, this));
	toolkitItem = MenuItemImage::create("hall/hall_btn_pack.png", "hall/hall_btn_pack.png", CC_CALLBACK_1(HallScene::uiCallback, this));
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

bool HallScene::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	return true;
}

void HallScene::onTouchMoved(Touch *pTouch, Event *pEvent)
{

}
void HallScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	Vec2 pos = touch->getLocation();
	if (pos.x < 200 && pos.y > 800){
		if (eventDelegate) {
			eventDelegate->uiService(ServiceCode::Tap_User_Info);
		}
	}
}


MIGU_NS_COCOS_END
