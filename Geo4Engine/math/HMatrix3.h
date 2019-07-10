
#ifndef __Matrix3_H__
#define __Matrix3_H__

#include "HPrerequisites.h"

#include "HVector3.h"


//##ModelId=42BA535C000F
    class  Matrix3
    {
    public:
	//##ModelId=42BA535C0010
		inline Matrix3 () {
			memset(m,0, 9 * sizeof(Real));
		};
	//##ModelId=42BA535C0011
        inline Matrix3 (const Real arr[3][3])
		{
			memcpy(m,arr,9*sizeof(Real));
		}
	//##ModelId=42BA535C0013
        inline Matrix3 (const Matrix3& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(Real));
		}
	//##ModelId=42BA535C0015
        Matrix3 (Real fEntry00, Real fEntry01, Real fEntry02,
                    Real fEntry10, Real fEntry11, Real fEntry12,
                    Real fEntry20, Real fEntry21, Real fEntry22)
		{
			m[0][0] = fEntry00;
			m[0][1] = fEntry01;
			m[0][2] = fEntry02;
			m[1][0] = fEntry10;
			m[1][1] = fEntry11;
			m[1][2] = fEntry12;
			m[2][0] = fEntry20;
			m[2][1] = fEntry21;
			m[2][2] = fEntry22;
		}

	//##ModelId=42BA535C0027
        inline Real* operator[] (int iRow) const
		{
			return (Real*)m[iRow];
		}
	//##ModelId=42BA535C002A
        inline operator Real* ()
		{
			return (Real*)m[0];
		}
	//##ModelId=42BA535C002B
        Vector3 GetColumn (int iCol) const;
	//##ModelId=42BA535C002E
        void SetColumn(int iCol, const Vector3& vec);
	//##ModelId=42BA535C0031
        void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

	//##ModelId=42BA535C0035
        inline Matrix3& operator= (const Matrix3& rkMatrix)
		{
			memcpy(m,rkMatrix.m,9*sizeof(Real));
			return *this;
		}
	//##ModelId=42BA535C0037
        bool operator== (const Matrix3& rkMatrix) const;
	//##ModelId=42BA535C003A
        inline bool operator!= (const Matrix3& rkMatrix) const
		{
			return !operator==(rkMatrix);
		}

	//##ModelId=42BA535C003D
        Matrix3 operator+ (const Matrix3& rkMatrix) const;
	//##ModelId=42BA535C0040
        Matrix3 operator- (const Matrix3& rkMatrix) const;
	//##ModelId=42BA535C0043
        Matrix3 operator* (const Matrix3& rkMatrix) const;
	//##ModelId=42BA535C0046
        Matrix3 operator- () const;

	//##ModelId=42BA535C0048
        Vector3 operator* (const Vector3& rkVector) const;

	//##ModelId=42BA535C00DD
        friend Vector3 operator* (const Vector3& rkVector,
            const Matrix3& rkMatrix);

	//##ModelId=42BA535C004B
        Matrix3 operator* (Real fScalar) const;

	//##ModelId=42BA535C00E1
        friend Matrix3 operator* (Real fScalar, const Matrix3& rkMatrix);

	//##ModelId=42BA535C004E
        Matrix3 Transpose () const;
	//##ModelId=42BA535C0050
        bool Inverse (Matrix3& rkInverse, Real fTolerance = 1e-06) const;
	//##ModelId=42BA535C0054
        Matrix3 Inverse (Real fTolerance = 1e-06) const;
	//##ModelId=42BA535C0057
        Real Determinant () const;

	//##ModelId=42BA535C0059
        void SingularValueDecomposition (Matrix3& rkL, Vector3& rkS,
            Matrix3& rkR) const;
	//##ModelId=42BA535C005E
        void SingularValueComposition (const Matrix3& rkL,
            const Vector3& rkS, const Matrix3& rkR);

	//##ModelId=42BA535C0062
        void Orthonormalize ();

	//##ModelId=42BA535C0063
        void QDUDecomposition (Matrix3& rkQ, Vector3& rkD,
            Vector3& rkU) const;

	//##ModelId=42BA535C0068
        Real SpectralNorm () const;

	//##ModelId=42BA535C006A
        void ToAxisAngle (Vector3& rkAxis, Real& rfRadians) const;
	//##ModelId=42BA535C006E
        void FromAxisAngle (const Vector3& rkAxis, Real fRadians);

	//##ModelId=42BA535C0071
        bool ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
	//##ModelId=42BA535C0076
        bool ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
	//##ModelId=42BA535C007B
        bool ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
	//##ModelId=42BA535C0080
        bool ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
	//##ModelId=42BA535C0085
        bool ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
	//##ModelId=42BA535C008A
        bool ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle,
            float& rfRAngle) const;
	//##ModelId=42BA535C008F
        void FromEulerAnglesXYZ (float fYAngle, float fPAngle, float fRAngle);
	//##ModelId=42BA535C0093
        void FromEulerAnglesXZY (float fYAngle, float fPAngle, float fRAngle);
	//##ModelId=42BA535C0097
        void FromEulerAnglesYXZ (float fYAngle, float fPAngle, float fRAngle);
	//##ModelId=42BA535C009B
        void FromEulerAnglesYZX (float fYAngle, float fPAngle, float fRAngle);
	//##ModelId=42BA535C009F
        void FromEulerAnglesZXY (float fYAngle, float fPAngle, float fRAngle);
	//##ModelId=42BA535C00A3
        void FromEulerAnglesZYX (float fYAngle, float fPAngle, float fRAngle);

	//##ModelId=42BA535C00A7
        void EigenSolveSymmetric (Real afEigenvalue[3],
            Vector3 akEigenvector[3]) const;

	//##ModelId=42BA535C00AB
        static void TensorProduct (const Vector3& rkU, const Vector3& rkV,
            Matrix3& rkProduct);

	//##ModelId=42BA535C00B1
        static const Real EPSILON;
	//##ModelId=42BA535C00B8
        static const Matrix3 ZERO;
	//##ModelId=42BA535C00BD
        static const Matrix3 IDENTITY;

    protected:
	//##ModelId=42BA535C00BF
        void Tridiagonal (Real afDiag[3], Real afSubDiag[3]);
	//##ModelId=42BA535C00C2
        bool QLAlgorithm (Real afDiag[3], Real afSubDiag[3]);

	//##ModelId=42BA535C00C6
        static const Real ms_fSvdEpsilon;
	//##ModelId=42BA535C00CA
        static const int ms_iSvdMaxIterations;
	//##ModelId=42BA535C00CB
        static void Bidiagonalize (Matrix3& kA, Matrix3& kL,
            Matrix3& kR);
	//##ModelId=42BA535C00D0
        static void GolubKahanStep (Matrix3& kA, Matrix3& kL,
            Matrix3& kR);

	//##ModelId=42BA535C00D5
        static Real MaxCubicRoot (Real afCoeff[3]);

	//##ModelId=42BA535C00D9
        Real m[3][3];

        friend class Matrix4;
    };
#endif
