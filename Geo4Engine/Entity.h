#pragma once

#include <string>
#include <list>
#include <algorithm>
#include <exception>
#include "math/HVector2.h"
#include "StringUtils.h"

class SceneManager;
//class Scene;
class EventManager;
class ResourceManager;
class CFONode;
//class GUIElement;
class Renderer;

/*
	Objektu sorteeshana

	notiek peec zindex veertiibas
	
	objekti kuriem nav defineeti zindex tiek apstraadaati pirmie

	gui elementi kuriem nav zindex vai zindex = -1 var tikt dinamiski sorteeti ar BringToFront paliidziibu
	gui elementi kuriem ir defineets zindex vienmeer atradiisies augstaak nekaa tie kuriem tas nav defineets, 
	piemeeram kursors

	kuraa vietaa notiek pirmaa sorteeshana peec zindex?

	burbuljsorteeshana

*/

#define BASE_CLASS_PROTOTYPE(classname)		\
	public:									\
		virtual bool isInstanceOf(std::string name){	return (name == #classname);	}


class Entity
{
	BASE_CLASS_PROTOTYPE(Entity)

	friend class SceneManager;
	friend class Scene;
	friend class GUIElement;
public:
	static unsigned int	GLOBAL_OBJECT_TYPE_COUNTER;

	Entity() : m_Name(""), m_HashName(0),  m_Parent(0), m_Childs() {}
	virtual ~Entity(){}
	
	virtual void	Initialise(EventManager*const, ResourceManager*const){};
	virtual void	Deinitialise(EventManager*const, SceneManager*const){};

	virtual void	Serialize(CFONode*){}
	virtual void	Deserialize(CFONode*);
	
	virtual void	PreRender(Renderer*){}
	virtual void	Render(Renderer*){}
	virtual void	PostRender(){}

	void						RemoveEntity(Entity* e){m_Childs.remove(e);}

	virtual std::string			getTypename(){return "Entity";}
	virtual unsigned int		getTypeId(){return 0;}

	std::string					getName(){return m_Name;}
	unsigned int 				getHashName() { return m_HashName; }
	void						setName(std::string name);
	void						addChildObject(Entity* obj){m_Childs.push_back(obj);}
	void						setParent(Entity* obj){m_Parent = obj;}

	Entity*					getParent(){return m_Parent;}
	Entity*					getRootObject(){
		if(m_Parent==0)return this;
		else return m_Parent->getRootObject();
	}
	
	template<class T>
	T*	getParent() { 
		if(m_Parent && T::TypeName() == m_Parent->getTypename()){
			return (T*)m_Parent;
		}
		return 0;
	}

	SceneManager*	getSceneManager();

	Entity*					getObjectByName(std::string name){
		Entity*	got_root = getRootObject();
		if(got_root){
			bool f=0;
			Entity* obj=0;
			got_root->recursiveFindObjectByName(obj,name, f);
			return obj;
		}
		return 0;
	}
	template<class T>
	T*	getObjectByName(std::string name) {
		Entity*	got_root = getRootObject();
		if (got_root) {
			bool f = 0;
			Entity* obj = 0;
			got_root->recursiveFindObjectByHashName(obj, hashStr(name.c_str()), f);
			if (obj && T::TypeID() == obj->getTypeId()) {
				return (T*)obj;
			}
			return 0;
		}
		return 0;
	}

	Entity*					getChildObjectByName(std::string name){
		bool f=0;
		Entity* obj=0;
		recursiveFindObjectByHashName(obj, hashStr(name.c_str()), f);
		return obj;
	}

	template<class T>
	T*	getChildObjectByName(std::string name) {
		bool f = 0;
		Entity* obj = 0;
		recursiveFindObjectByHashName(obj, hashStr(name.c_str()), f);
		if (obj && T::TypeID() == obj->getTypeId()) {
			return (T*)obj;
		}
		return 0;
	}

	unsigned int	getObjectsByClassName(std::string className, std::vector<Entity*>& out) {
		Entity*	got_root = getRootObject();
		if (got_root) {
			
			got_root->_recursiveFingObjectByClassName(className, out);
			return (unsigned int)out.size();
		}
		return 0;
	}
	template<class T>
	unsigned int	getObjectsByClassName(std::vector<T*>& out) {
		Entity*	got_root = getRootObject();
		if (got_root) {
			got_root->_recursiveFingObjectByClassName(T::TypeName(), out);
			return (unsigned int)out.size();
		}
		return 0;
	}

	template<class T>
	T*	instanceOf() {
		if (T::TypeID() == getTypeId()) {
			return (T*)this;
		}
		throw -1;
	}
	template<class T>
	T*	instanceOf(std::string name) {
		T* e = getObjectByName<T>(name);
		if (e)return e;
		throw -2;
	}
	void	CreateObjects(CFONode*);
	void	SaveObjects(CFONode*, bool include_parent=0);

	std::list<Entity*>*			getChildList(){return &m_Childs;}

	virtual bool		isRenderable() {	return true;	}

protected:	
	typedef std::vector<Entity*>	TEntityArray;
	void	_RecursiveDeserializeChilds(CFONode* node, Entity* parent, SceneManager*, TEntityArray* init_a=0);
	void	_RecursiveInitialiseChilds(Entity*, SceneManager*, TEntityArray* init_a=0);
	void	_RecursiveSerializeObject(CFONode* node, Entity* parent, SceneManager* mgr);

	template<class T>
	void	_recursiveFingObjectByClassName(std::string& className, std::vector<T*>& out) {
		if (getTypename() == className) {
			out.push_back((T*)this);
		}
		std::list<Entity*>::iterator pos = m_Childs.begin();
		while (pos != m_Childs.end()) {
			(*pos)->_recursiveFingObjectByClassName(className, out);
			pos++;
		}
	}

	void	recursiveFindObjectByName(Entity* &obj, const std::string& name, bool& found){
		if(found)return;
		if(getName()==name){
			obj = this;
			found=1;
			return;
		}else if(!m_Childs.empty()){
			std::list<Entity*>::iterator pos = m_Childs.begin();
			while(pos!=m_Childs.end()){
				(*pos)->recursiveFindObjectByName(obj, name, found);
				pos++;
			}
		}
	}

	void	recursiveFindObjectByHashName(Entity* &obj, const unsigned int& hname, bool& found) {
		if (found)return;
		if (getHashName() == hname) {
			obj = this;
			found = 1;
			return;
		}
		else if (!m_Childs.empty()) {
			std::list<Entity*>::iterator pos = m_Childs.begin();
			while (pos != m_Childs.end()) {
				(*pos)->recursiveFindObjectByHashName(obj, hname, found);
				pos++;
			}
		}
	}

	std::string				m_Name;
	unsigned int 			m_HashName;
	Entity*					m_Parent;
	std::list<Entity*>		m_Childs;

};

#define CLASS_PROTOTYPE(classname)			\
	public:									\
		virtual std::string			getTypename(){return type_name;}	\
		virtual unsigned int		getTypeId(){if(classname::type_id==0){Entity::GLOBAL_OBJECT_TYPE_COUNTER++; classname::type_id = Entity::GLOBAL_OBJECT_TYPE_COUNTER;}return classname::type_id;} \
		static	std::string			type_name;		\
		static	unsigned int		type_id;		\
		static	std::string			TypeName(){return classname::type_name;}	\
		static	unsigned int		TypeID(){if(classname::type_id==0){Entity::GLOBAL_OBJECT_TYPE_COUNTER++; classname::type_id = Entity::GLOBAL_OBJECT_TYPE_COUNTER;}return classname::type_id;}

#define CLASS_DECLARATION(classname)	\
	std::string classname::type_name = #classname; \
	unsigned int classname::type_id = 0;

		


















