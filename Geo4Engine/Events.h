#pragma once

#include "EventReceiver.h"
#include "Math/HVector3.h"

class SceneManager;
/*
class FrameEvent : public BaseEvent{
CLASS_PROTOTYPE(FrameEvent);
public:
	FrameEvent(float ft, float t, SceneManager* s):frametime(ft), time(t), scene_mgr(s){}
	virtual ~FrameEvent(void){}
	float frametime;
	float time;
	SceneManager* scene_mgr;
};

class SceneManagerEvent : public BaseEvent
{
CLASS_PROTOTYPE(SceneManagerEvent);
public:
	SceneManagerEvent():type(0), entity(0), node(0), filename(""){}
	SceneManagerEvent(unsigned int t, Entity* e, CFONode* n=0):type(t), entity(e), node(n), filename(""){}
	SceneManagerEvent(unsigned int t, Entity* e, std::string str):type(t), entity(e), node(0), filename(str){}
	virtual ~SceneManagerEvent(void){}

	unsigned int	type;
	Entity*			entity;
	CFONode*		node;
	std::string		filename;
};
*/