#ifndef __Color_H__
#define __Color_H__

#include "HPrerequisites.h"

class Color{
public:
	
	static int R(int& val){
		return (((val)>>24) & 0x000000FF);
	}
	static int G(int& val){
		return (((val)>>16) & 0x000000FF);
	}
	static int B(int& val){
		return (((val)>>8) & 0x000000FF);
	}
	static int A(int& val){
		return (((val)>>0) & 0x000000FF);
	}
	static int RGBA(int r=0, int g=0, int b=0, int a=0){
		
		int val=0;
		
		val |= (((a%256) & 0xff));
		val |= (((b%256) & 0xff)<<8);
		val |= (((g%256) & 0xff)<<16);
		val |= (((r%256) & 0xff)<<24);

	return val;
	}
	static int RGBAf(float r=0, float g=0, float b=0, float a=0){
		return RGBA(int(255.0f*r), int(255.0f*g), int(255.0f*b), int(255.0f*a));
	}
	static int Interpolate(int& A, int& B, float val);
	static void GLColor(int &c);

};



#endif