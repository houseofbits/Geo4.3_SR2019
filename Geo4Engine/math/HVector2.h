
#ifndef __Vector2_H__
#define __Vector2_H__

#include "HPrerequisites.h"
//#include "HMath.h"
#include "HMatrix2.h"


//##ModelId=42BA535B0148
class Vector2
{
public:
        union {
            struct {
                Real x, y;        
            };
            Real val[2];
        };

    inline bool isZeroLength(void) const
    {
        Real sqlen = (x * x) + (y * y);
        return (sqlen < (1e-06 * 1e-06));

    }

	inline Real& operator [] ( unsigned i )
    {
        assert( i < 2 );

        return *(&x+i);
    }

	//##ModelId=42BA535B0149
	inline Vector2( void ) : x(Real(0)), y(Real(0))
	{
	}

	Vector2 Direction(void) const
	{
		Vector2 Temp(*this);

		Temp.normalise();

		return Temp;
	}
	//##ModelId=42BA535B014B
	inline Vector2( float X_, float Y_ ) : x(X_), y(Y_)
	{
	}

	//##ModelId=42BA535B014E
	inline Vector2 &operator+=( Vector2 const &A )
	{
		x += A.x;
		y += A.y;
		return *this;
	}
	//##ModelId=42BA535B0150
	inline Vector2 &operator-=( Vector2 const &A )
	{
		x -= A.x;
		y -= A.y;
		return *this;
	}

	inline Vector2 operator -(void) const { return Vector2(-x, -y); }

	Vector2 operator* ( Matrix2 const &A) const
	{
		return Vector2(A.aElements[0][0] * x + A.aElements[0][1]*y,
						A.aElements[1][0]*x + A.aElements[1][1]*y);
	}

	Vector2 operator ^ (const Matrix2& M) const
	{
		Vector2 T;
		T.x = x * M.e11 + y * M.e21;
		T.y = x * M.e12 + y * M.e22;
		return T;
	}

	//##ModelId=42BA535B0158
	inline Vector2 &operator*=( float const &scalar ){
			x*=scalar;
			y*=scalar;
			return *this;
	}

	//##ModelId=42BA535B015A
	inline bool operator==( Vector2 const &v ){			
		return (x==v.x && y==v.y) ? 1 : 0;
	}
	//##ModelId=42BA535B015C
	inline bool operator!=( Vector2 const &v ){			
		return (x==v.x && y==v.y) ? 0 : 1;
	}

	//##ModelId=42BA535B015E
	inline bool	operator<( Vector2 const &B )
	{
		return (x<B.x && y<B.y);
	}

	//##ModelId=42BA535B0160
	inline bool	operator>( Vector2 const &B )
	{
		return(x>B.x && y>B.y);
	}

	//##ModelId=42BA535B0162
	inline Vector2 GetPerp()
	{
		return Vector2(-y,x);
	}

	//##ModelId=42BA535B0163
	inline bool operator < (const Vector2 &A) const
	{
		return (x<A.x && y<A.y) ? 1 : 0;
	}
	//##ModelId=42BA535B0169
	inline bool operator > (const Vector2 &A) const
	{
		return (x>A.x && y>A.y) ? 1 : 0;
	}

	//##ModelId=42BA535B016C
	inline Real GetLength(){
		return Real(sqrt(x*x + y*y));
	//	return Real(Math::FastSqrt(x*x + y*y));
	}

	//##ModelId=42BA535B016D
	inline Real GetLengthSquared(){
			return Real(x*x + y*y);
	}

	//##ModelId=42BA535B016E
	inline float		normalise(){
		float l=GetLength();
		Real OneOverLength = Real(1)/l;
		x = OneOverLength*x;
		y = OneOverLength*y;
		return l;
	}
	//##ModelId=42BA535B016F
	Vector2 normalisedCopy()
	{
		normalise();
		return Vector2(x,y);
	}

	//##ModelId=42BA535B0170
	inline void Invert()
	{
		x= -x;
		y= -y;
	}
	//! returns interpolated vector
	//! \param other: other vector to interpolate between
	//! \param d: value between 0.0f and 1.0f.
	//##ModelId=42BA535B0171
	Vector2 getInterpolated(const Vector2& other, float d) const
	{
		float inv = 1.0f - d;
		return Vector2(other.x*inv + x*d,	other.y*inv + y*d);
	}


	//##ModelId=42BA535B0175
	inline Vector2 operator-( Vector2 const &A)
	{
		return Vector2(x - A.x, y - A.y);
	}

	//##ModelId=42BA535B0178
	inline Vector2 operator+(const Vector2 &A)const 
	{
		return Vector2(x + A.x, y + A.y);
	}

	//##ModelId=42BA535B017A
	inline Vector2 operator*( Real A)
	{
		return Vector2(A*x,A*y);
	}



	//##ModelId=42BA535B017C
	inline Vector2 operator/(Real B )
	{
		return Vector2(x/B,y/B);
	}

	//##ModelId=42BA535B017E
	inline Real DotProduct( Vector2 const &A)
	{
		return x*A.x + y*A.y;
	}

	//##ModelId=42BA535B0180
	inline Real PerpDotProduct( Vector2 const &A)
	{
		return y*A.x - x*A.y;
	}

	//##ModelId=42BA535B0182
	inline Vector2 GetPerpendicular( Vector2 const &A )
	{
		return Vector2(-A.y,A.x);
	}

	inline Vector2 GetPerpendicular()
	{
		return Vector2(-y,x);
	}

	//##ModelId=42BA535B0187
	inline Real GetLength( Vector2 const &A )
	{
		return Real(sqrt(A.x*A.x + A.y*A.y));
	}

	//##ModelId=42BA535B0189
	inline Vector2 GetNormal( Vector2 const &A )
	{
		Real OneOverLength = Real(1)/GetLength(A);
		return Vector2(OneOverLength*A.x,OneOverLength*A.y);
	}
		//##ModelId=42BA535B0189
	inline Vector2 GetNormalised()
	{
		Real OneOverLength = Real(1)/GetLength();
		return Vector2(OneOverLength*x,OneOverLength*y);
	}
};

 


#endif
