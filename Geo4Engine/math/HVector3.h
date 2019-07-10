
#ifndef __Vector3_H__
#define __Vector3_H__

#include "HPrerequisites.h"
#include "HMath.h"
#include "HQuaternion.h"


//##ModelId=42BA535B009C
    class  Vector3
    {
    public:
        union {
            struct {
                Real x, y, z;        
            };
            Real val[3];
        };

    public:
	//##ModelId=42BA535B009D
        inline Vector3() : x(0), y(0), z(0)
        {
			memset(val, 0, 3 * sizeof(Real));
        }

	//##ModelId=42BA535B009E
        inline Vector3( Real fX, Real fY, Real fZ ) 
            : x( fX ), y( fY ), z( fZ )
        {
        }

	//##ModelId=42BA535B00A2
        inline Vector3( Real afCoordinate[3] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] )
        {
        }

	//##ModelId=42BA535B00A4
        inline Vector3( int afCoordinate[3] )
        {
            x = (Real)afCoordinate[0];
            y = (Real)afCoordinate[1];
            z = (Real)afCoordinate[2];
        }

	//##ModelId=42BA535B00A6
        inline Vector3( const Real* const r )
            : x( r[0] ), y( r[1] ), z( r[2] )
        {
        }

	//##ModelId=42BA535B00A8
        inline Vector3( const Vector3& rkVector )
            : x( rkVector.x ), y( rkVector.y ), z( rkVector.z )
        {
        }

	//##ModelId=42BA535B00AC
        inline Real operator [] ( unsigned i ) const
        {
            assert( i < 3 );

            return *(&x+i);
        }

	//##ModelId=42BA535B00AF
		inline Real& operator [] ( unsigned i )
        {
            assert( i < 3 );

            return *(&x+i);
        }

	//##ModelId=42BA535B00B1
        inline Vector3& operator = ( const Vector3& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;            

            return *this;
        }

	//##ModelId=42BA535B00B3
        inline bool operator == ( const Vector3& rkVector ) const
        {
            return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
        }

	//##ModelId=42BA535B00B6
        inline bool operator != ( const Vector3& rkVector ) const
        {
            return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
        }

	//##ModelId=42BA535B00B9
        inline Vector3 operator + ( const Vector3& rkVector ) const
        {
            Vector3 kSum;

            kSum.x = x + rkVector.x;
            kSum.y = y + rkVector.y;
            kSum.z = z + rkVector.z;

            return kSum;
        }

	//##ModelId=42BA535B00BC
        inline Vector3 operator - ( const Vector3& rkVector ) const
        {
            Vector3 kDiff;

            kDiff.x = x - rkVector.x;
            kDiff.y = y - rkVector.y;
            kDiff.z = z - rkVector.z;

            return kDiff;
        }

	//##ModelId=42BA535B00BF
        inline Vector3 operator * ( Real fScalar ) const
        {
            Vector3 kProd;

            kProd.x = fScalar*x;
            kProd.y = fScalar*y;
            kProd.z = fScalar*z;

            return kProd;
        }

	//##ModelId=42BA535B00C2
        inline Vector3 operator * ( const Vector3& rhs) const
        {
            Vector3 kProd;

            kProd.x = rhs.x * x;
            kProd.y = rhs.y * y;
            kProd.z = rhs.z * z;

            return kProd;
        }

	//##ModelId=42BA535B00C5
        inline Vector3 operator / ( Real fScalar ) const
        {
            assert( fScalar != 0.0 );

            Vector3 kDiv;

            Real fInv = 1.0f / fScalar;
            kDiv.x = x * fInv;
            kDiv.y = y * fInv;
            kDiv.z = z * fInv;

            return kDiv;
        }

	//##ModelId=42BA535B00C8
        inline Vector3 operator - () const
        {
            Vector3 kNeg;

            kNeg.x = -x;
            kNeg.y = -y;
            kNeg.z = -z;

            return kNeg;
        }

	//##ModelId=42BA535B0119
        inline friend Vector3 operator * ( Real fScalar, const Vector3& rkVector )
        {
            Vector3 kProd;

            kProd.x = fScalar * rkVector.x;
            kProd.y = fScalar * rkVector.y;
            kProd.z = fScalar * rkVector.z;

            return kProd;
        }

	//##ModelId=42BA535B00CA
        inline Vector3& operator += ( const Vector3& rkVector )
        {
            x += rkVector.x;
            y += rkVector.y;
            z += rkVector.z;

            return *this;
        }

	//##ModelId=42BA535B00CC
        inline Vector3& operator -= ( const Vector3& rkVector )
        {
            x -= rkVector.x;
            y -= rkVector.y;
            z -= rkVector.z;

            return *this;
        }

	//##ModelId=42BA535B00CE
        inline Vector3& operator *= ( Real fScalar )
        {
            x *= fScalar;
            y *= fScalar;
            z *= fScalar;
            return *this;
        }

	//##ModelId=42BA535B00D0
        inline Vector3& operator /= ( Real fScalar )
        {
            assert( fScalar != 0.0f );

            Real fInv = 1.0f / fScalar;

            x *= fInv;
            y *= fInv;
            z *= fInv;

            return *this;
        }

	//##ModelId=42BA535B00D2
        inline Real length () const
        {
            return Math::Sqrt( x * x + y * y + z * z );
        }

	//##ModelId=42BA535B00D4
        inline Real squaredLength () const
        {
            return x * x + y * y + z * z;
        }

	//##ModelId=42BA535B00D6
        inline Real dotProduct(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }
	//##ModelId=42BA535B00D9
        inline Real normalise()
        {
            Real fLength = Math::Sqrt( x * x + y * y + z * z );

            if ( fLength > 1e-06 )
            {
                Real fInvLength = 1.0f / fLength;
                x *= fInvLength;
                y *= fInvLength;
                z *= fInvLength;
            }

            return fLength;
        }

	//##ModelId=42BA535B00DA
        inline Vector3 crossProduct( const Vector3& rkVector ) const
        {
            Vector3 kCross;

            kCross.x = y * rkVector.z - z * rkVector.y;
            kCross.y = z * rkVector.x - x * rkVector.z;
            kCross.z = x * rkVector.y - y * rkVector.x;

            return kCross;
        }

	//##ModelId=42BA535B00DD
        inline Vector3 midPoint( const Vector3& vec ) const
        {
            return Vector3( 
                ( x + vec.x ) * 0.5f, 
                ( y + vec.y ) * 0.5f, 
                ( z + vec.z ) * 0.5f );
        }

	//##ModelId=42BA535B00E0
        inline bool operator < ( const Vector3& rhs ) const
        {
            if( x < rhs.x && y < rhs.y && z < rhs.z )
                return true;
            return false;
        }

	//##ModelId=42BA535B00E3
        inline bool operator > ( const Vector3& rhs ) const
        {
            if( x > rhs.x && y > rhs.y && z > rhs.z )
                return true;
            return false;
        }

	//##ModelId=42BA535B00E6
        inline void makeFloor( const Vector3& cmp )
        {
            if( cmp.x < x ) x = cmp.x;
            if( cmp.y < y ) y = cmp.y;
            if( cmp.z < z ) z = cmp.z;
        }

	//##ModelId=42BA535B00E8
        inline void makeCeil( const Vector3& cmp )
        {
            if( cmp.x > x ) x = cmp.x;
            if( cmp.y > y ) y = cmp.y;
            if( cmp.z > z ) z = cmp.z;
        }

	//##ModelId=42BA535B00EA
        inline Vector3 perpendicular(void) const
        {
            static const Real fSquareZero = float(1e-06 * 1e-06);

            Vector3 perp = this->crossProduct( Vector3::UNIT_X );

            if( perp.squaredLength() < fSquareZero )
            {
                perp = this->crossProduct( Vector3::UNIT_Y );
            }

            return perp;
        }

	//##ModelId=42BA535B00ED
        inline Vector3 randomDeviant(
            Real angle, 
            const Vector3& up = Vector3::ZERO ) const
        {
            Vector3 newUp;

            if (up == Vector3::ZERO)
            {
                newUp = this->perpendicular();
            }
            else
            {
                newUp = up;
            }

            Quaternion q;
            q.FromAngleAxis( Math::UnitRandom() * Math::TWO_PI, *this );
            newUp = q * newUp;

            q.FromAngleAxis( angle, newUp );
            return q * (*this);
        }

	//##ModelId=42BA535B00F1
        Quaternion getRotationTo(const Vector3& dest) const
        {
            Quaternion q;
            Vector3 v0 = *this;
            Vector3 v1 = dest;
            v0.normalise();
            v1.normalise();

            Vector3 c = v0.crossProduct(v1);

            Real d = v0.dotProduct(v1);
            if (d >= 1.0f)
            {
                return Quaternion::IDENTITY;
            }
            Real s = Math::Sqrt( (1+d)*2 );
            Real invs = 1 / s;


            q.x = c.x * invs;
            q.y = c.y * invs;
            q.z = c.z * invs;
            q.w = s * 0.5f;
            return q;
        }

	//##ModelId=42BA535B00F4
        inline bool isZeroLength(void) const
        {
            Real sqlen = (x * x) + (y * y) + (z * z);
            return (sqlen < (1e-06 * 1e-06));

        }

	//##ModelId=42BA535B00F7
        inline Vector3 normalisedCopy(void) const
        {
            Vector3 ret = *this;
            ret.normalise();
            return ret;
        }

	//##ModelId=42BA535B00FA
        inline Vector3 reflect(const Vector3& normal)
        {
            return Vector3( *this - ( 2 * this->dotProduct(normal) * normal ) );
        }

	//! returns interpolated vector
	//! \param other: other vector to interpolate between
	//! \param d: value between 0.0f and 1.0f.
	Vector3 getInterpolated(const Vector3& other, float d) const
	{
		float inv = 1.0f - d;
		return Vector3(other.x*inv + x*d,	other.y*inv + y*d, other.z*inv + z*d);
	}

	//##ModelId=42BA535B00FF
        static const Vector3 ZERO;
	//##ModelId=42BA535B0104
        static const Vector3 UNIT_X;
	//##ModelId=42BA535B0109
        static const Vector3 UNIT_Y;
	//##ModelId=42BA535B010E
        static const Vector3 UNIT_Z;
	//##ModelId=42BA535B0113
        static const Vector3 UNIT_SCALE;
    };

#endif
