#include "../Geo4.h"

//#pragma comment(lib, "OpenGL32.lib")
//#pragma comment(lib, "GlU32.lib")
//#pragma comment(lib, "../Lib/GLEW/lib/glew32.lib")

//#pragma comment(lib, "../Lib/GLES_SDK/x86/lib/libGLESv2.lib")

using namespace std;

CLASS_DECLARATION(ViewportGLES2);

ViewportGLES2::ViewportGLES2() : Entity(),
	EventHandler(), 
	mPosition(0,0,0), 
	mReference(0,0,0), 
	mDistance(100), 
	mYaw(0), 
	mPitch(0), 
	mFov(90), 
	ratio(0.75), 
	mFpscam(0),
	rotate_view(0),
	translate_view(0),
	window_width(800),
	window_height(600),
	mClearColor(0,0,0,1)
{
	SetOrientation(-45,60,100);
}

ViewportGLES2::~ViewportGLES2(void)
{ }

void ViewportGLES2::Initialise(EventManager*const event_manager, SceneManager* mgr)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &ViewportGLES2::OnWindowEvent);
	event_manager->RegisterEventReceiver(this, &ViewportGLES2::OnInputEvent);
}

void ViewportGLES2::Deserialize(CFONode* node, ResourceManager* mgr)
{	
	Entity::Deserialize(node, mgr);

	string val;
	if(node->getValueString("viewmode", val)){
		if(val=="fps"){
			mFpscam = 1;
		}else{
			mFpscam = 0;
		}
	}
	
	node->getValueVector4("clear_color", mClearColor);

	if(mFpscam)
		node->getValueVector3("cam_pos", mPosition);
	else
		node->getValueVector3("cam_pos", mReference);

	if(!node->getValueFloat("fov", mFov)){
		mFov = 90.0f;
	}
	if(!node->getValueFloat("distance", mDistance)){
		mDistance = 100.0f;
	}
	if(node->getValueFloat("pitch", mPitch) && node->getValueFloat("yaw", mYaw)){
		SetOrientation(mPitch, mYaw, mDistance);
	}
}


bool ViewportGLES2::OnWindowEvent(WindowEvent*const event)
{	
	window_width = event->window->width;
	window_height = event->window->height;

	return 1;
}

bool ViewportGLES2::OnInputEvent(InputEvent*const event)
{	

	switch(event->sdl->type){
		case SDL_MOUSEBUTTONDOWN:
			if(event->sdl->button.button == SDL_BUTTON_LEFT)
				rotate_view = true;
			if (event->sdl->button.button == SDL_BUTTON_RIGHT)
				translate_view = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event->sdl->button.button == SDL_BUTTON_LEFT)
				rotate_view = false;
			if (event->sdl->button.button == SDL_BUTTON_RIGHT)
				translate_view = false;
			break;
		case SDL_MOUSEMOTION:

			Vector2 mouse = Vector2((float)event->sdl->motion.xrel, (float)event->sdl->motion.yrel);

			if(rotate_view){

				mYaw += mouse.x;
				mPitch += mouse.y;

				if(mYaw<0)mYaw = 360.0f - fabs(mYaw);
				else if(mYaw>360.0f)mYaw = mYaw - 360.0f;

				CalculatePosition();
			}
			if (translate_view) {

				mRotation.yAxis() * mouse.x;

				if (!mFpscam) {
					mReference = mReference + mRotation.yAxis() * mouse.y;
					mReference = mReference + mRotation.xAxis() * mouse.x;
				}
				else {
					mPosition = mPosition + mRotation.yAxis() * mouse.y;
					mPosition = mPosition + mRotation.xAxis() * mouse.x;
				}
				CalculatePosition();
			}
			break;
	};
	return 1;
}

