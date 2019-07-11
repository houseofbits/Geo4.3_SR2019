#include "../Geo4.h"

using namespace std;

CLASS_DECLARATION(SceneManager);

SceneManager::SceneManager(void) : Entity(),
	object_factory(), 
	resource_manager(), 
	event_dispatcher(), 
	EventHandler()
{ 
	event_dispatcher.RegisterEventHandler(this);
	event_dispatcher.RegisterEventReceiver(this, &SceneManager::OnSceneManagerEvent);

#ifndef NO_OPENGL
	resource_manager.RegisterResourceType(TrueTypeFontFace());
	resource_manager.RegisterResourceType(GUIStyleResource());
	RegisterObjectType(SDLWindow());
	RegisterObjectType(Viewport());
	RegisterObjectType(GUIViewport());
	RegisterObjectType(GUISlider());
	RegisterObjectType(GUIWindow());
	RegisterObjectType(GUIButton());
	RegisterObjectType(GUIText());
	RegisterObjectType(GUITextInput());
	RegisterObjectType(GUIList());
	RegisterObjectType(GUILayer());
	RegisterObjectType(GUIColorPicker());
	RegisterObjectType(GUIListItem());
	RegisterObjectType(GUIGraph());
	RegisterObjectType(GUIWings());
	RegisterObjectType(Interface());
	RegisterObjectType(BasicSerial());
#endif

}

SceneManager::SceneManager(string cfoName) : SceneManager(){

	LoadCFO(cfoName);
}

SceneManager::~SceneManager(void)
{ }

bool SceneManager::OnSceneManagerEvent(SceneManagerEvent*const evt)
{	
	switch(evt->type){
		case LOAD_OBJECTS_FROM_NODE:
			object_manager_stack.push(new ObjectManager(LOAD_OBJECTS_FROM_NODE, evt->entity, evt->node));
			break;
		case LOAD_OBJECTS_FROM_FILE:
			object_manager_stack.push(new ObjectManager(LOAD_OBJECTS_FROM_FILE, evt->entity, evt->filename));
			break;
		case DELETE_OBJECTS:
			object_manager_stack.push(new ObjectManager(DELETE_OBJECTS, evt->entity));
			break;
	};
	return 1;
}

void SceneManager::DrawRecursive(Entity* parent)
{
	if(!parent->isRenderable())return;

	parent->PreRender(0);
	parent->Render(0);
	if(parent->m_Childs.empty()){
		parent->PostRender();
		return;
	}
	list<Entity*>::iterator pos = parent->m_Childs.begin();
	while(pos!=parent->m_Childs.end())
	{
		DrawRecursive((*pos));
		pos++;	
	}
	parent->PostRender();
}

bool	SceneManager::process()
{	
	bool evt = event_dispatcher.ProcessEvents();

	if (!object_manager_stack.empty()) {
		while (!object_manager_stack.empty()) {
			switch (object_manager_stack.top()->type) {
			case LOAD_OBJECTS_FROM_NODE:				
				if (object_manager_stack.top()->node && object_manager_stack.top()->entity) {
					object_manager_stack.top()->entity->CreateObjects(object_manager_stack.top()->node);
				}
				break;
			case LOAD_OBJECTS_FROM_FILE:
				if (!object_manager_stack.top()->filename.empty() && object_manager_stack.top()->entity) {
					object_manager_stack.top()->entity->CreateObjects(&CFODocument(object_manager_stack.top()->filename));
				}
				break;
			case DELETE_OBJECTS:
				if (object_manager_stack.top()->entity && object_manager_stack.top()->entity->getParent()) {
					object_manager_stack.top()->entity->getParent()->RemoveEntity(object_manager_stack.top()->entity);
					object_manager_stack.top()->entity->Deinitialise(&event_dispatcher, this);
					delete object_manager_stack.top()->entity;
				}
				break;
			};
			delete object_manager_stack.top();
			object_manager_stack.pop();
		}
	}

	DrawRecursive(this);

	return evt;
}

void SceneManager::LoadCFO(std::string filename)
{
	SendEvent(new SceneManagerEvent(LOAD_OBJECTS_FROM_FILE, this, filename));
}