#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUIText);

GUIText::GUIText() : renderable(),
	styleSheet(),
	styleName()
{	}

GUIText::~GUIText()
{	}

void GUIText::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIText::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIText::OnWindowEvent);

	resourceManager->Get(styleSheet, "style/styles.cfo");

	renderable.style = &styleSheet->get(styleName);

}

void GUIText::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);	

	styleName = "textDefault";

	node->getValueString("style", styleName);

	renderable.size = m_Size;

	setDisabled(true);	//Do not react to events
}

bool GUIText::OnWindowEvent(WindowEvent*const event)
{
	if (event->eventType == WindowEvent::WINDOW_CREATED) {

		renderable.update();

		renderable.setTextWithWrapping(m_Title);

	}
	return 1;
}

bool GUIText::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;


	return 1;
}

void GUIText::PreRender(Renderer* r) {
	GUIEntity::PreRender(r);
}

void GUIText::Render(Renderer* rnd){

	renderable.Draw();
}

void GUIText::PostRender() {
	GUIEntity::PostRender();
}

#endif