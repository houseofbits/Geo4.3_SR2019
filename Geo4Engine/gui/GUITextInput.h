#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GUITextInput :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUITextInput);
public:
	GUITextInput();
	~GUITextInput();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	virtual void	setTitle(std::string t) { 
		m_Title = t; 
		renderable.setTextWithWrapping(m_Title); 
		renderableActive.setTextWithWrapping(m_Title);
	}

	GUIRenderable	renderable;
	GUIRenderable	renderableActive;

	GUIStyleResourceHandle styleSheet;

	string styleName;
	string styleNameActive;

	unsigned int	maxCharacters;
	float			keyDownDelay;
	float			keyDownTimer;
	bool			backspaceDown;
};