void ViewportGLES2::PreRender(Renderer*)
{	
	glClearColor(mClearColor.x, mClearColor.y, mClearColor.z, mClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(0,0, window_width, window_height);
		
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	float rat = (float)window_width / (float)window_height;

	gluPerspective(mFov,rat,1,5000);
	
	CalculatePosition();
	
	gluLookAt(mPosition.x,mPosition.y,mPosition.z,   
						mReference.x, mReference.y,mReference.z,
						_mUpVector.x,_mUpVector.y,_mUpVector.z);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

}

void ViewportGLES2::Render(Renderer* rnd)
{	
	glPushMatrix();
		glColor3f(0.2f,0.2f,0.2f);

		Renderer::DrawGrid(0, 3000,7,3000);

		Renderer::DrawAxisGizmo(15);

	glPopMatrix();
}

void ViewportGLES2::PostRender()
{ 

}



void ViewportGLES2::SetOrientation(float pitch, float yaw, float faar)
{
	mPitch = pitch;
	mYaw = yaw;
	mDistance = faar;
	
	CalculatePosition();
	return;

	//calculate orientation
	Quaternion qa = Quaternion::IDENTITY;
	Quaternion qb = Quaternion::IDENTITY;

	qa.FromAngleAxis(Math::AngleUnitsToRadians(mPitch), Vector3(1,0,0));
	qb.FromAngleAxis(Math::AngleUnitsToRadians(mYaw), Vector3(0,0,1));

	mRotation = qb * qa;
	
	if(mFpscam)
		mReference = mPosition + mRotation.zAxis() * mDistance;
	else
		mPosition = mReference - mRotation.zAxis() * mDistance;

	_mUpVector = mRotation.yAxis();
}






void ViewportGLES2::CalculateFrustumPlanes()
{
	float   proj[16];
	float   modl[16];
	float   clip[16];
	float   t=0.0f;

	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	//Right
	mFrustumPlanes[0].normal.x = clip[ 3] - clip[ 0];
	mFrustumPlanes[0].normal.y = clip[ 7] - clip[ 4];
	mFrustumPlanes[0].normal.z = clip[11] - clip[ 8];
	mFrustumPlanes[0].d = clip[15] - clip[12];
	mFrustumPlanes[0].normalise();
	//Left
	mFrustumPlanes[1].normal.x = clip[ 3] + clip[ 0];
	mFrustumPlanes[1].normal.y = clip[ 7] + clip[ 4];
	mFrustumPlanes[1].normal.z = clip[11] + clip[ 8];
	mFrustumPlanes[1].d = clip[15] + clip[12];
	mFrustumPlanes[1].normalise();
	//Bottom
	mFrustumPlanes[2].normal.x = clip[ 3] + clip[ 1];
	mFrustumPlanes[2].normal.y = clip[ 7] + clip[ 5];
	mFrustumPlanes[2].normal.z = clip[11] + clip[ 9];
	mFrustumPlanes[2].d = clip[15] + clip[13];
	mFrustumPlanes[2].normalise();
	//Top
	mFrustumPlanes[3].normal.x = clip[ 3] - clip[ 1];
	mFrustumPlanes[3].normal.y = clip[ 7] - clip[ 5];
	mFrustumPlanes[3].normal.z = clip[11] - clip[ 9];
	mFrustumPlanes[3].d = clip[15] - clip[13];
	mFrustumPlanes[3].normalise();
	//Far
	mFrustumPlanes[4].normal.x  = clip[ 3] - clip[ 2];
	mFrustumPlanes[4].normal.y  = clip[ 7] - clip[ 6];
	mFrustumPlanes[4].normal.z  = clip[11] - clip[10];
	mFrustumPlanes[4].d  = clip[15] - clip[14];
	mFrustumPlanes[4].normalise();
	//Near
	mFrustumPlanes[5].normal.x = clip[ 3] + clip[ 2];
	mFrustumPlanes[5].normal.y = clip[ 7] + clip[ 6];
	mFrustumPlanes[5].normal.z= clip[11] + clip[10];
	mFrustumPlanes[5].d = clip[15] + clip[14];
}


void ViewportGLES2::DrawFrustum()
{
		glPushMatrix();
			glTranslatef(mPosition.x, mPosition.y, mPosition.z);
			glColor3f(0,0,1);
//			auxWireBox(5,5,5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(mReference.x, mReference.y, mReference.z);
			glColor3f(1,1,1);
		//	auxWireBox(5,5,5);
		glPopMatrix();
		Matrix3 m3; 
		mRotation.ToRotationMatrix(m3);
		Matrix4 m4(Matrix4::IDENTITY);
		m4 = m3;
		glPushMatrix();
			m4.setTrans(mPosition);
			m4 = m4.transpose();
			glLoadMatrixf(m4._m);
			glColor3f(0,0,1);
		///	Renderer::DrawAxisGizmo(15);
		glPopMatrix();
		
		Vector3 p1;
		//top right
		mFrustumPlanes[0].Intersects(mFrustumPlanes[3], mFrustumPlanes[4],p1);
		glColor3f(1,1,1);	
		glBegin(GL_LINES);
			glVertex3f(mPosition.x, mPosition.y, mPosition.z);
			glVertex3f(p1.x, p1.y, p1.z);
		glEnd();
		//to left
		Vector3 p2;
		mFrustumPlanes[1].Intersects(mFrustumPlanes[3], mFrustumPlanes[4],p2);
		glColor3f(1,1,1);	
		glBegin(GL_LINES);
			glVertex3f(mPosition.x, mPosition.y, mPosition.z);
			glVertex3f(p2.x, p2.y, p2.z);
		glEnd();
		//bottom right
		Vector3 p3;
		mFrustumPlanes[0].Intersects(mFrustumPlanes[2], mFrustumPlanes[4],p3);
		glColor3f(1,1,1);	
		glBegin(GL_LINES);
			glVertex3f(mPosition.x, mPosition.y, mPosition.z);
			glVertex3f(p3.x, p3.y, p3.z);
		glEnd();
		//bottom left
		Vector3 p4;
		mFrustumPlanes[1].Intersects(mFrustumPlanes[2], mFrustumPlanes[4],p4);
		glColor3f(1,1,1);	
		glBegin(GL_LINES);
			glVertex3f(mPosition.x, mPosition.y, mPosition.z);
			glVertex3f(p4.x, p4.y, p4.z);
		glEnd();
		//far rect
		glColor3f(1,1,1);	
		glBegin(GL_LINES);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p4.x, p4.y, p4.z);
			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p4.x, p4.y, p4.z);
			glVertex3f(p3.x, p3.y, p3.z);
		glEnd();
}

