#ifndef __CONTENT_SERVICE_H
#define __CONTENT_SERVICE_H

#include <vector>
#include <string>

#include "tinyxml2\tinyxml2.h"

#include "../PublicDef.h"

MIGU_NS_COCOS

class HallContent;

class ContentService{
public:
	ContentService(const std::string& url, bool inLocal = true):url(url),inLocal(inLocal), readFail(false){}
	~ContentService();

	std::vector<HallContent> loadContent();
private:

	std::string getUrl() const {return url;}
	bool isInlocal() const {return inLocal;}
	tinyxml2::XMLElement* getDocRoot() const {return doc->RootElement();}

	void loadRemoteContent();
	void loadLocalContent();

	std::string url;
	bool inLocal;
	bool readFail;
	tinyxml2::XMLDocument* doc;
};

MIGU_NS_COCOS_END


#endif