#pragma once
#include "Geo4.h"
#include "Serial.h"
#include "gui/GUIEvent.h"
//#include "gui/GUIWings.h"

class GUIWings;

class Interface : public Entity, public EventHandler
{
CLASS_PROTOTYPE(Interface);
public:
	Interface(void);
	virtual ~Interface(void);
	
	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIEvent(GUIEvent*const);

	bool	isRenderable() { return false; }

	GUIWings*	wings;
	GUISlider* 	timeline;

	void		updateTimeline();
	void		updateKeyIndex();

	bool		getInterpolatedKeyframe(float t, WingsKeyframe&);
	int			getNextKeyframe(float t, WingsKeyframe&);
	int			getPreviousKeyframe(float t, WingsKeyframe&);
	void		sortKeyframesByTime();
	void		scaleTimeline(float tmin,float tmax);
	void		expandTimeline(float left, float right);
	void		populateResourcesWindow();

	vector<WingsKeyframe*> keyframes;

	float	playSpeed;
	bool	playing;

	int		selectedKeyIndex;

	string	resourceFileName;
};	
