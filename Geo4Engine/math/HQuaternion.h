
#ifndef __Quaternion_H__
#define __Quaternion_H__

#include "HPrerequisites.h"

//##ModelId=42BA535B0232
    class Quaternion
    {
    public:
	//##ModelId=42BA535B0242
        inline Quaternion (
            Real fW = 1.0,
            Real fX = 0.0, Real fY = 0.0, Real fZ = 0.0)
		{
			w = fW;
			x = fX;
			y = fY;
			z = fZ;
		}
	//##ModelId=42BA535B0247
        inline Quaternion (const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
		}

	//##ModelId=42BA535B0249
        void FromRotationMatrix (const Matrix3& kRot);
	//##ModelId=42BA535B024B
        void ToRotationMatrix (Matrix3& kRot) const;
	//##ModelId=42BA535B024E
        void FromAngleAxis (const Real& rfAngle, const Vector3& rkAxis);
	//##ModelId=42BA535B0251
        void ToAngleAxis (Real& rfAngle, Vector3& rkAxis) const;
	//##ModelId=42BA535B0255
        void FromAxes (const Vector3* akAxis);
	//##ModelId=42BA535B0257
        void FromAxes (const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
	//##ModelId=42BA535B025B
        void ToAxes (Vector3* akAxis) const;
	//##ModelId=42BA535B025E
        void ToAxes (Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

	//##ModelId=42BA535B0263
        Vector3 xAxis(void);

	//##ModelId=42BA535B0265
        Vector3 yAxis(void);

	//##ModelId=42BA535B0267
        Vector3 zAxis(void);

	//##ModelId=42BA535B0269
        inline Quaternion& operator= (const Quaternion& rkQ)
		{
			w = rkQ.w;
			x = rkQ.x;
			y = rkQ.y;
			z = rkQ.z;
			return *this;
		}
	//##ModelId=42BA535B026B
        Quaternion operator+ (const Quaternion& rkQ) const;
	//##ModelId=42BA535B026E
        Quaternion operator- (const Quaternion& rkQ) const;
	//##ModelId=42BA535B0271
        Quaternion operator* (const Quaternion& rkQ) const;
	//##ModelId=42BA535B0274
        Quaternion operator* (Real fScalar) const;
	//##ModelId=42BA535B02B8
        friend Quaternion operator* (Real fScalar,
            const Quaternion& rkQ);
	//##ModelId=42BA535B0277
        Quaternion operator- () const;
	//##ModelId=42BA535B0279
        bool operator== (const Quaternion& rhs) const;


	//##ModelId=42BA535B027C
        Real Dot (const Quaternion& rkQ) const;  
	//##ModelId=42BA535B027F
        Real Norm () const;  
	//##ModelId=42BA535B0281
        Real normalise(void); 
	//##ModelId=42BA535B0283
        Quaternion Inverse () const;  
	//##ModelId=42BA535B0285
        Quaternion UnitInverse () const;  
	//##ModelId=42BA535B0287
        Quaternion Exp () const;
	//##ModelId=42BA535B0289
        Quaternion Log () const;

	//##ModelId=42BA535B028B
        Vector3 operator* (const Vector3& rkVector) const;

	//##ModelId=42BA535B028E
        static Quaternion Slerp (Real fT, const Quaternion& rkP,
            const Quaternion& rkQ, bool shortestPath = false);

		static Quaternion Lerp(Real fT, const Quaternion& rkP, const Quaternion& rkQ);

	//##ModelId=42BA535B0294
        static Quaternion SlerpExtraSpins (Real fT,
            const Quaternion& rkP, const Quaternion& rkQ,
            int iExtraSpins);

	//##ModelId=42BA535B029A
        static void Intermediate (const Quaternion& rkQ0,
            const Quaternion& rkQ1, const Quaternion& rkQ2,
            Quaternion& rka, Quaternion& rkB);

	//##ModelId=42BA535B02A1
        static Quaternion Squad (Real fT, const Quaternion& rkP,
            const Quaternion& rkA, const Quaternion& rkB,
            const Quaternion& rkQ, bool shortestPath = false);

	//##ModelId=42BA535B02AA
        static const Real ms_fEpsilon;

	//##ModelId=42BA535B02B1
        static const Quaternion ZERO;
	//##ModelId=42BA535B02B6
        static const Quaternion IDENTITY;

        Real w, x, y, z;


    };






#endif 
