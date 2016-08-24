#include "HallContent.h"

#include <sstream>

MIGU_NS_COCOS

using namespace std;
using namespace tinyxml2;

HallContent::HallContent(const tinyxml2::XMLElement *const element):name(""), resourceURL(""), eventKey(""), serviceCode(ServiceCode::None_Service){
	name = element->GetText();
	resourceURL = element->Attribute("resourceUL");
	if (element->FindAttribute("listener")){
		eventKey = element->Attribute("listener");
	}
	auto serviceName = element->Attribute("service");
	stringstream ss(serviceName);
	int code = 0;
	ss >> code;
	serviceCode = (ServiceCode)code;
}

MIGU_NS_COCOS_END