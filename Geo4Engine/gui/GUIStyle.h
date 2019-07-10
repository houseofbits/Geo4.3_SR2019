#pragma once

#include <vector>
#include "../ResourceManager.h"
#include "../TrueTypeFont.h"

/*
CFO example:
	GUIStyle={
	  buttonDefaultActive={
		radius=5;
		borderSize=1;
		borderColor=1,1,1,0.5;
		shadowSize=5;
		shadowHardness=0.5;
		shadowX=0;
		shadowY=1;
		shadowColor=0,0,0,0.4;
		backgroundGradientColor={
		  0=0.498, 0.545, 0.6, 0.6;
		  0.95=0.854, 0.874, 0.886, 0.8;
		  1=0.737, 0.756, 0.780, 0.8;
		  angle=45;
		}
		fontType="";
		fontSize=13;
		fontColor=0,0,0,1;
		fontShadowPosition=0,-1;
		fontShadowColor=1,1,1,1;
	  }
	}

All parameters:
	float radius
	float radiusTopLeft
	float radiusTopRight
	float radiusBottomLeft
	float radiusBottomRight

	float borderSize
	float borderSizeTop
	float borderSizeBottom
	float borderSizeLeft
	float borderSizeRight

	Vector4 borderColor
	Vector4 borderColorLeft
	Vector4 borderColorRight
	Vector4 borderColorTop
	Vector4 borderColorBottom

	float		shadowSize
	float		shadowHardness - [0,1]
	Vector2		shadowPosition
	Vector4		shadowColor

	GUIGradientColor	backgroundGradientColor
	Vector4				backgroundColor

	Vector4			graphLineColor;
	Vector4			graphDividerLineColor;
	Vector4			graphRefLineColor;
	Vector4			graphDataLineColor;
	Vector4			graphDataLineColor2;
	Vector4			graphDataLineColor3;
	Vector4			graphDataLineColor4;
	Vector4			graphDataLineColor5;
	LineStripple	graphLineStyle;
	LineStripple	graphDividerLineStyle;
	LineStripple	graphRefLineStyle;
	LineStripple	graphDataLineStyle;
	LineStripple	graphDataLineStyle2;
	LineStripple	graphDataLineStyle3;
	LineStripple	graphDataLineStyle4;
	LineStripple	graphDataLineStyle5;
*/
class GUIGradientColor {
public:
	GUIGradientColor();
	class GradientPoint {
	public:
		GradientPoint() : color(), position(0){}
		GradientPoint(Vector4 c, float p) :color(c), position(p) {}
		Vector4 color;
		float	position;
	};
	void add(Vector4 c, float p) { points.push_back(GradientPoint(c, p)); }
	void draw();
	void generate();
	void Deserialize(CFONode*);

	Vector4 _getColor(float f);

	vector<GradientPoint> points;
	GLuint			vertexArrayId;
	GLuint			indexArrayId;
	unsigned int	numIndices;
	float			angle;

	float			textureU;
	float			textureU1;

	static GLuint gradientTextureMap;
	static unsigned int gradientMapPositionIterator;
	const static unsigned int gradientMapWidth;
	const static unsigned int gradientMapHeight;
	const static unsigned int gradientEntryWidth;
};

class GUIStyle {
public:
	GUIStyle();

	enum FillType {
		NONE = 0,
		SOLID,
		GRADIENT
	};
	enum TextJusify {
		CENTER = 0,
		LEFT,
		RIGHT
	};
	enum TextVerticalAlign {
		MIDDLE = 0,
		TOP,
		BOTTOM,
		BASELINE
	};
	enum LineStripple {
		NO_STRIPPLE = 0,
		DOTTED = 0xAAAA,
		DASHED = 0x00FF,
	};

	virtual ~GUIStyle();

	float radius;				//ONLY FOR SERIALIZER. MIGHT BE REMOVED
	float radiusTopLeft;
	float radiusTopRight;
	float radiusBottomLeft;
	float radiusBottomRight;

	float borderSize;			//ONLY FOR SERIALIZER. MIGHT BE REMOVED
	float borderSizeTop;
	float borderSizeBottom;
	float borderSizeLeft;
	float borderSizeRight;

	Vector4 borderColor;		//ONLY FOR SERIALIZER. MIGHT BE REMOVED
	Vector4 borderColorLeft;
	Vector4 borderColorRight;
	Vector4 borderColorTop;
	Vector4 borderColorBottom;

	float shadowSize;
	float shadowHardness;
	Vector2 shadowPosition;
	Vector4 shadowColor;

	GUIGradientColor backgroundGradientColor;
	Vector4			backgroundColor;
	FillType		backgroundFill;

	std::string	fontName;
	float	fontSize;
	Vector4	fontColor;
	Vector2	fontShadowPosition;
	Vector4	fontShadowColor;
	float	lineHeight;

	TextJusify			textJustify;
	TextVerticalAlign	textVerticalAlign;

	float				textPaddingLeft;
	float				textPaddingRight;
	float				textPaddingTop;
	float				textPaddingBottom;

	TrueTypeFontFaceHandle fontHandle;

	bool	_fontValid;
	bool	_fontHasShadow;

	Vector4		graphLineColor;
	Vector4		graphDividerLineColor;
	Vector4		graphRefLineColor;
	Vector4		graphDataLineColor;
	Vector4		graphDataLineColor2;
	Vector4		graphDataLineColor3;
	Vector4		graphDataLineColor4;
	Vector4		graphDataLineColor5;
	LineStripple graphLineStyle;
	LineStripple graphDividerLineStyle;
	LineStripple graphRefLineStyle;
	LineStripple graphDataLineStyle;
	LineStripple graphDataLineStyle2;
	LineStripple graphDataLineStyle3;
	LineStripple graphDataLineStyle4;
	LineStripple graphDataLineStyle5;

	void Deserialize(CFONode*, ResourceManager*);

	LineStripple parseLineStripple(string);
	TextJusify parseTextJustify(string);
	TextVerticalAlign parseTextVerticalAlign(string);
};