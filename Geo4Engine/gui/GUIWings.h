#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class Wing {
public:
	Wing(){}
	Wing(float scale, float angle);

	void	draw(bool selected[7]);
	int		clip(Vector2);
	void	setColors(Vector3 c[7]);

	Vector3 colors[7];
	float	angle;
	float	scale;
	Vector2 quads[7][4];
};

class WingsKeyframe {
public:
	WingsKeyframe() {}
	WingsKeyframe(float tv, Vector3 c[12][7]) {
		t = tv;
		memcpy_s(colors, (sizeof(Vector3) * 12 * 7), c, (sizeof(Vector3) * 12 * 7));
	}
	void update(Vector3 c[12][7]) {
		memcpy_s(colors, (sizeof(Vector3) * 12 * 7), c, (sizeof(Vector3) * 12 * 7));
	}
	WingsKeyframe getInterpolated(float t, WingsKeyframe& other);
	float t;
	Vector3 colors[12][7];
};

class GUIWings :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIWings);
public:
	GUIWings();
	~GUIWings();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	void	setColorToSelected(Vector3);

	void	clearSelection();
	void	addSelection();
	void	selectAll();
	void	selectWing();
	void	selectRing();
	void	selectRingOdd();

	Wing*	wings[12];

	int		selectedWing;
	int		selectedSegment;

	bool	selection[12][7];

	TrueTypeFontFaceHandle fontHandle;

	WingsKeyframe	createKeyframe(float t);
	void			applyKeyframe(WingsKeyframe&);
};

