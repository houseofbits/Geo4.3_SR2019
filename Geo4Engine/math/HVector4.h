
#ifndef __Vector4_H__
#define __Vector4_H__

#include "HPrerequisites.h"
#include "HVector2.h"
#include "HVector3.h"
#include "HMatrix4.h"

    class Vector4
    {
    public:
        union {
            struct {
                Real x, y, z, w;        
            };
            Real val[4];
        };

    public:
        inline Vector4()
        {
			memset(val, 0, 4 * sizeof(Real));
        }

        inline Vector4( Real fX, Real fY, Real fZ, Real fW ) 
            : x( fX ), y( fY ), z( fZ ), w( fW)
        {
        }

        inline Vector4( Real afCoordinate[4] )
            : x( afCoordinate[0] ),
              y( afCoordinate[1] ),
              z( afCoordinate[2] ), 
              w (afCoordinate[3] )
        {
        }

        inline Vector4( int afCoordinate[4] )
        {
            x = (Real)afCoordinate[0];
            y = (Real)afCoordinate[1];
            z = (Real)afCoordinate[2];
            w = (Real)afCoordinate[3];
        }

        inline Vector4( const Real* const r )
            : x( r[0] ), y( r[1] ), z( r[2] ), w( r[3] )
        {
        }

        inline Vector4( const Vector4& rkVector )
            : x( rkVector.x ), y( rkVector.y ), z( rkVector.z ), w (rkVector.w)
        {
        }

		inline Vector4(const Vector3& rkVector, Real fW)
			: x(rkVector.x), y(rkVector.y), z(rkVector.z), w(fW)
		{
		}


        inline Real operator [] ( unsigned i ) const
        {
            assert( i < 4 );

            return *(&x+i);
        }

		inline Real& operator [] ( unsigned i )
        {
            assert( i < 4 );

            return *(&x+i);
        }

        inline Vector4& operator = ( const Vector4& rkVector )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;            
            w = rkVector.w;            

            return *this;
        }

        inline bool operator == ( const Vector4& rkVector ) const
        {
            return ( x == rkVector.x && 
                y == rkVector.y && 
                z == rkVector.z &&
                w == rkVector.w );
        }

        inline bool operator != ( const Vector4& rkVector ) const
        {
            return ( x != rkVector.x || 
                y != rkVector.y || 
                z != rkVector.z ||
                w != rkVector.w );
        }

        inline Vector4& operator = (const Vector3& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = 1.0f;
            return *this;
        }

        inline Vector4 operator * (const Matrix4& mat) const
        {
            return Vector4(
                x*mat[0][0] + y*mat[1][0] + z*mat[2][0] + w*mat[3][0],
                x*mat[0][1] + y*mat[1][1] + z*mat[2][1] + w*mat[3][1],
                x*mat[0][2] + y*mat[1][2] + z*mat[2][2] + w*mat[3][2],
                x*mat[0][3] + y*mat[1][3] + z*mat[2][3] + w*mat[3][3]
                );
        }

        inline Real dotProduct(const Vector4& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
        }

		inline Vector2 xy(){
			return Vector2(x,y);
		}

		inline Vector3 xyz() {
			return Vector3(x, y, z);
		}

    };

#endif
