#pragma once

#include <iostream>
#include <list>
#include "Entity.h"
#include <algorithm>

using namespace std;

class BaseEvent{
public:
	BaseEvent():m_Sender(0), m_Delayed(0), m_Target(0), m_ExecutionTime(0){}
	virtual ~BaseEvent(){}
	virtual std::string			getTypename(){return "BaseEvent";}
	Entity*		m_Sender;
	Entity*		m_Target;
	float			m_ExecutionTime;	//laiks kad events ir jaapalaizh, tiek uzstaadiits izmantojot timestamp + delay, tiek chekots pret timestamp
	float			m_Delayed;		//events ar nokaveeshanos
	//int			m_Target;		E_ALL | E_CHILDREN | E_NEIGHBOURS | E_SUBTREE | E_OBJECT
};

//template<class OBJ>class EventHandler;
class EventManager;
//template<class OBJ>
class EventHandler
{
friend class EventManager;
public:
	EventHandler():m_EventManagerPtr(0), m_ParentObjectPtr(0){}
	virtual ~EventHandler();

	void	SendEvent(BaseEvent* evt, float delay=0);
	void	SendEvent(BaseEvent* evt, Entity* target, float delay=0);

private:
	EventManager*	m_EventManagerPtr;
	Entity*			m_ParentObjectPtr;
};


class EventManager
{
	friend class EventHandler;
public:
	EventManager(){
		time = 0;
		next_time = 0;
		current = new TEventList();
		next = new TEventList();
		delayed = new TEventList();
	}
	virtual ~EventManager(){
	//	delete current;
	//	delete next;
	//	delete delayed;
	}

private:
    class IFunctor{
	public:
		virtual bool operator()(BaseEvent*){return 1;};
		virtual std::string	getTypeName(){return "";};
		virtual bool	whoAmI(Entity*){return 0;}
		virtual Entity*	getEntityPtr(){return 0;}
	};
	template<class OBJ, class EVT>
	class EventFunctor : public IFunctor{
	public:
		EventFunctor(){}
		EventFunctor(OBJ* o, bool (OBJ::*func_)(EVT*)){
			obj = o;
			func = func_;
			type_name = EVT::type_name;
		}
		bool	whoAmI(Entity* o){
			//std::cout<<o<<" "<<static_cast<Entity*>(obj)<<std::endl;
			//std::cout<<o->getName()<<" "<<obj->getName()<<std::endl;
			return (static_cast<Entity*>(obj) == o);		
		}
		
		bool operator()(BaseEvent* evt){
			return (*obj.*func)(static_cast<EVT*>(evt));
		}
		bool	(OBJ::*func)(EVT*const);
		OBJ*	obj;

		Entity*	getEntityPtr(){return static_cast<Entity*>(obj);}

		std::string		type_name;
		std::string		getTypeName(){return type_name;}
	};

	typedef std::vector<IFunctor*> TFunctorArray;

	struct FindFunctorByTypename{
		FindFunctorByTypename(string n):name(n){}
		inline bool operator () (IFunctor* obj) {return obj->getTypeName()==name;} 
		string name;
	};
	struct FindFunctorByEntity{
		FindFunctorByEntity(Entity* e):ent(e){}
		inline bool operator () (IFunctor* obj) {
			if(ent==0)return false;
			return obj->whoAmI(ent);
		} 
		Entity* ent;
	};
	
	TFunctorArray	functors;

	typedef std::list<BaseEvent*> TEventList;

	struct SortEventsByTypeName{
		inline bool operator()(BaseEvent* lhs, BaseEvent* rhs){
			return lhs->getTypename() < rhs->getTypename();
		}
	};
	void	AddEvent(BaseEvent* event){
		if(event->m_Delayed > 0){
			event->m_ExecutionTime = time + event->m_Delayed;
			delayed->push_back(event);
		}else{
			next->push_back(event);
		}
	}

	void	DeleteEventReceiver(Entity* ent){
	//	cout<<"Want to delete rec: "<<ent<<endl;
		TFunctorArray::iterator pos = functors.begin();
		while(pos!=functors.end()){
			if((*pos)->whoAmI(ent)){
			//	cout<<"delete rec"<<endl;
				pos = functors.erase(pos);
			}else{
				pos++;	
			}
		}
	//	TFunctorArray::iterator pos = functors.begin();
	//	pos = find_if (functors.begin(), functors.end(), FindFunctorByEntity(ent));
	//	if(pos!=functors.end()){
	//		functors.erase(pos);
	//		
	//	}
	}
public:	

