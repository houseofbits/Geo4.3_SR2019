#pragma once
#include "../EventReceiver.h"

class GUIEvent : public BaseEvent {
	CLASS_PROTOTYPE(GUIEvent);
public:
	enum Type{
		BUTTON_PRESSED = 1,
		LIST_SELECTED,
		WINDOW_SHOWN,
		WINDOW_HIDDEN,
		SLIDER_CHANGED,
		TEXT_INPUT_CHANGED,
	};
	GUIEvent() : type(BUTTON_PRESSED) {}
	GUIEvent(Type t) : type(t){}
	virtual ~GUIEvent(void) {}

	Type			type;
};