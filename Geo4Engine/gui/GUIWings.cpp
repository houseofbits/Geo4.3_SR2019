#include "../Geo4.h"

#ifndef NO_OPENGL

CLASS_DECLARATION(GUIWings);

#define S1_ID 0
#define S2_ID 1
#define A1L_ID 2
#define A1R_ID 3
#define S3_ID 4
#define A2L_ID 5
#define A2R_ID 6

GUIWings::GUIWings() :
	selectedWing(-1),
	selectedSegment(-1),
	selection(),
	fontHandle()
{	}

GUIWings::~GUIWings()
{	}

void GUIWings::Initialise(EventManager*const eventManager, ResourceManager*const resourceManager)
{
	eventManager->RegisterEventHandler(this);

	eventManager->RegisterEventReceiver(this, &GUIWings::OnGUIInputEvent);

	resourceManager->Get(fontHandle, "fonts/exo1/Exo-SemiBold.ttf");

	for (unsigned int i = 0; i < 12; i++) {
		wings[i] = new Wing(700, (float)i*30.0f);
	}
}

void GUIWings::Deserialize(CFONode* node)
{
	GUIEntity::Deserialize(node);	
}

bool GUIWings::OnGUIInputEvent(GUIInputEvent*const event)
{
	Vector2 mp;
	int selecteds = -1;

	if (isVisible() == 0)return 1;

	switch (event->type) {
	case GUIInputEvent::EventType::MOUSEUP:		
		mp = event->mousePosition - getWorldSpace(this->m_LocalPos);
		for (unsigned int i = 0; i < 12; i++) {
			selecteds = wings[i]->clip(mp);
			if (selecteds >= 0) {
				selectedWing = i;
				selectedSegment = selecteds;
				break;
			}
		}

		if (!event->modKeyShift) {

			clearSelection();

		}

		addSelection();

		//cout << Utils::VectorToString(mp) << endl;

		break;
	};
	return 1;
}

void GUIWings::PreRender(Renderer* r){
	GUIEntity::PreRender(r);
}

void GUIWings::Render(Renderer* rnd){

	glPushMatrix();

	for (unsigned int i = 0; i < 12; i++) {
		wings[i]->draw(selection[i]);
	}
	for (unsigned int i = 0; i < 12; i++) {

		glPushMatrix();

		glRotatef(wings[i]->angle, 0, 0, 1);

		glTranslatef(0,-40, 0);
		glColor4f(1,1,1,0.5f);

		fontHandle->Draw(Utils::IntToString(i), 8);

		glPopMatrix();

	}

	glPopMatrix();

}

void GUIWings::PostRender(){
	GUIEntity::PostRender();
}

#endif

Wing::Wing(float s, float a) {
	angle = a;
	scale = s;
	//S1
	quads[S1_ID][0] = Vector2(scale * 0.008f, scale * 0.035f);
	quads[S1_ID][1] = Vector2(scale * -0.008f, scale * 0.035f);
	quads[S1_ID][2] = Vector2(scale * -0.014f, scale * 0.13f);
	quads[S1_ID][3] = Vector2(scale * 0.014f, scale * 0.13f);
	//S2
	quads[S2_ID][0] = Vector2(scale * 0.014f, scale * 0.135f);
	quads[S2_ID][1] = Vector2(scale * -0.014f, scale * 0.135f);
	quads[S2_ID][2] = Vector2(scale * -0.019f, scale * 0.238f);
	quads[S2_ID][3] = Vector2(scale * 0.019f, scale * 0.238f);
	//S3
	quads[S3_ID][0] = Vector2(scale * 0.019f, scale * 0.243f);
	quads[S3_ID][1] = Vector2(scale * -0.019f, scale * 0.243f);
	quads[S3_ID][2] = Vector2(scale * -0.013f, scale * 0.491f);
	quads[S3_ID][3] = Vector2(scale * 0.013f, scale * 0.491f);
	//A1
	quads[A1R_ID][0] = Vector2(scale * 0.022f, scale * 0.243f);
	quads[A1R_ID][1] = Vector2(scale * 0.022f, scale * 0.273f);
	quads[A1R_ID][2] = Vector2(scale * 0.097f, scale * 0.402f);
	quads[A1R_ID][3] = Vector2(scale * 0.104f, scale * 0.4f);
	//A1 L
	quads[A1L_ID][0] = Vector2(scale * -0.022f, scale * 0.243f);
	quads[A1L_ID][1] = Vector2(scale * -0.022f, scale * 0.273f);
	quads[A1L_ID][2] = Vector2(scale * -0.097f, scale * 0.402f);
	quads[A1L_ID][3] = Vector2(scale * -0.104f, scale * 0.4f);
	//A2
	quads[A2R_ID][0] = Vector2(scale * 0.021f, scale * 0.19f);
	quads[A2R_ID][1] = Vector2(scale * 0.022f, scale * 0.237f);
	quads[A2R_ID][2] = Vector2(scale * 0.04f, scale * 0.149f);
	quads[A2R_ID][3] = Vector2(scale * 0.033f, scale * 0.151f);
	//A2 L
	quads[A2L_ID][0] = Vector2(scale * -0.021f, scale * 0.19f);
	quads[A2L_ID][1] = Vector2(scale * -0.022f, scale * 0.237f);
	quads[A2L_ID][2] = Vector2(scale * -0.04f, scale * 0.149f);
	quads[A2L_ID][3] = Vector2(scale * -0.033f, scale * 0.151f);
}

