#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUIWindow);

GUIWindow::GUIWindow() : renderableTitle(),
	renderableBody(),
	renderableShadow(),
	styleSheet(),
	titleBarSize(30),
	styleTitleName(),
	styleBodyName(),
	styleShadowName()
{	}

GUIWindow::~GUIWindow()
{	}

void GUIWindow::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIWindow::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIWindow::OnWindowEvent);

	resourceManager->Get(styleSheet, "style/styles.cfo");

	renderableTitle.style = &styleSheet->get(styleTitleName);
	renderableBody.style = &styleSheet->get(styleBodyName);
	renderableShadow.style = &styleSheet->get(styleShadowName);
}

void GUIWindow::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	node->getValueBool("showOverlay", showOverlay);

	styleTitleName = "windowDefaultTitle";
	styleBodyName = "windowDefaultBody";
	styleShadowName = "windowDefaultShadow";

	string style = "";
	if (node->getValueString("style", style)) {
		styleTitleName = style + "Title";
		styleBodyName = style + "Body";
		styleShadowName = style + "Shadow";
	}

	node->getValueString("styleTitle", styleTitleName);
	node->getValueString("styleBody", styleBodyName);
	node->getValueString("styleShadow", styleShadowName);
	node->getValueFloat("titleBarSize", titleBarSize);

	renderableTitle.size = m_Size;
	renderableTitle.size.y = titleBarSize;
	renderableBody.size = m_Size;
	renderableBody.size.y = m_Size.y - titleBarSize;
	renderableShadow.size = m_Size;
}

bool GUIWindow::OnWindowEvent(WindowEvent*const event)
{
	windowWidth = event->width;
	windowHeight = event->height;

	if (event->eventType == WindowEvent::WINDOW_CREATED) {

		renderableTitle.update();
		renderableBody.update();
		renderableShadow.update();

		renderableTitle.setText(m_Title);
	}
	return 1;
}

bool GUIWindow::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;

	Vector2 worldPos = getWorldSpace(m_LocalPos);
	Vector2 setLocalPos = m_LocalPos;
	switch (event->type) {
	case GUIInputEvent::EventType::DRAG:

		m_LocalPos += event->mouseMotion;	
		
//		if (worldPos.x < (m_Size.x * 0.5f))worldPos.x = (m_Size.x * 0.5f);
//		if (worldPos.y < (m_Size.y * 0.5f))worldPos.y = (m_Size.y * 0.5f);

		break;
	case GUIInputEvent::EventType::MOUSEMOVE:
		break;
	case GUIInputEvent::EventType::MOUSEDOWN:
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

void GUIWindow::PreRender(Renderer* r)
{
	if (m_Blocking && m_Visible) {
		glColor4f(0, 0, 0, 0.5f);
		glBegin(GL_QUADS);
		glVertex2f(0, 0);
		glVertex2f(0, (float)windowHeight);
		glVertex2f((float)windowWidth, (float)windowHeight);
		glVertex2f((float)windowWidth, 0);
		glEnd();
	}
	GUIEntity::PreRender(r);
}

void GUIWindow::Render(Renderer* rnd){

	renderableShadow.Draw();
	
	glPushMatrix();
	glTranslatef(0, (m_Size.y * 0.5f) - (titleBarSize * 0.5f), 0);
	renderableTitle.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,  - (titleBarSize * 0.5f), 0);
	renderableBody.Draw();
	glPopMatrix();
}

void GUIWindow::PostRender() {
	GUIEntity::PostRender();
}

#endif
