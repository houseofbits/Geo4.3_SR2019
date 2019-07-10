#pragma once
#include "GUIEntity.h"
#include "GUIRenderable.h"
#include "GUIStyleResource.h"
#include "GUIListItem.h"

class GUIList :
	public GUIEntity,
	public EventHandler
{
CLASS_PROTOTYPE(GUIList);
public:
	GUIList();
	~GUIList();

	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const) {};

	void	Serialize(CFONode*) {}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnGUIInputEvent(GUIInputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	void	DeselectAll();	
	void	addItem(string name, string title);
	void	deleteItem(string name);
	void	reorderItems();
	void	clearItems();

	GUIListItem* getSelectedItem();

	float	itemHeight;
	bool	multiSelect;

	GUIStyleResourceHandle styleSheet;

	GUIRenderable	renderableTitle;
	GUIRenderable	renderableBody;

	float			titleBarSize;

	vector<CFONode*> listItemNodes;
	
};

