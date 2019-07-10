#pragma once

#include "../Entity.h"
#include "../Events.h"
#include "../gui/GUIViewport.h"

#include "../../Lib/SDL2/include/SDL.h"  

class SDLWindow :	public Entity, public EventHandler
{
CLASS_PROTOTYPE(SDLWindow);
public:
	SDLWindow(void);
	virtual ~SDLWindow(void);

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const){};

	void	Serialize(CFONode*){}
	void	Deserialize(CFONode*);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	bool	OnWindowEvent(WindowEvent*const);

	void	processGUIEvents(SDL_Event*);

	int				width;
	int				height;
	std::string		fullscreen;
	std::string		window_title;
	unsigned int	rotate;

	SDL_Window*		window;
	SDL_GLContext	glcontext;

	float			time;
	float			frame_time;

	SDL_Joystick*	joystick;

	vector<GUIViewport*> guiViewports;
};