	template<class OBJ,class EVT>
	void	RegisterEventReceiver(OBJ* obj, bool (OBJ::*func_)(EVT*const)){
		EventFunctor<OBJ, EVT>* funct = new EventFunctor<OBJ, EVT>(obj, func_);
		functors.push_back(funct);	
		RegisterEventHandler(obj);
	}

	void	SendEvent(BaseEvent* evt, float delay=0){
			if(delay>0){
				evt->m_Delayed = delay;
			}
			AddEvent(evt);
	}
	void	SendEvent(BaseEvent* evt, Entity* target, float delay=0){
			if(delay>0){
				evt->m_Delayed = delay;
			}
			if(target){
				evt->m_Target = target;
			}
			AddEvent(evt);
	}

	void PrintDebug(){
		cout<<"-----------------------------"<<endl;
		TFunctorArray::iterator pos = functors.begin();
		while(pos!=functors.end()){
			cout<<(*pos)->getTypeName()<<" "<<(*pos)->getEntityPtr()->getName()<<" ["<<(*pos)->getEntityPtr()->getTypename()<<"]"<<endl;
		pos++;
		}
		cout<<endl<<"current events"<<endl;
		TEventList::iterator pos2 = current->begin();
		while(pos2!=current->end()){
			cout<<" evt: "<<(*pos2)<<endl;
			pos2++;
		}
		cout<<"next events"<<endl;
		pos2= next->begin();
		while(pos2!=next->end()){
			cout<<" evt: "<<(*pos2)<<endl;
			pos2++;
		}
	}

	bool ProcessEvents(){
		
		time = next_time;

		if(!delayed->empty()){
			TEventList::iterator iter = delayed->begin();
			while(iter!=delayed->end()){
				if((*iter)->m_ExecutionTime <= time){
					current->push_back((*iter));
					delayed->erase(iter);
					iter = delayed->begin();
				}
				iter++;
			}			
		}
		//bljedj: domaat vajag, domaat!
		if(!current->empty()){
			
			TFunctorArray::iterator pos = functors.begin();
			while(pos!=functors.end()){
				TEventList::iterator iter = current->begin();
				while(iter!=current->end()){					
					if((*pos)->getTypeName()==(*iter)->getTypename()){
					/*	if((*iter)->m_Target){
							std::cout<<"target is: "<<(*iter)->m_Target->getName()<<" "<<(*iter)->m_Target<<std::endl;
						}*/
						if((*iter)->m_Target && (*pos)->whoAmI((*iter)->m_Target)){
						//	std::cout<<"send to target"<<std::endl;
							if((*(*pos))((*iter))==0)return 0;
						}else if((*iter)->m_Target==0){
							if((*(*pos))((*iter))==0)return 0;
						}
					}
				iter++;
				}
			pos++;
			}
			TEventList::iterator iter = current->begin();
			while(iter!=current->end()){
				delete (*iter);
			iter++;
			}
			current->clear();
			/*
			current->sort(SortEventsByTypeName());
			string type_name;
			
			TFunctorArray::iterator pos = functors.begin();
			while(!current->empty()){
				type_name = (*current->begin())->getTypename();
				if((*pos)->getTypeName()!=type_name){
					pos = find_if (functors.begin(), functors.end(), FindFunctorByTypename(type_name));
				}
				if((*pos)->getTypeName()==type_name){
					if((*(*pos))(*current->begin())==0)return 0;
				}

				delete (*current->begin());
				current->erase(current->begin());
			}*/
		}


		std::swap(current, next);
		return 1;
	}
	//Set time, for timed events to work
	void SetCurrentTime(float t) {
		next_time = t;
	}

	template <class OB>
	void	RegisterEventHandler(OB* object){
		if(object){
			EventHandler* handler = static_cast<OB*>(object);
			handler->m_EventManagerPtr = this;
			handler->m_ParentObjectPtr = static_cast<Entity*>(object);
		}
	}

	float			time;	
	float			next_time;

	TEventList*		current;
	TEventList*		next;
	TEventList*		delayed;

};












