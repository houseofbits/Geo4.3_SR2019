#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"

class GraphBaseValue {
public:
	GraphBaseValue():value(0){}
	GraphBaseValue(float v):value(v){}
	virtual void setValue(float val) { value = val; }
	virtual string getDisplayVal() { return "x"; }
	float	value;
};

class GraphSegment {
public: 
	GraphSegment():name(){}
	GraphSegment(string n) :name(n){}
	string name;
	std::deque<GraphBaseValue*>	values;
};

class GUIGraph :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIGraph);
public:
	GUIGraph();
	~GUIGraph();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	unsigned int	createGraph(string name) { graphs.push_back(GraphSegment(name)); return (unsigned int)graphs.size()-1; }
	void	addGraphValue(unsigned int index, GraphBaseValue* val);
	void	ScaleY();

	GUIRenderable	renderable;

	GUIStyleResourceHandle styleSheet;

	string			styleName;

	Vector2			gridStep;
	Vector2			gridDivideStep;
	Vector2			gridReference;
	Vector2			xLimits;
	Vector2			yLimits;
	bool			autoScaleY;
	bool			showYValues;

	std::vector<GraphSegment>	graphs;

};

