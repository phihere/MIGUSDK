#ifndef __HALL_EVENT_DELEGATE_H
#define __HALL_EVENT_DELEGATE_H

#include <string>
#include "../PublicDef.h"

MIGU_NS_COCOS

enum class ServiceCode{
	None_Service = -1,
	Chat_Service = 0, //����
	Single_Game_Service = 1, //����
	Promotion1_Service = 2, //�ƹ�λ1
	Promotion2_Service = 3, //�ƹ�λ2
	Promotion3_Service = 4, //�ƹ�λ3
	Partner_Service = 5, //����
	Activity_Service = 6, //�
	Exchange_Service = 7, //�һ�
	Shop_Service = 8, //�̳�
	First_Charge_Service = 9, //�׳�
	Gift_Service = 10, //�Ͷ�
	Message_Service = 11, //��Ϣ����
	Menu_Service = 12, //�����˵�
	LaiZi_Game_Service = 13, //���Ӷ�����
	HuanLe_Game_Service = 14, //���ֶ�����
	Match_Game_Service = 15, //����������
	MiGu_Game_Service = 16, //�乾Լս
	
	Quick_Start_Service = 1000, //���ٿ�ʼ
	Add_Bean_Service = 1001, //��ֵ
	Tap_User_Info = 1002,   //����û�ͷ��
	Setting_Service = 1003,  //����
	ToolKit_Service = 1004, //������
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