#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUIGraph);

GUIGraph::GUIGraph() : styleSheet(),
renderable(),
gridStep(10, 0.01f),
gridDivideStep(50, 0.05f),
gridReference(500, 1.75f),
xLimits(0, 1000),
yLimits(1, 2),
graphs(),
autoScaleY(false),
showYValues(false)
{	}

GUIGraph::~GUIGraph()
{	}

void GUIGraph::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	resourceManager->Get(styleSheet, "style/styles.cfo");

	eventManager->RegisterEventHandler(this);
	eventManager->RegisterEventReceiver(this, &GUIGraph::OnGUIInputEvent);
	eventManager->RegisterEventReceiver(this, &GUIGraph::OnWindowEvent);

	resourceManager->Get(styleSheet, "style/styles.cfo");

	renderable.style = &styleSheet->get(styleName);

	renderable.update();

	renderable.setTextWithWrapping(m_Title);
}

void GUIGraph::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);

	styleName = "graph";

	node->getValueString("style", styleName);

	node->getValueVector2("gridStep", gridStep);
	node->getValueVector2("gridDivideStep", gridDivideStep);
	node->getValueVector2("gridReference", gridReference);
	node->getValueVector2("xLimits", xLimits);
	node->getValueVector2("yLimits", yLimits);

	node->getValueBool("autoScaleY", autoScaleY);
	node->getValueBool("showYValues", showYValues);	

	renderable.size = m_Size;

	for (unsigned int i = 1; i < 6; i++) {		
		string name = "Graph" + Utils::IntToString(i);
		CFONode* g = node->GetFirstChild(name);
		if (g) {
			string gname = "";
			g->getValueString("name", gname);
			unsigned int gid = createGraph(gname);

			//Random values
			float h = yLimits.y - yLimits.x;
			for (unsigned int i = 0; i < 500; i++) {
				addGraphValue(gid, new GraphBaseValue(Math::RangeRandom(yLimits.x + (h*0.3f), yLimits.y - (h*0.3f))));
			}
		}
	}

	if(autoScaleY)ScaleY();
}

bool GUIGraph::OnWindowEvent(WindowEvent*const event)
{
	if (event->eventType == WindowEvent::WINDOW_CREATED) {
		renderable.update();
	}
	return 1;
}

bool GUIGraph::OnGUIInputEvent(GUIInputEvent*const event)
{
	if (isVisible() == 0)return 1;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEUP:

		break;
	case GUIInputEvent::EventType::MOUSEDOWN:

		break;
	case GUIInputEvent::EventType::MOUSEENTER:

		break;
	case GUIInputEvent::EventType::MOUSELEAVE:

		break;
	};
	return 1;
}

void GUIGraph::PreRender(Renderer* r) {
	GUIEntity::PreRender(r);
}

