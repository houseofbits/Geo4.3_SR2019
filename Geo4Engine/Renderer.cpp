#include "Geo4.h"

#ifndef NO_OPENGL

#include "math/HVector2.h"
#include "math/HMath.h"
#include "math/HColor.h"
  
//#include "Font.h"  
#include "StringUtils.h"  
//#include "SDLWindow.h"  

//#pragma comment(lib, "glaux.lib")

GLuint Renderer::rect_display_list = 0;
bool	Renderer::renderer_disabled = 0;

Renderer::Renderer(void):camera(0)
{ }

Renderer::~Renderer(void)
{ }

static GLfloat g_rect_vertices[] = {0,1, 0,1, 1,0, 1,0};
static GLfloat g_rect_texcoords[] = {0,1, 0,0, 1,0, 1,1};

bool Renderer::IsExtensionSupported(string szTargetExtension )
{
	if(Renderer::renderer_disabled)return 1;

	const unsigned char *pszExtensions = NULL;
	const unsigned char *pszStart;
	unsigned char *pszWhere, *pszTerminator;

	// Extension names should not have spaces
	pszWhere = (unsigned char *) strchr( szTargetExtension.c_str(), ' ' );
	if( pszWhere || *szTargetExtension.c_str() == '\0' )
		return false;

	// Get Extensions String
	pszExtensions = glGetString( GL_EXTENSIONS );

	// Search The Extensions String For An Exact Copy
	pszStart = pszExtensions;
	for(;;)
	{
		pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension.c_str());
		if( !pszWhere )
			break;
		pszTerminator = pszWhere + strlen( szTargetExtension.c_str());
		if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
			if( *pszTerminator == ' ' || *pszTerminator == '\0' )
				return true;
		pszStart = pszTerminator;
	}
	cout<<"!OpenGL extension '"<<szTargetExtension<<"' not supported"<<endl;
	return false;
}


void Renderer::DrawRect(float x1, float y1, float x2, float y2)
{
//	if(Renderer::renderer_disabled)return;

	glEnableClientState( GL_VERTEX_ARRAY );					
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );		

	g_rect_vertices[0] = g_rect_vertices[2] = x1; 
	g_rect_vertices[1] = g_rect_vertices[7] = y1;
	g_rect_vertices[3] = g_rect_vertices[5] = y2;
	g_rect_vertices[4] = g_rect_vertices[6] = x2; 

	glVertexPointer(2, GL_FLOAT, 0, g_rect_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, g_rect_texcoords);
	glDrawArrays(GL_QUADS,0,4);

	glDisableClientState( GL_VERTEX_ARRAY );				
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );		

	/*
	if(glIsList(rect_display_list)){
		Vector2 p(x1+(x2-x1)/2, y1+(y2-y1)/2);
		Vector2 s((x2-x1), (y2-y1));
		glPushMatrix();
			glTranslatef(p.x, p.y, 0);
			glScalef(s.x, s.y, 1);
			glCallList(rect_display_list);
		glPopMatrix();
	}else{
		rect_display_list = glGenLists(1);	
		glNewList(rect_display_list,GL_COMPILE);	
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex2f(-0.5,0.5);

				glTexCoord2f(1,0);
				glVertex2f(0.5,0.5);

				glTexCoord2f(1,1);
				glVertex2f(0.5,-0.5);
				
				glTexCoord2f(0,1);
				glVertex2f(-0.5,-0.5);
			glEnd();
		glEndList();	
	}
/**			glBegin(GL_QUADS);
				glTexCoord2f(0,1);
				glVertex2f(x1,y1);

				glTexCoord2f(1,1);
				glVertex2f(x2,y1);

				glTexCoord2f(1,0);
				glVertex2f(x2,y2);
				
				glTexCoord2f(0,0);
				glVertex2f(x1,y2);
			glEnd();
			/**/
}

