#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUIButton :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIButton);
public:
	GUIButton();
	~GUIButton();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	GUIRenderable	renderableActive;
	GUIRenderable	renderableHover;
	GUIRenderable	renderablePressed;

	GUIStyleResourceHandle styleSheet;

	bool	hoverState;
	bool	clickState;

	string styleActiveName;
	string styleHoverName;
	string stylePressedName;
};

