#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUIList;

class GUIListItem :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIListItem);
public:
	GUIListItem();
	~GUIListItem();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, SceneManager*const);

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

	GUIList* parentList;

	bool	hoverState;
	bool	selected;

	string styleActiveName;
	string styleHoverName;
	string stylePressedName;

};