void	 Renderer::DrawCapsule(float width, float height, float r, int c1, int c2)
{	
	if(r*2 > width){
		r = width/2 - width/10;
	}
	if(r*2 > height){
		r = height/2 - height/10;
	}
	float step = 90.0f/5;
	glBegin(GL_TRIANGLE_STRIP);
	for(float u=90.0f; u>=0.0; u-=step){
		float x1 = -(width/2 - r) + (r * -(cos(u * Math::PI/180.0f)));
		float x2 = (width/2 - r) + (r * (cos(u * Math::PI/180.0f)));		
		float y = (height/2 - r) + (r * (sin(u * Math::PI/180.0f)));
		int c = Color::Interpolate(c1, c2, (y+(height/2))/height);
		Color::GLColor(c);
		glVertex2f(x1, y);
		glVertex2f(x2, y);
	}
	for(float u=0.0; u<=90.0; u+=step){
		float x1 = -(width/2 - r) + (r * -(cos(u * Math::PI/180.0f)));
		float x2 = (width/2 - r) + (r * (cos(u * Math::PI/180.0f)));		
		float y = -(height/2 - r) - (r * (sin(u * Math::PI/180.0f)));
		int c = Color::Interpolate(c1, c2, (y+(height/2))/height);
		Color::GLColor(c);
		glVertex2f(x1, y);
		glVertex2f(x2, y);
	}
	glEnd();

}

void	 Renderer::DrawCapsuleOutline(float width, float height, float r, float ow, int c1, int c2, Vector2 d)
{	
	if(r*2 > width){
		r = width/2 - width/10;
	}
	if(r*2 > height){
		r = height/2 - height/10;
	}
	glBegin(GL_TRIANGLE_STRIP);
	float step = 90.0f/5;
	Vector2 p, p2, v;
	Vector2 s[2];
	bool ss=false;
	for(unsigned int i=0; i<4; i++){
		for(float u=0.0f; u<=90.0; u+=step){
			if(i==0){
				v = Vector2(-cos(u * Math::PI/180.0f), sin(u * Math::PI/180.0f));			
				p = Vector2(	-(width/2 - r) + (r * v.x),
							(height/2 - r) + (r * v.y)	);
			}else if(i==1){
				v = Vector2(cos((90-u) * Math::PI/180.0f), sin((90-u) * Math::PI/180.0f));			
				p = Vector2(	(width/2 - r) + (r * v.x),
							(height/2 - r) + (r * v.y)	);			
			}else if(i==2){
				v = Vector2(cos(u * Math::PI/180.0f), -sin(u * Math::PI/180.0f));			
				p = Vector2(	(width/2 - r) + (r * v.x),
							-(height/2 - r) + (r * v.y)	);			
			}else if(i==3){
				v = Vector2(-cos((90-u) * Math::PI/180.0f), -sin((90-u) * Math::PI/180.0f));			
				p = Vector2(	-(width/2 - r) + (r * v.x),
							-(height/2 - r) + (r * v.y)	);						
			}			
			p2 = Vector2(p + v * ow) + d;
			
			if(!ss){
				s[0] = p;
				s[1] = p2;
				ss = true;
			}
			Color::GLColor(c1);
			glVertex2f(p.x, p.y);

			Color::GLColor(c2);
			glVertex2f(p2.x, p2.y);

		//	cout<<p.x<<", "<<p.y<<" / "<<p2.x<<", "<<p2.y<<endl;
		}
	//	cout<<"==="<<endl;
//		return;
	}
	Color::GLColor(c1);
	glVertex2f(s[0].x, s[0].y);

	Color::GLColor(c2);
	glVertex2f(s[1].x, s[1].y);

	glEnd();

	/*glBegin(GL_TRIANGLE_STRIP);
	for(float u=90.0f; u>=0.0; u-=step){
		float x1 = -(width/2 - r) + (r * -(cos(u * Math::PI/180.0f)));
		float x2 = (width/2 - r) + (r * (cos(u * Math::PI/180.0f)));		
		float y = (height/2 - r) + (r * (sin(u * Math::PI/180.0f)));
		int c = Color::Interpolate(c1, c2, (y+(height/2))/height);
		Color::GLColor(c);
		glVertex2f(x1, y);
		glVertex2f(x2, y);
	}
	for(float u=0.0; u<=90.0; u+=step){
		float x1 = -(width/2 - r) + (r * -(cos(u * Math::PI/180.0f)));
		float x2 = (width/2 - r) + (r * (cos(u * Math::PI/180.0f)));		
		float y = -(height/2 - r) - (r * (sin(u * Math::PI/180.0f)));
		int c = Color::Interpolate(c1, c2, (y+(height/2))/height);
		Color::GLColor(c);
		glVertex2f(x1, y);
		glVertex2f(x2, y);
	}
	glEnd();*/

}

