
#ifndef __Math_H__
#define __Math_H__

#include <algorithm>
#include "HPrerequisites.h"
#include "HVector2.h"
//


#define SMALL_NUM  0.00000001 // anything that avoids division overflow

    class  Math 
    {
   public:
       enum AngleUnit
       {
           AU_DEGREE,
           AU_RADIAN
       };

    protected:
       static AngleUnit msAngleUnit;

        static int mTrigTableSize;

        static Real mTrigTableFactor;
        static Real* mSinTable;
        static Real* mTanTable;

        void buildTrigTables();

		static Real SinTable (Real fValue);
		static Real TanTable (Real fValue);
    public:
        Math(unsigned int trigTableSize = 4096);

        ~Math();
		

		static bool		InsideRect(Vector2 p, Vector2 minc, Vector2 maxc){			
			if(p.x < __max(minc.x, maxc.x)){
				if(p.x > __min(minc.x, maxc.x)){
					if(p.y < __max(minc.y, maxc.y)){
						if(p.y > __min(minc.y, maxc.y)){
							return 1;
						}
					}
				}
			}
			return 0;
		}

		static bool		InsidePosRect(Vector2 p, Vector2 pos, Vector2 size){
			return InsideRect(p, pos-(size*0.5f), pos+(size*0.5f));
		}

		static Vector2 Truncate(Vector2 v, float max){         
			float l = v.GetLength();
			if(l>max){
					v.normalise();
					v = v * max;
					return v;
			}
			return v;                  
		}
		static float Interpolate(float a, float b, float d){
			float inv = 1.0f - d;
			return b*inv + a*d;
		}

		static float Trim(float val, float minf, float maxf){
			return __max(minf, __min(maxf, val));
		}

		static Vector3 Trim(Vector3 v, float minf, float maxf);
		
		static bool IntersectRect(Vector2 p1, Vector2 s1, Vector2 p2, Vector2 s2){
			return ((s1.x/2)+(s2.x/2))>fabs(p2.x-p1.x) && ((s1.y/2)+(s2.y/2))>fabs(p2.y-p1.y);
		}

		static float RadiansFromVector(Vector2 ori)
		{
			ori.normalise();

			float dot = Vector2(1,0).DotProduct(ori);
			float perpdot = Vector2(1,0).PerpDotProduct(ori);

			float angle = (float)fabs(acos(dot));
			angle = RadiansToDegrees(angle);
				
			int sign = (perpdot<0) ? 1 : -1;
						
			angle*=sign;

			return DegreesToRadians(angle);
		}
		
		static void clamptozero(float& val){
			float num =  0.000001f; //SMALL_NUM
			if(val<num && val>-num)val = 0.0f;
		}

		static Vector2	VectorFrom(float radians)
		{	
			/*
			Matrix2 m(radians);
			Vector2 ori(1,0);
			ori = ori * m;
			ori.normalise();
			/**/
			//return ori;

			float x = cos(radians);
			float y = sin(radians);

			clamptozero(x);
			clamptozero(y);

			return Vector2(x,y);
		}
		static inline Vector2 TransformIntoSpace(Vector2 XAxis, Vector2 YAxis, Vector2 point){
			return (XAxis * point.x + YAxis * point.y);
		}

		static inline int IAbs (int iValue) { return ( iValue >= 0 ? iValue : -iValue ); }
		static inline int ICeil (float fValue) { return int(ceil(fValue)); }
		static inline int IFloor (float fValue) { return int(floor(fValue)); }
        static int ISign (int iValue);

		static inline Real Abs (Real fValue) { return Real(fabs(fValue)); }
        static Real ACos (Real fValue);
        static Real ASin (Real fValue);
		static inline Real ATan (Real fValue) { return Real(atan(fValue)); }
		static inline Real ATan2 (Real fY, Real fX) { return Real(atan2(fY,fX)); }
		static inline Real Ceil (Real fValue) { return Real(ceil(fValue)); }

        static inline Real Cos (Real fValue, bool useTables = false) {
			return (!useTables) ? Real(cos(fValue)) : SinTable(fValue + HALF_PI);
		}

		static inline Real Exp (Real fValue) { return Real(exp(fValue)); }

		static inline Real Floor (Real fValue) { return Real(floor(fValue)); }

		static inline Real Log (Real fValue) { return Real(log(fValue)); }

		static inline Real Pow (Real fBase, Real fExponent) { return Real(pow(fBase,fExponent)); }

        static Real Sign (Real fValue);

        static inline Real Sin (Real fValue, bool useTables = false) {
			return (!useTables) ? Real(sin(fValue)) : SinTable(fValue);
		}

		static inline Real Sqr (Real fValue) { return fValue*fValue; }

		static inline Real Sqrt (Real fValue) { return Real(sqrt(fValue)); }

		static inline Real FastSqrt (Real number) {
			long i;
			float x, y;
			const float f = 1.5F;

			x = number * 0.5F;
			y  = number;
			i  = * ( long * ) &y;
			i  = 0x5f3759df - ( i >> 1 );
			y  = * ( float * ) &i;
			y  = y * ( f - ( x * y * y ) );
		//    y  = y * ( f - ( x * y * y ) );
			return number * y;
		}


		static Real InvSqrt(Real fValue);

        static Real UnitRandom ();  // in [0,1]

        static Real RangeRandom (Real fLow, Real fHigh);  // in [fLow,fHigh]

        static Real SymmetricRandom ();  // in [-1,1]

		static inline Real Tan (Real fValue, bool useTables = false) {
			return (!useTables) ? Real(tan(fValue)) : TanTable(fValue);
		}

		static inline Real DegreesToRadians(Real degrees) { return degrees * fDeg2Rad; }
        static inline Real RadiansToDegrees(Real radians) { return radians * fRad2Deg; }

       static void setAngleUnit(AngleUnit unit);
       static AngleUnit getAngleUnit(void);

       static Real AngleUnitsToRadians(Real units);
       static Real RadiansToAngleUnits(Real radians);

        static bool pointInTri2D( Real px, Real pz, Real ax, Real az, Real bx, Real bz, Real cx, Real cz );

		static bool PointInsideTriangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c);

        static std::pair<bool, Real> intersects(const Ray& ray, const Plane& plane);

        static std::pair<bool, Real> intersects(const Ray& ray, const Sphere& sphere);
        
        static std::pair<bool, Real> intersects(const Ray& ray, const AxisAlignedBox& sphere);

        static bool intersects(const Sphere& sphere, const AxisAlignedBox& box);

        static bool intersects(const Plane& plane, const AxisAlignedBox& box);

        static bool intersects(const Sphere& sphere, const Plane& plane);


        static bool RealEqual(Real a, Real b,
            Real tolerance = std::numeric_limits<Real>::epsilon());

        static Vector3 calculateTangentSpaceVector(
            const Vector3& position1, const Vector3& position2, const Vector3& position3,
            Real u1, Real v1, Real u2, Real v2, Real u3, Real v3);

        static Matrix4 buildReflectionMatrix(const Plane& p);

        static const Real POS_INFINITY;
        static const Real NEG_INFINITY;
        static const Real PI;
        static const Real TWO_PI;
        static const Real HALF_PI;
		static const Real fDeg2Rad;
		static const Real fRad2Deg;

		//
		//http://www.flong.com/texts/code/shapers_poly/
		//
		static inline float circularEaseIn(float x) {
			float y = 1 - sqrt(1 - x*x);
			return y;
		}
		static inline float circularEaseOut(float x) {
			float y = sqrt(1 - Math::Sqr(1 - x));
			return y;
		}
		static inline float exponentialEasing(float x, float a) {

			float epsilon = 0.00001f;
			float min_param_a = 0.0f + epsilon;
			float max_param_a = 1.0f - epsilon;
			a = std::max<float>(min_param_a, std::min<float>(max_param_a, a));

			if (a < 0.5f) {
				// emphasis
				a = 2.0f*(a);
				float y = pow(x, a);
				return y;
			}
			else {
				// de-emphasis
				a = 2.0f*(a - 0.5f);
				float y = pow(x, 1.0f / (1 - a));
				return y;
			}
		}


		// Helper function called by HSL2RGB
		static inline float Hue2RGB(float v1, float v2, float vH)
		{
			if (vH < 0) vH += 1.f;
			if (vH > 1) vH -= 1.f;
			if (6.f * vH < 1) return v1 + (v2 - v1) * 6.f * vH;
			if (2.f * vH < 1) return v2;
			if (3.f * vH < 2) return v1 + (v2 - v1) * (0.666666f - vH) * 6.f;
			return v1;
		}
		static Vector3 HSL2RGB(Vector3 hsl);



    };


	static float asm_rsq( float r )
	{
		return float(1.0f / sqrt( r ));
	}

	static float asm_rand()
	{
		return float( rand() );
	}

	static float asm_rand_max()
	{
		return float( RAND_MAX );
	}


#endif
