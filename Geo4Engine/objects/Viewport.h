#pragma once

#include "../Entity.h"
#include "../Events.h"

#include "../math/HVector2.h"
#include "../math/HVector3.h"
#include "../math/HVector4.h"
#include "../math/HQuaternion.h"
#include "../math/HAxisAlignedBox.h"
#include "../math/HPlane.h"



class Viewport : public Entity, public EventHandler
{
CLASS_PROTOTYPE(Viewport);
public:
	Viewport();
	~Viewport(void);
	
	void	Initialise(EventManager*const, ResourceManager*const);
	void	Deinitialise(EventManager*const, ResourceManager*const){};

	void	Serialize(CFONode*){}
	void	Deserialize(CFONode*);

	bool	OnWindowEvent(WindowEvent*const);
	bool	OnInputEvent(InputEvent*const);

	void	PreRender(Renderer*);
	void	Render(Renderer*);
	void	PostRender();

	void		CalculatePosition(){
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

	void		SetOrientation(float pitch, float yaw, float faar);

	void		CalculateFrustumPlanes();
	void		DrawFrustum();
	bool		Intersect(AxisAlignedBox&);

	bool		mFpscam;

	float		mPitch;
	float		mYaw;
	float		mRoll;
	float		mDistance;
	float		mFov;
	Quaternion	mRotation;
	Vector3		mPosition;
	Vector3		mReference;

	Vector3		_mUpVector;

	Vector4		mClearColor;

	Plane		mFrustumPlanes[6]; 
	float		ratio;

	bool		rotate_view;
	bool		translate_view;

	int			window_width;
	int			window_height;
};