void Renderer::DrawRing(float radius, Vector2 pos, int se)
{
	if(Renderer::renderer_disabled)return;

glPushMatrix();
glTranslatef(pos.x, pos.y, 0);
  glBegin(GL_LINE_STRIP);
    float step = 360.0f/se;
    for (float n=0;n <= 360.0f; n+=step)
      glVertex2f(radius*cos(n*Math::PI/180.0f), radius*sin(n*Math::PI/180.0f));
      
      glVertex2d(radius,0);
  glEnd();
  glPopMatrix();
}


void Renderer::DrawCircle(float radius,Vector2 p, int se, float z)
{
	if(Renderer::renderer_disabled)return;

  glBegin(GL_POLYGON);
    float step = 360.0f/se;
    for (float n=0;n <= 360.0f; n+=step)
      glVertex3f(p.x + radius*cos(n*Math::PI/180.0f), p.y + radius*sin(n*Math::PI/180.0f), z);
      
      glVertex3d(p.x + radius,p.y, z);
  glEnd();
}

void Renderer::DrawCircle(int se, float radius, float z)
{
if(Renderer::renderer_disabled)return;

  glBegin(GL_POLYGON);
    float step = 360.0f/se;
    for (float n=0;n <= 360.0f; n+=step)
      glVertex3f(radius*cos(n*Math::PI/180.0f), radius*sin(n*Math::PI/180.0f), z);
      
      glVertex3d(radius,0, z);
  glEnd();
}

void Renderer::DrawArrow(const Vector2& P, const Vector2& D, float length, int uARGB)
{
	if(Renderer::renderer_disabled)return;

	float fAngle = atan2(D.y, D.x);

	glPushMatrix();
	glTranslatef(P.x, P.y, 0.0f);
	glRotatef(Math::RadiansToDegrees(fAngle), 0.0f, 0.0f, 1.0f);
	glScalef(length, length, 1.0f);

	glColor4ub(Color::R(uARGB), Color::G(uARGB), Color::B(uARGB), Color::A(uARGB));

	glBegin(GL_LINES);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(1.0f, 0.0f);

		glVertex2f(1.0f, 0.0f);
		glVertex2f(0.75f, 0.2f);

		glVertex2f(1.0f, 0.0f);
		glVertex2f(0.75f,-0.2f);
	glEnd();

	glPopMatrix();
}


//position, width, depth, size
void Renderer::DrawGrid(float p, float w, int d, float sz)
{
	if(Renderer::renderer_disabled)return;

	float l = sz;
	//y direction
	if(p==0)
	{
	glLineWidth(4);
	}
	else if(d-1==0)
	{
	glLineWidth(2);
	}
	else
	{
	glLineWidth(1);
	}

	glBegin(GL_LINES);
		glVertex2f(p, l/2);
		glVertex2f(p, -l/2);
	glEnd();
	glBegin(GL_LINES);
		glVertex2f(l/2, p);
		glVertex2f(-l/2, p);
	glEnd();
	if(d>0)
	{
	//front of p
	DrawGrid(p+(w/4), w/2, d-1, sz);
	//back of p
	DrawGrid(p-(w/4), w/2, d-1, sz);
	}
}

