#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUILayer);

GUILayer::GUILayer() : renderable(),
	styleSheet(),
	styleName()
{	}

GUILayer::~GUILayer()
{	}

void GUILayer::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);

	resourceManager->Get(styleSheet, "style/styles.cfo");

	renderable.style = &styleSheet->get(styleName);

	renderable.update();

	renderable.setTextWithWrapping(m_Title);
}

void GUILayer::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);	

	styleName = "layer";

	node->getValueString("style", styleName);

	renderable.size = m_Size;
}

void GUILayer::PreRender(Renderer* r){
	GUIEntity::PreRender(r);
}

void GUILayer::Render(Renderer* rnd){
	renderable.Draw();
}

void GUILayer::PostRender(){
	GUIEntity::PostRender();
}

#endif