bool	ViewportGLES2::Intersect(AxisAlignedBox& box)
{
	if(box.intersects(mPosition))return true;

	Vector3 pos = box.getCenter();
	Vector3 size = box.getExtents();
	size *= 0.5f;

	for(unsigned int p = 0; p < 6; p++ )
	{	
		if( mFrustumPlanes[p].normal.x * (pos.x - size.x) + mFrustumPlanes[p].normal.y * (pos.y - size.y) + mFrustumPlanes[p].normal.z * (pos.z - size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		if( mFrustumPlanes[p].normal.x * (pos.x + size.x) + mFrustumPlanes[p].normal.y * (pos.y - size.y) + mFrustumPlanes[p].normal.z * (pos.z - size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		if( mFrustumPlanes[p].normal.x * (pos.x - size.x) + mFrustumPlanes[p].normal.y * (pos.y + size.y) + mFrustumPlanes[p].normal.z * (pos.z - size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		if( mFrustumPlanes[p].normal.x * (pos.x + size.x) + mFrustumPlanes[p].normal.y * (pos.y + size.y) + mFrustumPlanes[p].normal.z * (pos.z - size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		if( mFrustumPlanes[p].normal.x * (pos.x - size.x) + mFrustumPlanes[p].normal.y * (pos.y - size.y) + mFrustumPlanes[p].normal.z * (pos.z + size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		if( mFrustumPlanes[p].normal.x * (pos.x + size.x) + mFrustumPlanes[p].normal.y * (pos.y - size.y) + mFrustumPlanes[p].normal.z * (pos.z + size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		if(  mFrustumPlanes[p].normal.x * (pos.x - size.x) + mFrustumPlanes[p].normal.y * (pos.y + size.y) + mFrustumPlanes[p].normal.z * (pos.z + size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		if( mFrustumPlanes[p].normal.x * (pos.x + size.x) + mFrustumPlanes[p].normal.y * (pos.y + size.y) + mFrustumPlanes[p].normal.z * (pos.z + size.z) + mFrustumPlanes[p].d > 0 )
			continue;
		return false;
	}
	return true;
}