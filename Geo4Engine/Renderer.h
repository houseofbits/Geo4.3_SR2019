#pragma once

#include "../Lib/GLEW/include/GL/glew.h" 
#include <gl/GL.h>
#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include "Entity.h"
#include "math/HAxisAlignedBox.h"

class Camera;

//using namespace std;

class Renderer
{
public:
	Renderer(void);
	virtual ~Renderer(void);	


	virtual void BeginPerspectiveProjection() {}
	virtual void EndPerspectiveProjection() {}

	virtual void BeginOrthogonalProjection() {}
	virtual void EndOrthogonalProjection() {}


	//Variants:
	//Visas funkcijas liekam modelii, rendereris ziimee modeli, vaigjeneree predefineetus modeljus

//	virtual void DrawModel(Model, Matrix4) {}
//	virtual Model CreateModel(unsigned int model) {}	//Uztaisa predefineetu modeli (AXIS_GIZMO, GRID, ARROW, WIRE_BOX)

	//sho ieksh modelja
	//Model		- holds multiple models
	//	Mesh[]	- holds vertices
	//		mesh_type		//points, triangles, lines
	//		pos[]
	//		additional[]	//color, normal, texcoords, etc		

	virtual void BeginLine() {}
	virtual void EndLine() {}

	virtual void BeginPoly() {}
	virtual void EndPoly() {}

	virtual void CreateLine(Vector3 a, Vector3 b, Vector3 ac, Vector3 bc) {}
	virtual void CreateTriangle() {}
	virtual void CreateQuad() {}

	virtual void CreatePoint(Vector3 a, Vector3 ac) {}

	virtual void CreatePoint(Vector3 a, Vector3 ac, float size) {}


	//Materiaali???
	virtual void BeginShader() {}	//virtual class* Shader
	virtual void EndShader() {}

	//Material
	//	Shader
	//	Texture[]


	//struct SortRenderablesByZ{
		//inline bool operator()(Entity* a, Entity* b){
		//	return a->getZIndex() < b->getZIndex();
		//}
	//};
	void	AddRenderable(Entity* obj){	
	//	if(obj->isVisible() && !renderer_disabled)m_RenderableList.push_back(obj);
	}
	void	Render(){
		if(renderer_disabled)return;
		/*
		//m_RenderableList.sort(SortRenderablesByZ());

		std::list<Entity*>::iterator pos = m_RenderableList.begin();
		while(pos!=m_RenderableList.end()){
			(*pos)->PreRender(this);
			pos++;
		}
		pos = m_RenderableList.begin();
		while(pos!=m_RenderableList.end()){
			(*pos)->Render(this);
			//std::cout<<(*pos)->getTypename()<<" "<<(*pos)->getZIndex()<<std::endl;
			pos++;
		}
		pos = m_RenderableList.begin();
		while(pos!=m_RenderableList.end()){
			(*pos)->PostRender();
			pos++;
		}
		//std::cout<<"=========="<<std::endl;
		m_RenderableList.clear();

		//http://developer.apple.com/qa/qa2004/qa1158.html
		//http://www.gamedev.net/community/forums/topic.asp?topic_id=256113&whichpage=1&#1552564
		//glFlush();
		//glFinish();
		*/
	}
	static bool	renderer_disabled;

	std::list<Entity*>	m_RenderableList;

	Camera* camera;

	static	GLuint	rect_display_list;
	
	static bool	IsExtensionSupported(std::string name);

	static void	DrawCapsule(float width, float height, float r, int c1, int c2);
	static void	DrawCapsuleOutline(float width, float height, float r, float w, int c1, int c2, Vector2 deform=Vector2(0,0));

	static void DrawRect(float x1, float y1, float x2, float y2);
	static void DrawRing(float radius=1.0f, Vector2 pos = Vector2(0,0), int se=16);
	static void DrawCircle(int se, float radius, float z=0);
	static void DrawCircle(float radius, Vector2 p, int se = 16.0f, float z = 0.0f);
	static void DrawArrow(const Vector2& P, const Vector2& D, float length, int uARGB);
	static void DrawGrid(float p, float w, int d, float sz);
	static void	DrawAxisGizmo(float size);
	static void	DrawLoadScreen(std::string, bool clean=0);
	static void	DrawBox(AxisAlignedBox);

	static void	DrawCross(float w, float h, float fat);
};





