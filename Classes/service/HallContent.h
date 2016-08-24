#ifndef __HALL_CONTENT_H
#define __HALL_CONTENT_H

#include <string>
#include "tinyxml2\tinyxml2.h"
#include "..\PublicDef.h"
#include "..\ui\HallEventDelegate.h"

MIGU_NS_COCOS

class HallContent {
public:
	explicit HallContent(const tinyxml2::XMLElement *const element);

	std::string getResourceURL() const {return resourceURL;}
	std::string getName() const {return name;}
	std::string getEventKey() const {return eventKey;}
	ServiceCode getServiceCode() const {return serviceCode;}
	int getIndex() const {return index;}
	void setIndex(int vectorIndex) {index = vectorIndex;}
private:
	std::string resourceURL;
	std::string name;
	std::string eventKey;
	ServiceCode serviceCode;
	int index;
};

MIGU_NS_COCOS_END

#endif