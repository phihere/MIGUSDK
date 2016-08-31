#include "HallDialog.h"

#include "ui/CocosGUI.h"
#include "../service/GameResourceManager.h"

MIGU_NS_COCOS

using namespace cocos2d;
using namespace cocos2d::ui;

bool HallDialog::init() {
	if (!Node::init()) {
		return false;
	}

	backSprite = Sprite::create("hall/dialog_bg.png");
	addChild(backSprite);

	auto menu = Menu::create();
	auto comfirmItem = MenuItemImage::create("btn_green.png", "btn_green.png", CC_CALLBACK_1(HallDialog::comfirmBtnCallback, this));
	auto negativeItem = MenuItemImage::create("btn_red.png", "btn_red.png", CC_CALLBACK_1(HallDialog::negativeBtnCallback, this));
	menu->setPosition(Vec2::ZERO);
	comfirmItem->setPosition(470, 180);
	negativeItem->setPosition(830, 180);
	menu->addChild(comfirmItem);
	menu->addChild(negativeItem);

	backSprite->addChild(menu);

	auto comfirmLabel = Label::createWithSystemFont("comfirm", "fonts/arial.ttf", 30,
		Size(200, 40), TextHAlignment::CENTER);
	comfirmLabel->setPosition(110, 40);
	comfirmItem->addChild(comfirmLabel);

	auto negativeLabel = Label::createWithSystemFont("cancel", "fonts/arial.ttf", 30,
		Size(200, 40), TextHAlignment::CENTER);
	negativeLabel->setPosition(110, 40);
	negativeItem->addChild(negativeLabel);

	return true;
}

void HallDialog::comfirmBtnCallback(cocos2d::Ref* sender) {
	removeFromParent();

	GameResourceManager::getInstance().downloadGame(GameType::Migu_GuanDan, nullptr);

}

void HallDialog::negativeBtnCallback(cocos2d::Ref* sender) {
	removeFromParent();
	
}


void HallDialog::showInLayer(cocos2d::Layer *layer) {
	Size size = layer->getContentSize();
	setPosition(size.width * 0.5, size.height * 0.5);
	layer->addChild(this);
}

void HallDialog::setContent(const std::string& content) {
	auto negativeLabel = Label::createWithSystemFont(content.c_str(), "fonts/arial.ttf", 30,
		Size(600, 300), TextHAlignment::CENTER);
	negativeLabel->setPosition(620, 240);
	backSprite->addChild(negativeLabel);
}


MIGU_NS_COCOS_END