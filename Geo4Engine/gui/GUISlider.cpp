#include "../Geo4.h"

#ifndef NO_OPENGL

#include "../StringUtils.h"
#include <algorithm>

using namespace std;

CLASS_DECLARATION(GUISlider);

GUISlider::GUISlider(void) : GUIEntity(), EventHandler(), 
	renderable(),
	renderableControlPin(),
	styleSheet(),
	styleName(),
	styleNameControlPin(),
	value(100),
	valueRange(0, 255),
	pinSize(20, 20),
	referencePoints()

{ }

GUISlider::~GUISlider(void)
{ }

void GUISlider::Initialise(EventManager* const eventManager, ResourceManager*const resourceManager)
{	
	resourceManager->Get(styleSheet, "style/styles.cfo");

	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUISlider::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUISlider::OnWindowEvent);

	renderable.style = &styleSheet->get(styleName);
	renderableControlPin.style = &styleSheet->get(styleNameControlPin);
}

void GUISlider::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	node->getValueVector2("range", valueRange);
	node->getValueFloat("value", value);

	if (value > valueRange.y)value = valueRange.y;
	if (value < valueRange.x)value = valueRange.x;

	node->getValueVector2("pinSize", pinSize);

	styleName = "sliderDefault";
	styleNameControlPin = "sliderDefaultPin";

	node->getValueString("style", styleName);
	node->getValueString("styleControlPin", styleNameControlPin);

	renderable.size = m_Size;
	renderableControlPin.size = pinSize;
}

bool GUISlider::OnWindowEvent(WindowEvent*const event)
{
	//Set default values on window creation
	if (event->eventType == WindowEvent::WINDOW_CREATED) {

		renderable.update();
		renderableControlPin.update();

		SendEvent(new GUIEvent(GUIEvent::SLIDER_CHANGED));
	}
	return 1;
}

bool GUISlider::OnGUIInputEvent(GUIInputEvent*const event)
{	
	if(isVisible()==0)return 1;

	Vector2 worldPos = getWorldSpace(m_LocalPos);
	Vector2 setLocalPos = m_LocalPos;
	switch (event->type) {
	case GUIInputEvent::EventType::DRAG:
		OnChangeValue(event->mousePosition);
		break;
	case GUIInputEvent::EventType::MOUSEMOVE:
		break;
	case GUIInputEvent::EventType::MOUSEDOWN:
		OnChangeValue(event->mousePosition);
		break;
	case GUIInputEvent::EventType::MOUSEUP:		
		break;
	case GUIInputEvent::EventType::MOUSEENTER:
		break;
	case GUIInputEvent::EventType::MOUSELEAVE:
		break;
	};
	return 1;
}

void GUISlider::setValue(float val) {

	if (val < valueRange.x || val > valueRange.y)return;
	
	value = val;
}

bool GUISlider::IsOver(Vector2 p)
{
	return Clip(Vector2(p));
}

bool GUISlider::OnChangeValue(Vector2 mouse_pos)
{	
	Vector2 worldPos = getWorldSpace(m_LocalPos);
	float rel_mx = worldPos.x - mouse_pos.x;
	if(rel_mx < (-m_Size.x * 0.5f) || rel_mx > (m_Size.x * 0.5f))return 0;
	value = (((-rel_mx + m_Size.x * 0.5f) * (valueRange.y - valueRange.x)) / m_Size.x) + valueRange.x;

	SendEvent(new GUIEvent(GUIEvent::SLIDER_CHANGED));

	return 1;
}

void GUISlider::PreRender(Renderer* r){ 
	GUIEntity::PreRender(r);
}

void GUISlider::Render(Renderer*)
{
	renderable.Draw();

	float w = valueRange.y - valueRange.x;
	float width = m_Size.x - pinSize.x;
	float x;

	glBegin(GL_LINES);
	glColor4f(1,1,1, 0.5f);
	for (int i = 0; i < referencePoints.size(); i++) {
		x = (width * ((referencePoints[i] - valueRange.x) / w)) - (width * 0.5f);
		glVertex2f(x, -m_Size.y / 2);
		glVertex2f(x, m_Size.y / 2);
	}
	glEnd();

	glPushMatrix();
	
	x = (width * ((value - valueRange.x) / w)) - (width * 0.5f);

	glTranslatef(x, 0, 0);

	renderableControlPin.Draw();

	glPopMatrix();
	


}

void GUISlider::PostRender(){ 
	GUIEntity::PostRender();
}

#endif