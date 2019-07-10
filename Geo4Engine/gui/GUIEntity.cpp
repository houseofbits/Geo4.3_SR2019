#include "../Geo4.h"

#ifndef NO_OPENGL

GUIEntity::GUIEntity() : m_Visible(true),
	m_Disabled(false),
	m_ZIndex(-1.0f),
	m_WorldPos(),
	m_LocalPos(0.0f, 0.0f),
	m_Size(),
	m_Title(),
	m_Blocking(false),
	m_Focused(false)
{	}

GUIEntity::~GUIEntity()
{	}

void GUIEntity::Deserialize(CFONode* node)
{	
	Entity::Deserialize(node);
	node->getValueFloat("zindex", m_ZIndex);
	node->getValueBool("visible", m_Visible);
	node->getValueBool("disabled", m_Disabled);
	node->getValueBool("blocking", m_Blocking);
	node->getValueVector2("size", m_Size);
	node->getValueString("title", m_Title);
	if (node->getValueVector2("pos", m_LocalPos))
		m_WorldPos = getWorldSpace(m_LocalPos);
}

Vector2 GUIEntity::getWorldSpace(Vector2& v)
{
	return _getWorldSpace(v, this);
}

Vector2 GUIEntity::_getWorldSpace(Vector2& v, Entity* e)
{	
	if (e) {
		if (e->isInstanceOf("GUIEntity")) {
			GUIEntity* guie = (GUIEntity*)e;
			Vector2 parentPos = _getWorldSpace(v, e->getParent());
			//cout << guie << " - " << Utils::VectorToString(guie->m_LocalPos) <<" / "<< Utils::VectorToString(parentPos)<< endl;
			return guie->m_LocalPos + parentPos;
		}
		else {
			return _getWorldSpace(v, e->getParent());
		}
	}
	return Vector2(0,0);
}

bool GUIEntity::Clip(Vector2 p) {
	m_WorldPos = getWorldSpace(m_LocalPos);
	Vector2 lp = p - m_WorldPos;
	if (lp.x < -m_Size.x / 2 || lp.x > m_Size.x / 2)return 0;
	if (lp.y < -m_Size.y / 2 || lp.y > m_Size.y / 2)return 0;
	return 1;
}

void GUIEntity::PreRender(Renderer*) {
	glPushMatrix();
	glTranslatef(m_LocalPos.x, m_LocalPos.y, 0);
}

void GUIEntity::PostRender() {
	/*
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x0F0F);
	glLineWidth(1);

	Vector2 hs = m_Size * 0.5f;
	glColor4f(1,0,1,1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(-hs.x, -hs.y);
		glVertex2f(-hs.x, hs.y);
		glVertex2f(hs.x, hs.y);
		glVertex2f(hs.x, -hs.y);
	glEnd();

	glDisable(GL_LINE_STIPPLE);
	/**/
	glPopMatrix();
}

bool GUIEntity::isVisible()
{
	return _isVisible(this);
}

bool GUIEntity::_isVisible(Entity* e)
{
	if (e) {
		if (e->isInstanceOf("GUIEntity")) {
			GUIEntity* guie = (GUIEntity*)e;
			if (!guie->m_Visible)return false;
		}
		return _isVisible(e->getParent());
	}
	return true;
}

bool GUIEntity::isDisabled()
{	
	//TODO: recursion
	return m_Disabled; 
}

bool compare_zindices(Entity* first, Entity* second)
{
	float z_first = 0;
	float z_second = 0;

	if (first->isInstanceOf("GUIEntity")) {
		GUIEntity* guie = (GUIEntity*)first;
		z_first = guie->getZIndex();
	}
	if (second->isInstanceOf("GUIEntity")) {
		GUIEntity* guie = (GUIEntity*)second;
		z_second = guie->getZIndex();
	}

	if (z_first < z_second)return true;

	return false;
}

void GUIEntity::_SortByZIndex()
{
	m_Childs.sort(compare_zindices);

}

Entity* GUIEntity::getObjectAtPoint(Vector2& p) {
	Entity*	got_root = getRootObject();
	if (got_root) {
		Entity* obj = 0;
		_RecursiveFindObjectByPosition(got_root, obj, p);
		return obj;
	}
	return 0;
}

void	GUIEntity::_RecursiveFindObjectByPosition(Entity* e, Entity* &obj, const Vector2& p) {
	
	if (e->isInstanceOf("GUIEntity")) {
		GUIEntity* guie = (GUIEntity*)e;
		if (!guie->m_Visible || guie->m_Disabled)return;
		if (guie->Clip(p)) {
			obj = e;
		//	cout << "clip "<< guie->getName() << endl;
		}
	}
	if (!e->getChildList()->empty()) {
		std::list<Entity*>::iterator pos = e->getChildList()->begin();
		while (pos != e->getChildList()->end()) {
			//Entity is blocking event propagation to lower layers, discard previously found object
			if ((*pos)->isInstanceOf("GUIEntity")) {
				GUIEntity* guie = (GUIEntity*)(*pos);
				if (guie->m_Blocking && guie->m_Visible) {
					obj = 0;
				}
			}
			_RecursiveFindObjectByPosition((*pos), obj, p);			
			pos++;
		}
	}
}

void	GUIEntity::leaveFocus(){
	m_Focused = false;
	if (!getChildList()->empty()) {
		std::list<Entity*>::iterator pos = getChildList()->begin();
		while (pos != getChildList()->end()) {
			if ((*pos)->isInstanceOf("GUIEntity")) {
				GUIEntity* guie = (GUIEntity*)(*pos);
				guie->leaveFocus();
			}
			pos++;
		}
	}
}

GUIEntity*	GUIEntity::getFocusedObject() {	
	if (!getChildList()->empty()) {
		std::list<Entity*>::iterator pos = getChildList()->begin();
		while (pos != getChildList()->end()) {
			if ((*pos)->isInstanceOf("GUIEntity")) {
				GUIEntity* guie = (GUIEntity*)(*pos);
				GUIEntity* focused = guie->getFocusedObject();
				if (focused) {
					return focused;
				}
			}
			pos++;
		}
	}
	if (m_Focused)return this;
	return 0;
}


#endif