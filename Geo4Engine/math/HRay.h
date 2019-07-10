
#ifndef __Ray_H_
#define __Ray_H_

#include "HPrerequisites.h"
#include "HVector3.h"


//##ModelId=42BA535B01D4
    class Ray
    {
    protected:
	public:
	//##ModelId=42BA535B01D6
        Vector3 mOrigin;
	//##ModelId=42BA535B01E5
        Vector3 mDirection;
   
	//##ModelId=42BA535B01E9
        Ray():mOrigin(Vector3::ZERO), mDirection(Vector3::UNIT_Z) {}
	//##ModelId=42BA535B01EA
        Ray(const Vector3& origin, const Vector3& direction)
            :mOrigin(origin), mDirection(direction) {}
	//##ModelId=42BA535B01ED
        virtual ~Ray() {}

	//##ModelId=42BA535B01EF
        void setOrigin(const Vector3& origin) {mOrigin = origin;} 

	//##ModelId=42BA535B01F5
        const Vector3& getOrigin(void) const {return mOrigin;} 


	//##ModelId=42BA535B01F8
        void setDirection(const Vector3& dir) {mDirection = dir;} 
	//##ModelId=42BA535B01FA
        const Vector3& getDirection(void) const {return mDirection;} 

	//##ModelId=42BA535B01FD
		Vector3 getPoint(Real t) const { 
			return Vector3(mOrigin + (mDirection * t));
		}
		
	//##ModelId=42BA535B0200
		Vector3 operator*(Real t) const { 
			return getPoint(t);
		};

	//##ModelId=42BA535B0205
		std::pair<bool, Real> intersects(const Plane& p) const
		{
			return Math::intersects(*this, p);
		}
	//##ModelId=42BA535B0208
		std::pair<bool, Real> intersects(const Sphere& s) const
		{
			return Math::intersects(*this, s);
		}
	//##ModelId=42BA535B020B
		std::pair<bool, Real> intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}

    };

#endif
