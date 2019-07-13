#include "Geo4.h"

#ifndef NO_OPENGL

#include <crtdbg.h>
#include <tchar.h>
#include <windows.h>
#include <sys/types.h>

using namespace std;

CLASS_DECLARATION(Interface);

Interface::Interface(void) : Entity(), 
	wings(0), 
	selectedKeyIndex(-1),
	resourceFileName()
{	}

Interface::~Interface(void)
{	}

void Interface::Initialise(EventManager*const event_manager, ResourceManager*const resourceManager)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &Interface::OnWindowEvent);
	event_manager->RegisterEventReceiver(this, &Interface::OnGUIEvent);
	try {
		wings = instanceOf<GUIWings>("wings");
		timeline = instanceOf<GUISlider>("timeline");
		hardware = instanceOf<BasicSerial>("hardware");
		playSpeed = (float)atof(instanceOf<GUITextInput>("playSpeed")->m_Title.c_str());
	}
	catch (int e) {
		cout << "Exception (Interface) " << e << endl;
	}
}

void Interface::Deserialize(CFONode* node)
{	
	Entity::Deserialize(node);
}

bool Interface::OnWindowEvent(WindowEvent*const event){

	if (playing && timeline) {

		timeline->value += event->frametime * playSpeed;

		if (timeline->value > timeline->valueRange.y)timeline->value = timeline->valueRange.x;

		if (wings) {
			WingsKeyframe kf;
			if (getInterpolatedKeyframe(timeline->value, kf)) {
				wings->applyKeyframe(kf);
				hardware->updateLightData(kf);
			}
		}

		instanceOf<GUITextInput>("timelineValue")->setTitle(Utils::FloatToString(timeline->value, 4));
	}

	return 1;
}