/*


class EventManager
{
public:
//	void	SendEvent(SDL_Event&);
	void	SendEvent(BaseEvent* evt){
	//	m_EventList.push_back(evt);
		process_event_list.push_back(evt);
	}
	void	RegisterEventHandler(IEventHandler* handler);
	void	UnregisterEventHandler(IEventHandler* handler);

	bool	PoolEvents(){
		if(m_EventList.empty() || m_EventHandlers.empty())return 1;
		while(!m_EventList.empty()){
			std::list<IEventHandler*>::iterator iter = m_EventHandlers.begin();
			while(iter!=m_EventHandlers.end()){
				if(!(*iter)->OnEvent((*m_EventList.begin())))return 0;
				iter++;
			}

		//	cout<<"event"<<endl;
			delete *m_EventList.begin();
			m_EventList.erase(m_EventList.begin());		
		}
		return 1;
	}


	typedef bool(*EventReceiverFunc)(BaseEvent*);
	typedef std::vector<EventReceiverFunc> EventRecArray;
	typedef std::pair<std::string, EventRecArray> EventArrayAsoc;
	typedef std::vector<EventArrayAsoc> EventRecRegister;
	typedef std::list<BaseEvent*> EventList;
	
	struct FindEventAsocArray{
		FindEventAsocArray(string n):name(n){}
		inline bool operator () (const EventArrayAsoc& obj) {return obj.first==name;} 
		string name;
	};
	
	struct SortAsocArrayByName{
		inline bool operator()(EventArrayAsoc& lhs, EventArrayAsoc& rhs){
			return lhs.first < rhs.first;
		}
	};
	void	RegisterEventReceiver(EventReceiverFunc func, std::string event_type){
		if(!event_rec_register.empty()){
			EventRecRegister::iterator pos = find_if (event_rec_register.begin(), event_rec_register.end(), FindEventAsocArray(event_type));  
			if(pos!=event_rec_register.end()){	//found
				(*pos).second.push_back(func);
			}else{	//not found
				event_rec_register.insert(event_rec_register.begin(), EventArrayAsoc(event_type, EventRecArray()));
				(*event_rec_register.begin()).second.push_back(func);				
			}
		}else{
			event_rec_register.insert(event_rec_register.begin(), EventArrayAsoc(event_type, EventRecArray()));
			(*event_rec_register.begin()).second.push_back(func);
		}
		std::sort(event_rec_register.begin(), event_rec_register.end(), SortAsocArrayByName());
	//	m_EventReceivers.push_back(std::pair<std::string, EventReceiverFunc>(event_type, func));
	}
	

//	std::vector<std::pair<std::string, EventReceiverFunc> >	m_EventReceivers;


	EventRecRegister event_rec_register;
	EventList process_event_list;
	EventList* next_event_list;
	
	struct SortEventsByName{
		inline bool operator()(BaseEvent* lhs, BaseEvent* rhs){
			return lhs->getTypename() < rhs->getTypename();
		}
	};

	bool	PoolEvents2(){

		process_event_list.sort(SortEventsByName());
		
	



		EventList::iterator pose = process_event_list.begin();
		while(pose!=process_event_list.end()){
			cout<<(*pose)<<" "<<(*pose)->getTypename()<<endl;
		pose++;
		}
		cout<<"-----------------------"<<endl;

		EventRecRegister::iterator pos = event_rec_register.begin();
		while(pos!=event_rec_register.end()){
			cout<<(*pos).first<<endl;
			EventRecArray::iterator pos2 = (*pos).second.begin();
			while(pos2!=(*pos).second.end()){
				cout<<"  func: "<<(*pos2)<<endl;
				pos2++;
			}
		pos++;
		}

		return 1;

		sort events by name
		vector<eventreceiverfunc>* ptrtohandlers;
		string lasthandlertype
		for(all events in process_event_list)
			if(lasthandlername!=eventname)
				ptrtohandlers = find(eventname)
			if(!ptrtohandlers.empty())
				for(all ptrtohandlers)
					if(ptrtohandlers[i].handle(event)==0)return 0
		clear process_event_list;
		swap(process_event_list, next_event_list);
	}	


	std::list<IEventHandler*>	m_EventHandlers;
	std::list<BaseEvent*>		m_EventList;
};

/**/
