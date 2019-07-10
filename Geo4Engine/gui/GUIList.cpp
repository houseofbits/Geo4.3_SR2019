#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUIList);

GUIList::GUIList() : itemHeight(30),
	styleSheet(),
	renderableBody(),
	renderableTitle(),
	multiSelect(false),
	listItemNodes(),
	titleBarSize(30)
{	}

GUIList::~GUIList()
{	}

void GUIList::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	resourceManager->Get(styleSheet, "style/styles.cfo");

	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIList::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIList::OnWindowEvent);

	renderableBody.style = &styleSheet->get("listBody");
	renderableTitle.style = &styleSheet->get("listTitle");

	//addItem("66", "Dynamic item");
	reorderItems();
}

void GUIList::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	node->getValueBool("multiSelect", multiSelect);
	node->getValueFloat("itemHeight", itemHeight);



	//renderableTitle.size = m_Size;
	//renderableTitle.size.y = titleBarSize;
	renderableBody.size = m_Size;
	renderableBody.size.y = m_Size.y - titleBarSize;
//	renderableShadow.size = m_Size;
}

bool GUIList::OnWindowEvent(WindowEvent*const event)
{
	if (event->eventType == WindowEvent::WINDOW_CREATED) {
		renderableTitle.update();
		renderableBody.update();

		renderableTitle.setText(m_Title);
	}
	return 1;
}

bool GUIList::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEUP:

		break;
	case GUIInputEvent::EventType::MOUSEDOWN:

		break;
	case GUIInputEvent::EventType::MOUSEENTER:

		break;
	case GUIInputEvent::EventType::MOUSELEAVE:

		break;
	};
	return 1;
}

void GUIList::PreRender(Renderer* r) {
	GUIEntity::PreRender(r);
}

void GUIList::Render(Renderer* rnd){
	/*
	glPushMatrix();
	glTranslatef(0, (m_Size.y * 0.5f) - (titleBarSize * 0.5f), 0);
	renderableTitle.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -(titleBarSize * 0.5f), 0);
	renderableBody.Draw();
	glPopMatrix();
	*/
}

void GUIList::PostRender() {
	GUIEntity::PostRender();
}

void GUIList::DeselectAll() {
	std::list<Entity*>::iterator pos = m_Childs.begin();
	while (pos != m_Childs.end()) {		
		if ((*pos)->getTypename() == "GUIListItem") {
			GUIListItem* item = (GUIListItem*)(*pos);
			item->selected = false;
		}
		pos++;
	}
}

void GUIList::reorderItems() {
	float top = (m_Size.y * 0.5f) - (itemHeight * 0.5f);
	std::list<Entity*>::iterator pos = m_Childs.begin();
	while (pos != m_Childs.end()) {
		if ((*pos)->getTypename() == "GUIListItem") {
			GUIListItem* item = (GUIListItem*)(*pos);

			item->setLocalPosition(Vector2(0, top));

			top -= itemHeight;
		}
		pos++;
	}
}
void GUIList::addItem(string name, string title) {
	CFONode * node = new CFONode();
	node->SetName("GUIListItem");
	node->CreateChildNode("name", name);
	node->CreateChildNode("title", title);	
	listItemNodes.push_back(node);
	SendEvent(new SceneManagerEvent(SceneManager::LOAD_OBJECTS_FROM_NODE, this, node));
}
void GUIList::deleteItem(string name) {
	//TODO
}

void GUIList::clearItems()
{
	std::list<Entity*>::iterator pos = m_Childs.begin();
	while (pos != m_Childs.end()) {
		SendEvent(new SceneManagerEvent(SceneManager::DELETE_OBJECTS, (*pos)));
		pos++;
	}
	m_Childs.clear();
}

GUIListItem*  GUIList::getSelectedItem() {

	std::list<Entity*>::iterator pos = m_Childs.begin();
	while (pos != m_Childs.end()) {
		if ((*pos)->getTypename() == "GUIListItem") {
			GUIListItem* item = (GUIListItem*)(*pos);
			if (item->selected)return item;
		}
		pos++;
	}

	return 0;
}

#endif