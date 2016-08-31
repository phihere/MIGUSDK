#ifndef __HALL_DIALOG_H
#define __HALL_DIALOG_H

#include <functional>
#include <string>

#include "cocos2d.h"

#include "../PublicDef.h"

MIGU_NS_COCOS

class HallDialog : cocos2d::Node {
public:
	CREATE_FUNC(HallDialog);

	virtual bool init();

	void showInLayer(cocos2d::Layer *layer);

	void setContent(const std::string& content);

private:
	void comfirmBtnCallback(cocos2d::Ref* sender);
	void negativeBtnCallback(cocos2d::Ref* sender);

	cocos2d::Sprite *backSprite;
};

MIGU_NS_COCOS_END


#endif // !__HALL_DIALOG_H
