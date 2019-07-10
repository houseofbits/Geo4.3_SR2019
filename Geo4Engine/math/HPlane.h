
#ifndef __Plane_H__
#define __Plane_H__

#include "HPrerequisites.h"

#include "HVector3.h"

//##ModelId=42BA535B02BC
    class  Plane
    {
    public:
	//##ModelId=42BA535B02BF
        Plane ();
	//##ModelId=42BA535B02C0
        Plane (const Plane& rhs);
	//##ModelId=42BA535B02C2
        Plane (const Vector3& rkNormal, Real fConstant);
	//##ModelId=42BA535B02C5
        Plane (const Vector3& rkNormal, const Vector3& rkPoint);
	//##ModelId=42BA535B02C8
        Plane (const Vector3& rkPoint0, const Vector3& rkPoint1,
            const Vector3& rkPoint2);

	//##ModelId=42BA535B02DC
        enum Side
        {
		//##ModelId=42BA535B02DE
            NO_SIDE,
		//##ModelId=42BA535B02DF
            POSITIVE_SIDE,
		//##ModelId=42BA535B02E0
            NEGATIVE_SIDE
        };
	
		void normalise(){
			d /= normal.normalise();	
		}

		bool Intersects(const Plane &other1, const Plane &other2, Vector3 &pt) const
		{
			float denom = normal.crossProduct(other1.normal).dotProduct(other2.normal);
			if( 0.0 == denom )
			{
				return false;
			}
			pt = ( 
				-(d * other1.normal.crossProduct(other2.normal)) - 
				(other1.d * other2.normal.crossProduct(normal)) - 
			(other2.d * normal.crossProduct(other1.normal))
			)/denom;

			return true;
		}


	//##ModelId=42BA535B02CC
        Side getSide (const Vector3& rkPoint) const;

	//##ModelId=42BA535B02CF
        Real getDistance (const Vector3& rkPoint) const;

	//##ModelId=42BA535B02D3
        Vector3 normal;
	//##ModelId=42BA535B02D8
        Real d;

    };


#endif
