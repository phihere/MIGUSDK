#include "ContentService.h"
#include "HallContent.h"
#include "cocos2d.h"
#include <iostream>

MIGU_NS_COCOS

using namespace tinyxml2;
using namespace std;

ContentService::~ContentService(){
	delete doc;
}

void ContentService::loadRemoteContent(){
}

void ContentService::loadLocalContent(){
	doc = new tinyxml2::XMLDocument();
	std::string content = cocos2d::FileUtils::getInstance()->getStringFromFile(url);
    //XMLError errorID = doc->LoadFile(url.c_str());
	XMLError errorID = doc->Parse(content.c_str());
	if (errorID){
		readFail = true;
		return;
	}
	
}

vector<HallContent> ContentService::loadContent(){
	inLocal?loadLocalContent():loadRemoteContent();
	auto docRoot = doc->RootElement();

	vector<HallContent> reslut;

	if (readFail || doc->NoChildren()){
		return reslut;
	}

	int index = 0;
	auto element = docRoot->FirstChildElement();
	if (element){
		HallContent content(element);
		content.setIndex(index);
		reslut.push_back(content);
		while (element->NextSiblingElement())
		{
			++index;
			element = element->NextSiblingElement();
			HallContent content(element);
			content.setIndex(index);
			reslut.push_back(content);
		}
	}

	return reslut;
}

MIGU_NS_COCOS_END
