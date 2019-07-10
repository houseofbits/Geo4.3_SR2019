#pragma once
#include "Geo4.h"

class GUISlider: public GUIEntity, public EventHandler
{
CLASS_PROTOTYPE(GUISlider);
public:	
	GUISlider(void);
	virtual ~GUISlider(void);

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const){};

	void	Serialize(CFONode*){}
	void	Deserialize(CFONode*);
	
	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();
	
	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);
		
	bool	OnChangeValue(Vector2);
	bool	IsOver(Vector2);

	void setValue(float val);

	GUIRenderable	renderable;
	GUIRenderable	renderableControlPin;

	GUIStyleResourceHandle styleSheet;

	string styleName;
	string styleNameControlPin;

	Vector2			valueRange;
	float			value;

	Vector2			pinSize;

	vector<float>	referencePoints;
};
