#pragma once

#include "../EventReceiver.h"

union SDL_Event;

class InputEvent :
	public BaseEvent
{
CLASS_PROTOTYPE(InputEvent);
public:

	InputEvent(SDL_Event* e) : sdl(e){	}
	~InputEvent();

	SDL_Event* sdl;

};