void Wing::setColors(Vector3 c[7])
{
	memcpy_s(colors, sizeof(Vector3) * 7, c, sizeof(Vector3) * 7);
}

void Wing::draw(bool selected[7])
{
	glPushMatrix();
	glRotatef(angle, 0, 0, 1);
	for (unsigned int i = 0; i < 7; i++) {
		glColor3f(colors[i].x, colors[i].y, colors[i].z);
		glBegin(GL_QUADS);
		for (unsigned int v = 0; v < 4; v++) {
			glVertex2f(quads[i][v].x, quads[i][v].y);
		}
		glEnd();

		if (selected[i]) {
//			glColor3f(1, 0, 0);
			glColor3f(1.0f - colors[i].x, 1.0f - colors[i].y, 1.0f - colors[i].z);
			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
			for (unsigned int v = 0; v < 4; v++) {
				glVertex2f(quads[i][v].x, quads[i][v].y);
			}
			glEnd();
		}
	}
	glPopMatrix();
}

int	Wing::clip(Vector2 v) {

	Matrix2 m(Math::DegreesToRadians(angle));

	m = m.Transpose();

	v = v / scale;

	Vector2 vp = v * m;

	Vector2 a = Math::VectorFrom(Math::DegreesToRadians(105)) * 0.5f;
	Vector2 b = Math::VectorFrom(Math::DegreesToRadians(75)) * 0.5f;

	if (!Math::PointInsideTriangle(vp, a, b, Vector2(0, 0)))return -1;

	//S1
	if (vp.y < 0.13f) {
		if (vp.x > -0.015f && vp.x < 0.015f) {
			return S1_ID;
		}
	}
	//s2 / a2
	else if (vp.y > 0.13f && vp.y < 0.238f) {
		if (vp.x > -0.019f && vp.x < 0.019f) {
			return S2_ID;
		}
		else if (vp.x < -0.019f) {
			return A2L_ID;
		}
		else if (vp.x > 0.019f) {
			return A2R_ID;
		}
	}
	//s3 / a1
	else if (vp.y > 0.238f) {
		if (vp.x > -0.019f && vp.x < 0.019f) {
			return S3_ID;
		}
		else if (vp.x < -0.019f) {
			return A1L_ID;
		}
		else if (vp.x > 0.019f) {
			return A1R_ID;
		}
	}

	return -1;
}

void GUIWings::setColorToSelected(Vector3 color)
{
	for (int i = 0; i < 12; i++) {
		for (int a = 0; a < 7; a++) {
			if (selection[i][a]) {
				wings[i]->colors[a] = color;
			}
		}
	}
}

void GUIWings::clearSelection()
{
	for (int i = 0; i < 12; i++)
		for (int a = 0; a < 7; a++)
			selection[i][a] = false;
}

void GUIWings::addSelection()
{
	if (selectedWing >= 0 && selectedSegment >= 0) {
		selection[selectedWing][selectedSegment] = !selection[selectedWing][selectedSegment];
	}
}
void GUIWings::selectAll() {
	for (int i = 0; i < 12; i++)
		for (int a = 0; a < 7; a++)
			selection[i][a] = true;
}
void GUIWings::selectWing() {
	vector<int> sel;
	for (int i = 0; i < 12; i++)
		for (int a = 0; a < 7; a++)
			if (selection[i][a])sel.push_back(i);

	for (int i = 0; i < sel.size(); i++) {
		for (int a = 0; a < 7; a++)
			selection[sel[i]][a] = true;
	}
}
void GUIWings::selectRing() {
	vector<int> sel;
	for (int i = 0; i < 12; i++)
		for (int a = 0; a < 7; a++)
			if (selection[i][a])sel.push_back(a);

	for (int i = 0; i < sel.size(); i++) {
		for (int a = 0; a < 12; a++)
			selection[a][sel[i]] = true;
	}
}
void GUIWings::selectRingOdd() {
	vector<pair<int,int>> sel;
	for (int i = 0; i < 12; i++)
		for (int a = 0; a < 7; a++)
			if (selection[i][a])sel.push_back(pair<int,int>(i,a));

	for (int i = 0; i < sel.size(); i++) {
		for (int a = 0; a < 12; a += 2) {
			int idx = (a + sel[i].first) % 12;
			selection[idx][sel[i].second] = true;
		}
	}
}
void GUIWings::applyKeyframe(WingsKeyframe& keyframe)
{
	for (int i = 0; i < 12; i++) {
		wings[i]->setColors(keyframe.colors[i]);
	}
}

WingsKeyframe WingsKeyframe::getInterpolated(float td, WingsKeyframe& other)
{
	WingsKeyframe k;
	for (int i = 0; i < 12; i++) {
		for (int a = 0; a < 7; a++) {
			k.colors[i][a] = colors[i][a].getInterpolated(other.colors[i][a], td);
		}
	}
	k.t = t + td;
	return k;
}

WingsKeyframe GUIWings::createKeyframe(float t)
{
	WingsKeyframe k;
	for (int i = 0; i < 12; i++) {
		memcpy_s(k.colors[i], sizeof(Vector3) * 7, wings[i]->colors, sizeof(Vector3) * 7);
	}
	k.t = t;
	return k;
}