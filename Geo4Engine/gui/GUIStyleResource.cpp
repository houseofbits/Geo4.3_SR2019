#include "../Geo4.h"

#ifndef NO_OPENGL

GUIStyleResource GUIStyleResource::m_DefaultInstance;

GUIStyle GUIStyleResource::defaultStyle;

GUIStyleResource::GUIStyleResource() : styles()
{
}


GUIStyleResource::~GUIStyleResource()
{
}

bool GUIStyleResource::Load(std::string filename) {

	CFODocument doc;
	if (!doc.ReadFile(filename))return false;

	CFONode* root = doc.GetFirstChild("GUIStyle");
	if (root) {
		CFONode* object = root->GetFirstChild();
		while (object) {
			string classname = object->GetName();

			GUIStyle* style = new GUIStyle();
			style->Deserialize(object, resourceManager);

			styles.push_back(StyleEntry(classname, style));

			object = object->GetNextNode();
		}
	}
	return true;
}

GUIStyle& GUIStyleResource::get(string name) {
	for (unsigned int i = 0; i < styles.size(); i++){
		if (styles[i].first == name)return (*styles[i].second);
	}
	return defaultStyle;
}

#endif