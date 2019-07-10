
#ifndef __AxisAlignedBox_H_
#define __AxisAlignedBox_H_

#include "HPrerequisites.h"

#include "HVector3.h"
#include "HMatrix4.h"


//##ModelId=42BA535C0128
    class  AxisAlignedBox
    {
    protected:
	//##ModelId=42BA535C012A
	    Vector3 mMinimum;
	//##ModelId=42BA535C0139
	    Vector3 mMaximum;
	//##ModelId=42BA535C013D
	    bool mNull;

	//##ModelId=42BA535C013F
	    Vector3 mCorners[8];

	//##ModelId=42BA535C0143
	    void updateCorners(void)
	    {
		    mCorners[0] = mMinimum;
		    mCorners[1].x = mMinimum.x; mCorners[1].y = mMaximum.y; mCorners[1].z = mMinimum.z;
		    mCorners[2].x = mMaximum.x; mCorners[2].y = mMaximum.y; mCorners[2].z = mMinimum.z;
		    mCorners[3].x = mMaximum.x; mCorners[3].y = mMinimum.y; mCorners[3].z = mMinimum.z;            

		    mCorners[4] = mMaximum;
		    mCorners[5].x = mMinimum.x; mCorners[5].y = mMaximum.y; mCorners[5].z = mMaximum.z;
		    mCorners[6].x = mMinimum.x; mCorners[6].y = mMinimum.y; mCorners[6].z = mMaximum.z;
		    mCorners[7].x = mMaximum.x; mCorners[7].y = mMinimum.y; mCorners[7].z = mMaximum.z;            
	    }        

    public:
	//##ModelId=42BA535C0149
	    inline AxisAlignedBox()
	    {
		    setMinimum( -0.5, -0.5, -0.5 );
		    setMaximum( 0.5, 0.5, 0.5 );
		    mNull = true;
	    }

	//##ModelId=42BA535C014A
	    inline AxisAlignedBox( const Vector3& min, const Vector3& max )
	    {
		    setExtents( min, max );
			mNull = false;
	    }

	//##ModelId=42BA535C014D
	    inline AxisAlignedBox(
		    Real mx, Real my, Real mz,
		    Real Mx, Real My, Real Mz )
	    {
		    setExtents( mx, my, mz, Mx, My, Mz );
			mNull = false;
	    }

	//##ModelId=42BA535C0157
	    inline const Vector3& getMinimum(void) const
	    { 
		    return mMinimum; 
	    }

	//##ModelId=42BA535C015A
	    inline const Vector3& getMaximum(void) const
	    { 
		    return mMaximum;
	    }

	//##ModelId=42BA535C015D
	    inline void setMinimum( const Vector3& vec )
	    {
		    mNull = false;
		    mMinimum = vec;
		    updateCorners();
	    }

	//##ModelId=42BA535C015F
	    inline void setMinimum( Real x, Real y, Real z )
	    {
		    mNull = false;
		    mMinimum.x = x;
		    mMinimum.y = y;
		    mMinimum.z = z;
		    updateCorners();
	    }

	//##ModelId=42BA535C0163
	    inline void setMaximum( const Vector3& vec )
	    {
		    mNull = false;
		    mMaximum = vec;
		    updateCorners();
	    }

	//##ModelId=42BA535C0168
	    inline void setMaximum( Real x, Real y, Real z )
	    {
		    mNull = false;
		    mMaximum.x = x;
		    mMaximum.y = y;
		    mMaximum.z = z;
		    updateCorners();
	    }

	//##ModelId=42BA535C016C
	    inline void setExtents( const Vector3& min, const Vector3& max )
	    {
		    mNull = false;
		    mMinimum = min;
		    mMaximum = max;
		    updateCorners();
	    }

	//##ModelId=42BA535C016F
	    inline void setExtents(
		    Real mx, Real my, Real mz,
		    Real Mx, Real My, Real Mz )
	    {
		    mNull = false;

		    mMinimum.x = mx;
		    mMinimum.y = my;
		    mMinimum.z = mz;

		    mMaximum.x = Mx;
		    mMaximum.y = My;
		    mMaximum.z = Mz;

		    updateCorners();
	    }


	//##ModelId=42BA535C017B
	    inline const Vector3* getAllCorners(void) const
	    {
		    assert( !mNull && "Can't get corners of a null AAB" );
		    return (const Vector3*)mCorners;
	    }


	//##ModelId=42BA535C017E
	    void merge( const AxisAlignedBox& rhs )
	    {
		    if (rhs.mNull)
		    {
			    return;
		    }
		    else if (mNull)
		    {
			    setExtents(rhs.mMinimum, rhs.mMaximum);
		    }
		    else
		    {
			    Vector3 min = mMinimum;
			    Vector3 max = mMaximum;
			    max.makeCeil(rhs.mMaximum);
			    min.makeFloor(rhs.mMinimum);

			    setExtents(min, max);
		    }

	    }
		
	//##ModelId=42BA535C0180
		void merge( const Vector3& point )
		{
			if (mNull){ 
				setExtents(point, point);
			} else {
				mMaximum.makeCeil(point);
				mMinimum.makeFloor(point);
				updateCorners();
			}
		}

	//##ModelId=42BA535C0182
	    void transform( const Matrix4& matrix )
	    {
		    if( mNull )
			    return;

		    Vector3 min, max, temp;
		    bool first = true;
		    size_t i;

		    for( i = 0; i < 8; ++i )
		    {
			    temp = matrix * mCorners[i];
			    if( first || temp.x > max.x )
				    max.x = temp.x;
			    if( first || temp.y > max.y )
				    max.y = temp.y;
			    if( first || temp.z > max.z )
				    max.z = temp.z;
			    if( first || temp.x < min.x )
				    min.x = temp.x;
			    if( first || temp.y < min.y )
				    min.y = temp.y;
			    if( first || temp.z < min.z )
				    min.z = temp.z;

			    first = false;
		    }

		    setExtents( min,max );

	    }

	//##ModelId=42BA535C0187
	    inline void setNull()
	    {
		    mNull = true;
	    }

	//##ModelId=42BA535C0188
	    bool isNull(void) const
	    {
		    return mNull;
	    }

	//##ModelId=42BA535C018B
        inline bool intersects(const AxisAlignedBox& b2) const
        {
            if (this->isNull() || b2.isNull())
                return false;

            if (mMaximum.x < b2.mMinimum.x)
                return false;
            if (mMaximum.y < b2.mMinimum.y)
                return false;
            if (mMaximum.z < b2.mMinimum.z)
                return false;

            if (mMinimum.x > b2.mMaximum.x)
                return false;
            if (mMinimum.y > b2.mMaximum.y)
                return false;
            if (mMinimum.z > b2.mMaximum.z)
                return false;

            return true;

        }

	//##ModelId=42BA535C018E
        inline void scale(const Vector3& s)
        {
            mMinimum = mMinimum * s;
            mMaximum = mMaximum * s;
        }

	//##ModelId=42BA535C0190
		bool intersects(const Sphere& s) const
		{
			return Math::intersects(s, *this); 
		}
	//##ModelId=42BA535C0193
		bool intersects(const Plane& p) const
		{
			return Math::intersects(p, *this);
		}
	//##ModelId=42BA535C0198
        bool intersects(const Vector3& v) const
        {
			return(v.x >= mMinimum.x  &&  v.x <= mMaximum.x  && 
			    v.y >= mMinimum.y  &&  v.y <= mMaximum.y  && 
    			v.z >= mMinimum.z  &&  v.z <= mMaximum.z);
        }
	//##ModelId=42BA535C019B
		Vector3 getCenter(void) const
		{
			return Vector3((mMaximum + mMinimum) * 0.5);
		}

		Vector3 getExtents(void) const
		{
			return Vector3(mMaximum - mMinimum);
		}


    };

#endif
