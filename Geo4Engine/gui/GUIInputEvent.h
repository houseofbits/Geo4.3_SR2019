#pragma once
#include "../EventReceiver.h"
class GUIInputEvent : public BaseEvent
{
CLASS_PROTOTYPE(GUIInputEvent);
public:
	enum EventType {
		MOUSEDOWN,
		MOUSEUP,
		MOUSEENTER,
		MOUSELEAVE,
		MOUSEMOVE,
		KEYDOWN,
		KEYUP,
		KEYPRESS,
		DRAG,
		TEXENTER,
		ENTERFOCUS,
		LEAVEFOCUS
	};

	GUIInputEvent()	: type(MOUSEDOWN),
		mousePosition(),
		mouseMotion(),
		keyCode(0),
		mouseButtonLeft(false),
		mouseButtonRight(false),
		textInput() {}
	GUIInputEvent(EventType type, unsigned int key) : type(type), keyCode(key) {	}
	GUIInputEvent(EventType type, Vector2 mousePosition, bool mouseLeft, bool mouseRight) : type(type), 
		mousePosition(mousePosition), 
		mouseButtonLeft(mouseLeft),
		mouseButtonRight(mouseRight) {	}
	GUIInputEvent(EventType type, Vector2 mousePosition, Vector2 mouseRel) : type(type), 
		mousePosition(mousePosition), 
		mouseMotion(mouseRel) {	}	
	GUIInputEvent(EventType type, Vector2 mousePosition, Vector2 mouseRel, bool mouseLeft, bool mouseRight) : type(type), 
		mousePosition(mousePosition), 
		mouseMotion(mouseRel),
		mouseButtonLeft(mouseLeft),
		mouseButtonRight(mouseRight) {	}
	GUIInputEvent(EventType type, std::string text) : type(type), textInput(text) {	}
	GUIInputEvent(EventType type) : type(type) {	}

	virtual ~GUIInputEvent();
	
	EventType		type;
	Vector2			mousePosition;
	Vector2			mouseMotion;
	unsigned int	keyCode;
	bool			mouseButtonLeft;
	bool			mouseButtonRight;
	std::string		textInput;
	bool			modKeyShift;
	bool			modKeyAlt;
	bool			modKeyCtrl;
};

