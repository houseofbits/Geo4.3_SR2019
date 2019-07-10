// IEventReceiver.cpp: implementation of the IEventReceiver class.
//
//////////////////////////////////////////////////////////////////////

#include "EventReceiver.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
using namespace std;

EventHandler::~EventHandler(){
	if(m_EventManagerPtr && m_ParentObjectPtr){
		m_EventManagerPtr->DeleteEventReceiver(m_ParentObjectPtr);
	}
}

void EventHandler::SendEvent(BaseEvent* evt, float delay)
{
	if(m_EventManagerPtr && evt){	
		evt->m_Sender = m_ParentObjectPtr;
		m_EventManagerPtr->SendEvent(evt, delay);
	}
}

void EventHandler::SendEvent(BaseEvent* evt, Entity* target, float delay)
{
	if(m_EventManagerPtr && evt){	
		evt->m_Sender = m_ParentObjectPtr;
		m_EventManagerPtr->SendEvent(evt, target, delay);
	}
}



/*
void	EventManager::RegisterEventHandler(IEventHandler* handler){
	
	handler->m_EventDispatcherPtr = this;
	m_EventHandlers.push_back(handler);
}

void	IEventHandler::SendEvent(BaseEvent* evt){
	if(m_EventDispatcherPtr)m_EventDispatcherPtr->SendEvent(evt);
	else delete evt;
}

void	IEventHandler::Unregister(){
	if(m_EventDispatcherPtr)m_EventDispatcherPtr->UnregisterEventHandler(this);
}

void	EventManager::UnregisterEventHandler(IEventHandler* handler)
{
	if(!m_EventHandlers.empty())m_EventHandlers.remove(handler);
}
*/