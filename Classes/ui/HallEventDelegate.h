#ifndef __HALL_EVENT_DELEGATE_H
#define __HALL_EVENT_DELEGATE_H

#include <string>
#include "../PublicDef.h"

MIGU_NS_COCOS

enum class ServiceCode{
	None_Service = -1,
	Chat_Service = 0, //聊天
	Single_Game_Service = 1, //单机
	Promotion1_Service = 2, //推广位1
	Promotion2_Service = 3, //推广位2
	Promotion3_Service = 4, //推广位3
	Partner_Service = 5, //牌友
	Activity_Service = 6, //活动
	Exchange_Service = 7, //兑换
	Shop_Service = 8, //商城
	First_Charge_Service = 9, //首充
	Gift_Service = 10, //送豆
	Message_Service = 11, //信息中心
	Menu_Service = 12, //下拉菜单
	LaiZi_Game_Service = 13, //赖子斗地主
	HuanLe_Game_Service = 14, //欢乐斗地主
	Match_Game_Service = 15, //比赛斗地主
	MiGu_Game_Service = 16, //咪咕约战
	
	Quick_Start_Service = 1000, //快速开始
	Add_Bean_Service = 1001, //充值
	Tap_User_Info = 1002,   //点击用户头像
	Setting_Service = 1003,  //设置
	ToolKit_Service = 1004, //道具箱
};

enum class GameType {
	Migu_GuanDan = 0
};

class HallEventDelegate{
public:
	virtual ~HallEventDelegate(){}
	virtual void uiService(ServiceCode code) = 0;
	virtual void onEnter() = 0;
	virtual void onExit() = 0;
	virtual void lanuchOtherGame(GameType type, const std::string& resourcePath) = 0;

};

MIGU_NS_COCOS_END

#endif