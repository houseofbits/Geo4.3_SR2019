#include "math/HMath.h"
#include "math/HColor.h"
//#include <gl/GL.h>

//#include <gl/GL.h>
//#include <gl/GLU.h>


void Color::GLColor(int &c) {
	/*glColor4ub(Color::R(c),
		Color::G(c),
		Color::B(c),
		Color::A(c)
	);*/

}

int Color::Interpolate(int& A, int& B, float val) {

	return RGBA((int)Math::Interpolate((float)Color::R(A), (float)Color::R(B), val),
		(int)Math::Interpolate((float)Color::G(A), (float)Color::G(B), val),
		(int)Math::Interpolate((float)Color::B(A), (float)Color::B(B), val),
		(int)Math::Interpolate((float)Color::A(A), (float)Color::A(B), val)
	);
}