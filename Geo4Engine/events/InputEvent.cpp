#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(InputEvent);

InputEvent::~InputEvent()
{
	delete sdl;
}

#endif