void Renderer::DrawAxisGizmo(float size)
{
if(Renderer::renderer_disabled)return;

  glDisable(GL_TEXTURE_2D);     
glPushMatrix();
	
glColor3f(1,0,0);
//vector x
glPushMatrix();
	glTranslatef(size, 0, 0);
	glRotatef(90,0,1,0);
		//auxSolidCone(0.1*size,0.5*size);
glPopMatrix();

glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(size, 0, 0);
glEnd();

glColor3f(0,1,0);
//vector y
glPushMatrix();
	glTranslatef(0, size, 0);
	glRotatef(-90,1,0,0);
		//auxSolidCone(0.1*size,0.5*size);
glPopMatrix();

glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, size, 0);
glEnd();

glColor3f(0,0,1);
//vector z
glPushMatrix();
	glTranslatef(0, 0, size);
	glRotatef(90,0,0,1);
//		auxSolidCone(0.1*size,0.5*size);
glPopMatrix();

glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, size);
glEnd();

glPopMatrix();
}


void Renderer::DrawLoadScreen(string str, bool clean)
{
	return
	//if(Renderer::renderer_disabled)return;

	//if(clean){
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(0,SDLWindow::m_fViewportWidth, SDLWindow::m_fViewportHeight,0,1,0);
	glMatrixMode(GL_MODELVIEW);
	
	float width = 800;//SDLWindow::m_fViewportWidth * 0.8f;
	float height = 600;//SDLWindow::m_fViewportHeight * 0.04f;

	glDisable(GL_DEPTH_TEST);
	
	glDisable(GL_TEXTURE_2D);
	glColor4f(0,0,0.3f, 1);
//	Renderer::DrawRect(SDLWindow::m_fViewportWidth/2 - width/2,SDLWindow::m_fViewportHeight/2 - height/2, SDLWindow::m_fViewportWidth/2 + width/2,SDLWindow::m_fViewportHeight/2 + height/2);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1,1,1);
//	Renderer::DrawRect(SDLWindow::m_fViewportWidth/2 - width/2,SDLWindow::m_fViewportHeight/2 - height/2, SDLWindow::m_fViewportWidth/2 + width/2,SDLWindow::m_fViewportHeight/2 + height/2);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		

	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	Font::m_DefaultInstance.Draw(str, 0.5f, Vector2(200, -230));	
//	Font::m_DefaultInstance.Draw(str, 0.5f, Vector2(SDLWindow::m_fViewportWidth/2 - width/2.5, -SDLWindow::m_fViewportHeight/2 + height/2));	

//	SDL_GL_SwapBuffers();
}

void Renderer::DrawBox(AxisAlignedBox box)
{
if(Renderer::renderer_disabled)return;

glPushMatrix();
	Vector3 p = box.getCenter();
	Vector3 s = box.getExtents();
	glTranslatef(p.x, p.y, p.z);
//	auxWireBox(s.x, s.y, s.z);
glPopMatrix();
}

void Renderer::DrawCross(float w, float h, float fat)
{
	glBegin(GL_QUADS);	
		glVertex2f(-w/2, h*fat / 2);
		glVertex2f(w/2, h*fat / 2);
		glVertex2f(w/2, -h*fat / 2);
		glVertex2f(-w/2,-h*fat / 2);
	glEnd();
	glBegin(GL_QUADS);	
		glVertex2f(-w*fat / 2, h/2);
		glVertex2f(w*fat / 2, h/2);
		glVertex2f(w*fat / 2, h*fat  / 2);
		glVertex2f(-w*fat / 2, h*fat / 2);
	glEnd();
	glBegin(GL_QUADS);	
		glVertex2f(-w*fat / 2, -h/2);
		glVertex2f(w*fat / 2, -h/2);
		glVertex2f(w*fat / 2, -h*fat  / 2);
		glVertex2f(-w*fat / 2, -h*fat / 2);
	glEnd();
}

#endif