bool Interface::OnGUIEvent(GUIEvent*const event) {

	try {

		if (event->type == GUIEvent::SLIDER_CHANGED) {

			if (event->m_Sender->getName() == "timeline") {
				playing = false;
				if (wings) {
					WingsKeyframe kf;
					if (getInterpolatedKeyframe(timeline->value, kf)) {
						wings->applyKeyframe(kf);
						hardware->updateLightData(kf);
					}
				}
				updateKeyIndex();
				instanceOf<GUITextInput>("timelineValue")->setTitle(Utils::FloatToString(event->m_Sender->instanceOf<GUISlider>()->value, 4));
			}

		}
		if (event->type == GUIEvent::TEXT_INPUT_CHANGED) {
			float value = 0;
			if (event->m_Sender->getHashName() == hashStr("timelineValue")){
				value = (float)atof(event->m_Sender->instanceOf<GUITextInput>()->m_Title.c_str());				
				instanceOf<GUISlider>("timeline")->setValue(value);
				WingsKeyframe kf;
				if (getInterpolatedKeyframe(timeline->value, kf)) {
					wings->applyKeyframe(kf);
					hardware->updateLightData(kf);
				}
			}
			if (event->m_Sender->getHashName() == hashStr("playSpeed")) {
				playSpeed = (float)atof(event->m_Sender->instanceOf<GUITextInput>()->m_Title.c_str());
			}
			if (event->m_Sender->getHashName() == hashStr("selectedKeyIndex")) {
				int idx = atoi(event->m_Sender->instanceOf<GUITextInput>()->m_Title.c_str());
				if (idx >= 0 && idx < keyframes.size()) {
					timeline->value = keyframes[idx]->t;
					instanceOf<GUITextInput>("timelineValue")->setTitle(Utils::FloatToString(timeline->value, 4));
					selectedKeyIndex = idx;
					wings->applyKeyframe(*keyframes[idx]);
					hardware->updateLightData(*keyframes[idx]);
				}
			}
		}
		if (event->type == GUIEvent::BUTTON_PRESSED) {
			
			GUIListItem *item;



			switch (event->m_Sender->getHashName()) {

			case hashStr("colorPicker"):
				if (wings)wings->setColorToSelected(event->m_Sender->instanceOf<GUIColorPicker>()->selectedColor);
				break;
			case hashStr("colorBlack"):
				if (wings)wings->setColorToSelected(Vector3(0, 0, 0));
				break;
			case hashStr("colorWhite"):
				if (wings)wings->setColorToSelected(Vector3(1, 1, 1));
				break;

			case hashStr("selAll"):
				if (wings)wings->selectAll();
				break;
			case hashStr("selWing"):
				if (wings)wings->selectWing();
				break;
			case hashStr("selRing"):
				if (wings)wings->selectRing();
				break;
			case hashStr("selRingOdd"):
				if (wings)wings->selectRingOdd();
				break;

			case hashStr("keySet"):
				if (wings && timeline) {
					playing = false;
					if (selectedKeyIndex >= 0) {
						WingsKeyframe kf(wings->createKeyframe(timeline->value));
						keyframes[selectedKeyIndex]->update(kf.colors);
					}
					else {
						keyframes.push_back(new WingsKeyframe(wings->createKeyframe(timeline->value)));
						sortKeyframesByTime();
						updateTimeline();
						updateKeyIndex();
					}
				}
				break;
			case hashStr("keyDelete"):
				if (wings && timeline && selectedKeyIndex >= 0) {
					delete keyframes[selectedKeyIndex];
					keyframes.erase(keyframes.begin() + selectedKeyIndex);
					sortKeyframesByTime();
					updateTimeline();
					updateKeyIndex();
				}
				break;
			case hashStr("keyNext"):
				if (wings && timeline) {
					WingsKeyframe kf;
					int i = getNextKeyframe(timeline->value, kf);
					if (i >= 0) {
						wings->applyKeyframe(kf);
						hardware->updateLightData(kf);
						timeline->value = kf.t;
						selectedKeyIndex = i;
					}
					updateTimeline();
					updateKeyIndex();
				}
				break;
			case hashStr("keyPrev"):
				if (wings && timeline) {
					WingsKeyframe kf;
					int i = getPreviousKeyframe(timeline->value, kf);
					if (i >= 0) {
						wings->applyKeyframe(kf);
						hardware->updateLightData(kf);
						timeline->value = kf.t;
					}
					updateTimeline();
					updateKeyIndex();
				}
				break;
			case hashStr("playStop"):
				playing = !playing;
				break;
			case hashStr("keyEdit"):
				if (selectedKeyIndex >= 0) {
					instanceOf<GUIWindow>("windowEditKey")->setTitle("Edit key #" + Utils::IntToString(selectedKeyIndex));
					instanceOf<GUIWindow>("windowEditKey")->setVisible(true);
					instanceOf<GUITextInput>("keyTime")->setTitle(Utils::FloatToString(keyframes[selectedKeyIndex]->t, 4));
				}
				break;
			case hashStr("buttonApplyEditKey"):
				instanceOf<GUIWindow>("windowEditKey")->setVisible(false);
				keyframes[selectedKeyIndex]->t = (float)atof(instanceOf<GUITextInput>("keyTime")->m_Title.c_str());
				timeline->value = keyframes[selectedKeyIndex]->t;
				sortKeyframesByTime();
				updateTimeline();
				updateKeyIndex();
				break;
			case hashStr("timelineSettings"):
				instanceOf<GUITextInput>("timelineTStart")->setTitle(Utils::FloatToString(timeline->valueRange.x, 4));
				instanceOf<GUITextInput>("timelineTEnd")->setTitle(Utils::FloatToString(timeline->valueRange.y, 4));
				instanceOf<GUIWindow>("windowEditTimeline")->setVisible(true);				
				break;
			case hashStr("buttonCancelEditT"):
				instanceOf<GUIWindow>("windowEditTimeline")->setVisible(false);
				break;
			case hashStr("buttonResizeEditT"):
				instanceOf<GUIWindow>("windowEditTimeline")->setVisible(false);
				timeline->valueRange.x = (float)atof(instanceOf<GUITextInput>("timelineTStart")->m_Title.c_str());
				timeline->valueRange.y = (float)atof(instanceOf<GUITextInput>("timelineTEnd")->m_Title.c_str());
				updateTimeline();
				updateKeyIndex();
				break;			
			case hashStr("buttonScaleEditT"):
				instanceOf<GUIWindow>("windowEditTimeline")->setVisible(false);
				scaleTimeline((float)atof(instanceOf<GUITextInput>("timelineTStart")->m_Title.c_str()),
					(float)atof(instanceOf<GUITextInput>("timelineTEnd")->m_Title.c_str()));
				updateTimeline();
				updateKeyIndex();
				break;

			case hashStr("buttonResourcesWindow"):
				instanceOf<GUIButton>("buttonLoadResources")->setVisible(true);
				instanceOf<GUIButton>("buttonInsertResources")->setVisible(false);
				instanceOf<GUIButton>("buttonInsertExpandResources")->setVisible(false);
				instanceOf<GUIWindow>("windowResources")->setVisible(true);
				populateResourcesWindow();
				break;
			case hashStr("buttonInsertResourcesWindow"):
				instanceOf<GUIButton>("buttonLoadResources")->setVisible(false);
				instanceOf<GUIButton>("buttonInsertResources")->setVisible(true);
				instanceOf<GUIButton>("buttonInsertExpandResources")->setVisible(true);
				instanceOf<GUIWindow>("windowResources")->setVisible(true);
				populateResourcesWindow();
				break;
			case hashStr("buttonCloseResources"):
				instanceOf<GUIWindow>("windowResources")->setVisible(false);
				break;
			case hashStr("buttonLoadResources"):
				instanceOf<GUIWindow>("windowResources")->setVisible(false);
				item = instanceOf<GUIList>("resourcesList")->getSelectedItem();
				if (item) {
					resourceFileName = "savedResources\\" + item->getName();
					WingKeyframesResource res;
					res.ReadFromFile(resourceFileName, keyframes, timeline->valueRange.x, timeline->valueRange.y);
					sortKeyframesByTime();
					updateTimeline();
					updateKeyIndex();
					instanceOf<GUIText>("textResourceFileName")->setTitle(resourceFileName);
				}
				break;
			case hashStr("buttonInsertResources"):
				instanceOf<GUIWindow>("windowResources")->setVisible(false);
				item = instanceOf<GUIList>("resourcesList")->getSelectedItem();
				if (item) {
					WingKeyframesResource res;
					res.InsertFromFile("savedResources\\" + item->getName(), keyframes, timeline->valueRange.x, timeline->valueRange.y, timeline->value, false);
					sortKeyframesByTime();
					updateTimeline();
					updateKeyIndex();
				}
				break;
			case hashStr("buttonInsertExpandResources"):
				instanceOf<GUIWindow>("windowResources")->setVisible(false);
				item = instanceOf<GUIList>("resourcesList")->getSelectedItem();
				if (item) {
					WingKeyframesResource res;
					res.InsertFromFile("savedResources\\" + item->getName(), keyframes, timeline->valueRange.x, timeline->valueRange.y, timeline->value, true);
					sortKeyframesByTime();
					updateTimeline();
					updateKeyIndex();
				}
				break;
			case hashStr("buttonNewResource"):
				resourceFileName = "";
				instanceOf<GUIText>("textResourceFileName")->setTitle("* New resource");
				keyframes.clear(); //!!!!!!!!!!!!!!!!!!!!!!!TODO memory leak
				sortKeyframesByTime();
				updateTimeline();
				updateKeyIndex();
				break;
			case hashStr("saveData"):
				if (resourceFileName.empty()) {
					instanceOf<GUIWindow>("windowSaveNewResource")->setVisible(true);
				}
				else{
					WingKeyframesResource res;
					res.SaveToFile(resourceFileName, keyframes, timeline->valueRange.x, timeline->valueRange.y);
				}
				break;
			case hashStr("buttonSaveNewResource"):
				if (!instanceOf<GUITextInput>("textInputResourcetName")->m_Title.empty()) {
					resourceFileName = "savedResources\\" + instanceOf<GUITextInput>("textInputResourcetName")->m_Title;
					WingKeyframesResource res;
					res.SaveToFile(resourceFileName, keyframes, timeline->valueRange.x, timeline->valueRange.y);
					instanceOf<GUIText>("textResourceFileName")->setTitle(resourceFileName);
					instanceOf<GUIWindow>("windowSaveNewResource")->setVisible(false);
				}
				break;
			case hashStr("buttonCancelNewResource"):
				instanceOf<GUIWindow>("windowSaveNewResource")->setVisible(false);
				break;

			case hashStr("buttonExpandWindow"):
				instanceOf<GUIWindow>("windowExpandTimeline")->setVisible(true);
				break;
			case hashStr("buttonCancelExpand"):
				instanceOf<GUIWindow>("windowExpandTimeline")->setVisible(false);
				break;
			case hashStr("buttonExpand"):
				instanceOf<GUIWindow>("windowExpandTimeline")->setVisible(false);
				expandTimeline((float)atof(instanceOf<GUITextInput>("timelineExpandLeftInput")->m_Title.c_str()),
					(float)atof(instanceOf<GUITextInput>("timelineExpandRightInput")->m_Title.c_str()));
				sortKeyframesByTime();
				updateTimeline();
				updateKeyIndex();
				break;

			};
		}
	}
	catch (int e) {
		cout << "Exception (Interface) " << e << endl;
	}
	return 1;
}

