#ifndef __Sphere_H_
#define __Sphere_H_

#include "HPrerequisites.h"

#include "HVector3.h"


//##ModelId=42BA535B0196
    class Sphere
    {
    protected:
	//##ModelId=42BA535B0198
        Real mRadius;
	//##ModelId=42BA535B01A6
        Vector3 mCenter;
    public:
	//##ModelId=42BA535B01AA
        Sphere() : mRadius(1.0), mCenter(Vector3::ZERO) {}
	//##ModelId=42BA535B01AB
       Sphere(const Vector3& center, Real radius)
            : mRadius(radius), mCenter(center) {}

	//##ModelId=42BA535B01AE
        Real getRadius(void) const { return mRadius; }

	//##ModelId=42BA535B01B1
        void setRadius(Real radius) { mRadius = radius; }

	//##ModelId=42BA535B01B5
        const Vector3& getCenter(void) const { return mCenter; }

	//##ModelId=42BA535B01B8
        void setCenter(const Vector3& center) { mCenter = center; }

	//##ModelId=42BA535B01BA
		bool intersects(const Sphere& s) const
		{
			return (s.mCenter - mCenter).length() <=
				(s.mRadius + mRadius);
		}
	//##ModelId=42BA535B01BD
		bool intersects(const AxisAlignedBox& box) const
		{
			return Math::intersects(*this, box);
		}
	//##ModelId=42BA535B01C0
		bool intersects(const Plane& plane) const
		{
			return Math::intersects(*this, plane);
		}
	//##ModelId=42BA535B01C5
		bool intersects(const Vector3& v) const
		{
			return ((v - mCenter).length() <= mRadius);
		}
        

    };

#endif

