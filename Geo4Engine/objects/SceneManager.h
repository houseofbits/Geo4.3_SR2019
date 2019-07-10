#pragma once

#include "../Entity.h"
#include "../ObjectFactory.h"
#include "../ResourceManager.h"
#include "../CFODocument.h"
#include "../Events.h"
#include <stack>

class SceneManager : public Entity, public EventHandler
{
CLASS_PROTOTYPE(SceneManager);
public:
	SceneManager(void);
	SceneManager(string cfoName);
	virtual ~SceneManager(void);

	template<class O>
	void	RegisterObjectType(O o){object_factory.RegisterObject(o, o.getTypename());}
	template<class O>
	void	RegisterResourceType(O o){	
		resource_manager.RegisterResourceType(o);
	}

	bool	process();

	void	DrawRecursive(Entity* parent);
	
	bool	OnSceneManagerEvent(SceneManagerEvent*const);

	void	LoadCFO(std::string);
	
	enum	{LOAD_OBJECTS_FROM_NODE, LOAD_OBJECTS_FROM_FILE, DELETE_OBJECTS};

	ObjectFactory<Entity>		object_factory;
	EventManager				event_dispatcher;
	ResourceManager				resource_manager;

private:
	class ObjectManager
	{
	public:
		ObjectManager():type(0), entity(0), node(0), filename(""){}
		ObjectManager(unsigned int t, Entity* e, CFONode* n=0):type(t), entity(e), node(n), filename(""){}
		ObjectManager(unsigned int t, Entity* e, std::string str):type(t), entity(e), node(0), filename(str){}
		~ObjectManager(){}
		unsigned int	type;
		Entity*			entity;
		CFONode*		node;
		std::string		filename;
	};

	stack<ObjectManager*>		object_manager_stack;							
};