bool compare_keyframes(WingsKeyframe* first, WingsKeyframe* second)
{
	return (first->t < second->t);
}

void Interface::sortKeyframesByTime()
{
	sort(keyframes.begin(), keyframes.end(), compare_keyframes);
}

bool Interface::getInterpolatedKeyframe(float t, WingsKeyframe& keyframe)
{
	if (keyframes.size() < 2)return false;

	for (int i = 0; i < keyframes.size() - 1; i++) {
		if(keyframes[i]->t < t && keyframes[i+1]->t >= t) {
			
			float dt = (t - keyframes[i]->t) / (keyframes[i + 1]->t - keyframes[i]->t);

			keyframe = keyframes[i]->getInterpolated(1.0f - dt, *keyframes[i + 1]);

			return true;
		}
	}
	return false;
}

int Interface::getNextKeyframe(float t, WingsKeyframe& keyframe)
{
	if (keyframes.empty())return -1;

	for (int i = 0; i < keyframes.size(); i++) {
		if (keyframes[i]->t > t) {
			keyframe = *keyframes[i];
			return i;
		}
	}
	return -1;
}

int Interface::getPreviousKeyframe(float t, WingsKeyframe& keyframe)
{
	if (keyframes.size() < 2)return -1;

	for (int i = 0; i < keyframes.size()-1; i++) {
		if (keyframes[i+1]->t >= t) {
			keyframe = *keyframes[i];
			return i;
		}
	}
	return -1;
}

