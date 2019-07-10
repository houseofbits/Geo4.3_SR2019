#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUIColorPicker :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIColorPicker);
public:
	GUIColorPicker();
	~GUIColorPicker();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	bool	hoverState;
	bool	clickState;
	
	int		resolutionX;
	int		resolutionY;

	Vector3 getColor(Vector2);

	Vector3 selectedColor;

};

