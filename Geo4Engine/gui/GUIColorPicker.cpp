#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUIColorPicker);

GUIColorPicker::GUIColorPicker() :
	resolutionX(60),
	resolutionY(40),
	selectedColor(0,0,0)
{	}

GUIColorPicker::~GUIColorPicker()
{	}

void GUIColorPicker::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIColorPicker::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIColorPicker::OnWindowEvent);

}

void GUIColorPicker::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);
}

bool GUIColorPicker::OnWindowEvent(WindowEvent*const event)
{
	if (event->eventType == WindowEvent::WINDOW_CREATED) {

	}
	return 1;
}

bool GUIColorPicker::OnGUIInputEvent(GUIInputEvent*const event)
{
	switch (event->type) {
	case GUIInputEvent::EventType::MOUSELEAVE:
		hoverState = 0;
		clickState = 0;
		break;
	};

	if (isVisible() == 0)return 1;
	
	Vector2 mp;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEUP:
		clickState = 0;
		mp = event->mousePosition - getWorldSpace(this->m_LocalPos);
		selectedColor = getColor(mp);		
		SendEvent(new GUIEvent(GUIEvent::BUTTON_PRESSED));
		break;
	case GUIInputEvent::EventType::MOUSEDOWN:
		clickState = 1;
		break;
	case GUIInputEvent::EventType::MOUSEENTER:
		hoverState = 1;
		break;
	};
	return 1;
}

Vector3 GUIColorPicker::getColor(Vector2 v) {
	Vector2 vp((v.x / m_Size.x) + 0.5f, (v.y / m_Size.y) + 0.5f);
	return Math::HSL2RGB(Vector3(vp.x, 1, vp.y));
}

void GUIColorPicker::PreRender(Renderer* r){
	GUIEntity::PreRender(r);
}

void GUIColorPicker::Render(Renderer* rnd)
{
	glPushMatrix();
	Vector2 hs = m_Size / 2;
	glTranslatef(-hs.x, -hs.y, 0);

	Vector2 cellSize(m_Size.x / resolutionX, m_Size.y / resolutionY);


	for (int x = 0; x < resolutionX; x++) {
		for (int y = 0; y < resolutionY; y++) {
			
			Vector2 uni((float)x/(float)resolutionX, (float)y / (float)resolutionY);
			Vector2 pc(x * cellSize.x, y * cellSize.y);

			Vector3 color = Math::HSL2RGB(Vector3(uni.x, 1, uni.y));
			
			glColor3f(color.x, color.y, color.z);
			glBegin(GL_QUADS);
				glVertex2f(pc.x, pc.y);
				glVertex2f(pc.x + cellSize.x, pc.y);
				glVertex2f(pc.x + cellSize.x, pc.y + cellSize.y);
				glVertex2f(pc.x, pc.y + cellSize.y);
			glEnd();
		}
	}


	glPopMatrix();
}

void GUIColorPicker::PostRender() {
	GUIEntity::PostRender();
}


#endif