void GUIGraph::Render(Renderer* rnd){

	renderable.Draw();
	
	Vector2 hs = m_Size * 0.5f;
	Vector2	_limits(xLimits.y - xLimits.x, yLimits.y - yLimits.x);
	Vector2	_scale(m_Size.x / _limits.x, m_Size.y / _limits.y);

	glColor4fv(renderable.style->graphLineColor.val);

	if (renderable.style->graphLineStyle > 0) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(2, renderable.style->graphLineStyle);
	}
	glBegin(GL_LINES);
	for (float x = xLimits.x; x < xLimits.y; x += gridStep.x) {
		float wx = -hs.x + ((x - xLimits.x) * _scale.x);
		glVertex2f(wx, -hs.y);
		glVertex2f(wx, hs.y);
	}
	for (float y = yLimits.x; y < yLimits.y; y += gridStep.y) {
		float wy = -hs.y + ((y - yLimits.x) * _scale.y);
		glVertex2f(-hs.x, wy);
		glVertex2f(hs.x, wy);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glColor4fv(renderable.style->graphDividerLineColor.val);

	if (renderable.style->graphDividerLineStyle > 0) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(2, renderable.style->graphDividerLineStyle);
	}
	glBegin(GL_LINES);
	for (float x = xLimits.x; x < xLimits.y; x += gridDivideStep.x) {
		float wx = -hs.x + ((x - xLimits.x) * _scale.x);
		glVertex2f(wx, -hs.y);
		glVertex2f(wx, hs.y);
	}
	for (float y = yLimits.x; y < yLimits.y; y += gridDivideStep.y) {
		float wy = -hs.y + ((y - yLimits.x) * _scale.y);
		glVertex2f(-hs.x, wy);
		glVertex2f(hs.x, wy);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glColor4fv(renderable.style->graphRefLineColor.val);

	if (renderable.style->graphRefLineStyle > 0) {
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(2, renderable.style->graphRefLineStyle);
	}
	glBegin(GL_LINES);
		float wy = -hs.y + ((gridReference.y - yLimits.x) * _scale.y);
		glVertex2f(-hs.x, wy);
		glVertex2f(hs.x, wy);

		float wx = -hs.x + ((gridReference.x - xLimits.x) * _scale.x);
		glVertex2f(wx, -hs.y);
		glVertex2f(wx, hs.y);
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	//Text
	if (showYValues) {
		wx = -hs.x + ((gridReference.x - xLimits.x) * _scale.x);
		for (float y = yLimits.x; y <= yLimits.y; y += gridDivideStep.y) {
			float wy = -hs.y + ((y - yLimits.x) * _scale.y);
			renderable.DrawStaticText(Utils::FloatToString(y, 4), Vector2(wx + 3, wy + 3));
		}
	}

	//Graph values
	unsigned int valuesDisplayCount = 100;
	float step = m_Size.x / valuesDisplayCount;
	Vector4 graphColor;
	float labelOffset = 5;

	for (unsigned int i = 0; i < graphs.size(); i++) {
		switch (i) {
		case 0: graphColor = renderable.style->graphDataLineColor;	break;
		case 1: graphColor = renderable.style->graphDataLineColor2;	break;
		case 2: graphColor = renderable.style->graphDataLineColor3;	break;
		case 3: graphColor = renderable.style->graphDataLineColor4;	break;
		case 4: graphColor = renderable.style->graphDataLineColor5;	break;
		default: graphColor = renderable.style->graphDataLineColor;
		};
		
		glColor4fv(graphColor.val);

		std::deque<GraphBaseValue*>::iterator it = graphs[i].values.begin();
		float x = -hs.x;
		if (renderable.style->graphDataLineStyle > 0) {
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(2, renderable.style->graphDataLineStyle);
		}
		glBegin(GL_LINE_STRIP);
		unsigned int cnt = 0;
		while (it != graphs[i].values.end()) {
			GraphBaseValue* v = (*it);
			float wy = -hs.y + ((v->value - yLimits.x) * _scale.y);
			glVertex2f(x, wy);
			x += step;
			cnt++;
			*it++;
			if (cnt > valuesDisplayCount)break;
		}
		glEnd();
		glDisable(GL_LINE_STIPPLE);
		
		renderable.DrawStaticText(graphs[i].name, Vector2(0, labelOffset), graphColor, 
			GUIStyle::TextJusify::LEFT, 
			GUIStyle::TextVerticalAlign::BOTTOM,3,3,3,3);

		labelOffset += renderable.style->lineHeight;
	}
}

void GUIGraph::PostRender() {
	GUIEntity::PostRender();
}

void GUIGraph::addGraphValue(unsigned int index, GraphBaseValue* val) {
	if (index < graphs.size()) {
		graphs[index].values.push_back(val);
	}
}

void GUIGraph::ScaleY() {
	Vector2 minmax(1000000, -1000000);
	for (unsigned int i = 0; i < graphs.size(); i++) {
		std::deque<GraphBaseValue*>::iterator it = graphs[i].values.begin();
		while (it != graphs[i].values.end()) {
			if ((*it)->value > minmax.y)minmax.y = (*it)->value;
			if ((*it)->value < minmax.x)minmax.x = (*it)->value;
			*it++;
		}
	}
	yLimits = minmax;
	yLimits.x = minmax.x - ((minmax.y - minmax.x) * 0.5f);
	yLimits.y = minmax.y + ((minmax.y - minmax.x) * 0.5f);
}


#endif