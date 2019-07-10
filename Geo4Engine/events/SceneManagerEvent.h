#pragma once
#include "../EventReceiver.h"
#include <string>

class CFONode;
class Entity;

class SceneManagerEvent :
	public BaseEvent
{
CLASS_PROTOTYPE(SceneManagerEvent);
public:
	SceneManagerEvent() :type(0), entity(0), node(0), filename("") {}
	SceneManagerEvent(unsigned int t, Entity* e, CFONode* n = 0) :type(t), entity(e), node(n), filename("") {}
	SceneManagerEvent(unsigned int t, Entity* e, std::string str) :type(t), entity(e), node(0), filename(str) {}
	virtual ~SceneManagerEvent(void) {}

	unsigned int	type;
	Entity*			entity;
	CFONode*		node;
	std::string		filename;
};

