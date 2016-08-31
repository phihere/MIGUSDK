#include "HallContent.h"

#include <sstream>

MIGU_NS_COCOS

using namespace std;
using namespace tinyxml2;

string filenameWithoutExtension(const string& filename);

string filenameWithoutExtension(const string& filename) {
	auto pos = filename.find(".");
	return filename.substr(0, pos);
}

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
	string test = filenameWithoutExtension(resourceURL);

	resourcePURL = filenameWithoutExtension(resourceURL) + string("_p.png");
}



MIGU_NS_COCOS_END