void Interface::updateTimeline()
{
	if (timeline) {
		timeline->referencePoints.clear();
		for (int i = 0; i < keyframes.size(); i++)
			timeline->referencePoints.push_back(keyframes[i]->t);
	}
	instanceOf<GUITextInput>("timelineValue")->setTitle(Utils::FloatToString(timeline->value, 4));
}

void Interface::expandTimeline(float left, float right) {
	if (timeline) {
		//cout << left << ", " << right << endl;
		for (int i = 0; i < keyframes.size(); i++) {
			if (keyframes[i]->t < timeline->value) {
				keyframes[i]->t += left;
			}
			else if(keyframes[i]->t > timeline->value){
				keyframes[i]->t += right;
			}
		}
		timeline->valueRange.x += left;
		timeline->valueRange.y += right;
	}
}

void Interface::updateKeyIndex()
{
	selectedKeyIndex = -1;
	if (timeline) {
		for (int i = 0; i < keyframes.size(); i++) {
			if (keyframes[i]->t == timeline->value) {
				selectedKeyIndex = i;
			}
		}
	}
	
	try {
		if (selectedKeyIndex >= 0) {
			instanceOf<GUITextInput>("selectedKeyIndex")->setTitle(Utils::IntToString(selectedKeyIndex));
		}
		else {
			instanceOf<GUITextInput>("selectedKeyIndex")->setTitle("");
		}
	}
	catch (int e) {
		cout << "Exception (Interface::updateKeyIndex) " << e << endl;
	}
}


void Interface::scaleTimeline(float tmin, float tmax)
{
	Vector2 tm = timeline->valueRange;

	float l = tm.y - tm.x;

	float newl = tmax - tmin;

	float scale = newl / l;

	for (unsigned int i = 0; i < keyframes.size(); i++)
	{
		float v = (keyframes[i]->t - tm.x) * scale;

		keyframes[i]->t = v + tmin;
	}
	timeline->valueRange.x = tmin;
	timeline->valueRange.y = tmax;
}

void Interface::populateResourcesWindow()
{	
	//cout << "Interface::populateResourcesWindow" << endl;

	GUIList* list = instanceOf<GUIList>("resourcesList");

	list->clearItems();

	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	wstring ws(buffer);
	string str(ws.begin(), ws.end());
	unsigned int pos = (unsigned int)str.find_last_of("\\/");
	str = str.substr(0, pos) + "\\savedResources\\*.bin";

	WIN32_FIND_DATA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	std::wstring ws2;
	ws2.assign(str.begin(), str.end());

	hFind = FindFirstFile(&ws2[0], &ffd);

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){		}
		else{
			wstring ws(ffd.cFileName);
			string str(ws.begin(), ws.end());
			//cout << str << endl;
			list->addItem(str, str);
		}
	} while (FindNextFile(hFind, &ffd) != 0);
}



#endif