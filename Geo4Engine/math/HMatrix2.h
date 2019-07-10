#ifndef __Matrix2_H__
#define __Matrix2_H__

#include "HPrerequisites.h"

//##ModelId=42BA535C00E5
class Matrix2
{
public:
union
	{
		struct
		{
			float e11;
			float e12;
			float e21;
			float e22;
		};
		Real aElements[2][2];
	};

	//##ModelId=42BA535C00EF
	inline Matrix2( void )
	{
		aElements[0][0] = aElements[0][1] = aElements[1][0] = aElements[1][1] =Real(0);
	}

	//##ModelId=42BA535C00F1
	inline Matrix2( Real Radians )
	{
		Real const CosAngle = (Real)cos(Radians);
		Real const SinAngle = (Real)sin(Radians);

		aElements[0][0] = CosAngle; aElements[0][1] = -SinAngle;
		aElements[1][0] = SinAngle; aElements[1][1] =  CosAngle;
	}

	//##ModelId=42BA535C00F3
	inline Matrix2 Transpose()
	{
		Matrix2 trans;

		for(int i=0; i<2;i++){	
			for(int j=0; j<2; j++){
			trans.aElements[i][j] = aElements[j][i];
			}
		}
		return trans;
	}
	Matrix2 operator * (const Matrix2& M) const 
	{
		Matrix2 T;

		T.e11 = e11 * M.e11 + e12 * M.e21;
		T.e21 = e21 * M.e11 + e22 * M.e21;
		T.e12 = e11 * M.e12 + e12 * M.e22;
		T.e22 = e21 * M.e12 + e22 * M.e22;
		
		return T;
	}

	Matrix2 operator ^ (const Matrix2& M) const 
	{
		Matrix2 T;

		T.e11 = e11 * M.e11 + e12 * M.e12;
		T.e21 = e21 * M.e11 + e22 * M.e12;
		T.e12 = e11 * M.e21 + e12 * M.e22;
		T.e22 = e21 * M.e21 + e22 * M.e22;
		
		return T;
	}

	inline Matrix2 operator * ( float s) const
	{
		Matrix2 T;

		T.e11 = e11 * s;
		T.e21 = e21 * s;
		T.e12 = e12 * s;
		T.e22 = e22 * s;
		
		return T;
	}

};



#endif
