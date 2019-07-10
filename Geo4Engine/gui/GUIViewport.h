#pragma once
#include "GUIEntity.h"
#include "../TrueTypeFont.h"

class GUIViewport :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIViewport);
public:
	GUIViewport();
	~GUIViewport();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	void	keyPressedEvent(unsigned int);
	void	textEnterEvent(std::string);
	void	keyDownEvent(unsigned int);
	void	keyUpEvent(unsigned int);
	void	mouseMoveEvent(Vector2, Vector2, bool, bool);
	void	mouseButtonDownEvent(Vector2, bool, bool);
	void	mouseButtonUpEvent(Vector2, bool, bool);
	void	setModifierKeys(bool ctrl, bool shift, bool alt);

	Entity* hoverObject;

	int			window_width;
	int			window_height;

	TrueTypeFontFace ttfont;

	bool		showLoader;
	float		loaderTimeout;

	bool	keyCtrlPressed;
	bool	keyShiftPressed;
	bool	keyAltPressed;

};

