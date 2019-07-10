#include "../Geo4.h"

#ifndef NO_OPENGL

GLuint GUIGradientColor::gradientTextureMap = 0;
unsigned int GUIGradientColor::gradientMapPositionIterator = 0;
const unsigned int GUIGradientColor::gradientMapWidth = 512;
const unsigned int GUIGradientColor::gradientMapHeight = 16;
const unsigned int GUIGradientColor::gradientEntryWidth = 4;

GUIGradientColor::GUIGradientColor() : points(),
	vertexArrayId(0),
	indexArrayId(0),
	numIndices(0),
	angle(0),
	textureU(0),
	textureU1(1)
{	}

void GUIGradientColor::draw() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(textureU, 0);
	glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(textureU, 1);
	glVertex2f(-0.5f, 0.5f);
	glTexCoord2f(textureU1, 1);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(textureU1, 0);
	glVertex2f(0.5f, -0.5f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void GUIGradientColor::generate() {

	if (points.size() < 2)return;

	//0.Sort
	std::sort(points.begin(), points.end(),
		[](GradientPoint const& a, GradientPoint const& b) { return a.position < b.position; });

	//Generate empty texture
	if (!glIsTexture(gradientTextureMap)) {
		unsigned int size = gradientMapWidth * gradientMapHeight;
		unsigned char* data = new unsigned char[size * 4];
		for (unsigned int i = 0, a = 0; i < size; i++, a += 4) {
			data[a] = 255;
			data[a + 1] = 255;
			data[a + 2] = 255;
			data[a + 3] = 255;
		}
		glGenTextures(1, &gradientTextureMap);
		glBindTexture(GL_TEXTURE_2D, gradientTextureMap);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTexImage2D(GL_TEXTURE_2D, 0, 4, gradientMapWidth, gradientMapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete[] data;
	}
	unsigned int size = gradientMapHeight;
	unsigned char* data = new unsigned char[size * 4];
	unsigned int buffIncr = 0;
	for (unsigned int i = 0; i < gradientMapHeight; i++) {
		float f = (float)i / (float)gradientMapHeight;
		Vector4 c = _getColor(1.0f - f);

		data[buffIncr] = (unsigned char)(255.0f * c.x);
		data[buffIncr + 1] = (unsigned char)(255.0f * c.y);
		data[buffIncr + 2] = (unsigned char)(255.0f * c.z);
		data[buffIncr + 3] = (unsigned char)(255.0f * c.w);
		buffIncr += 4;
	}

	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, gradientMapPositionIterator, 0, 1, gradientMapHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gradientMapPositionIterator++;

	textureU = (float)gradientMapPositionIterator / (float)gradientMapWidth;

	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, gradientMapPositionIterator, 0, 1, gradientMapHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gradientMapPositionIterator++;

	textureU1 = (float)gradientMapPositionIterator / (float)gradientMapWidth;

	glBindTexture(GL_TEXTURE_2D, gradientTextureMap);
	glTexSubImage2D(GL_TEXTURE_2D, 0, gradientMapPositionIterator, 0, 1, gradientMapHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
	gradientMapPositionIterator++;
}

Vector4 GUIGradientColor::_getColor(float f) {
	for (unsigned int i = 0; i < points.size() - 1; i++) {
		float d = f - points[i].position;
		if (points[i].position <= f && points[i + 1].position >= f) {
			Vector4 c;
			c.x = Math::Interpolate(points[i + 1].color.x, points[i].color.x, d);
			c.y = Math::Interpolate(points[i + 1].color.y, points[i].color.y, d);
			c.z = Math::Interpolate(points[i + 1].color.z, points[i].color.z, d);
			c.w = Math::Interpolate(points[i + 1].color.w, points[i].color.w, d);
			return c;
		}
		else if (f == points[i + 1].position) {
			return points[i + 1].color;
		}
	}
	return Vector4(1, 1, 1, 1);
}

GUIStyle::GUIStyle() : radius(0),
	radiusTopLeft(0),
	radiusTopRight(0),
	radiusBottomLeft(0),
	radiusBottomRight(0),
	borderSize(0),
	borderSizeTop(0),
	borderSizeBottom(0),
	borderSizeLeft(0),
	borderSizeRight(0),
	borderColor(0, 0, 0, 1),
	borderColorLeft(0, 0, 0, 1),
	borderColorRight(0, 0, 0, 1),
	borderColorTop(0, 0, 0, 1),
	borderColorBottom(0, 0, 0, 1),
	shadowSize(0),
	shadowHardness(0),
	shadowPosition(0, 0),
	shadowColor(0, 0, 0, 1),
	backgroundFill(FillType::NONE),
	backgroundColor(0, 0, 0, 1),

	graphLineColor(1, 1, 1, 1),
	graphDividerLineColor(1, 1, 1, 1),
	graphRefLineColor(1, 1, 1, 1),
	graphDataLineColor(0, 1, 0, 1),
	graphDataLineColor2(0, 0, 1, 1),
	graphDataLineColor3(1, 0, 0, 1),
	graphDataLineColor4(1, 1, 0, 1),
	graphDataLineColor5(1, 0, 1, 1),
	graphLineStyle(DOTTED),
	graphDividerLineStyle(NO_STRIPPLE),
	graphRefLineStyle(NO_STRIPPLE),
	graphDataLineStyle(NO_STRIPPLE),
	graphDataLineStyle2(NO_STRIPPLE),
	graphDataLineStyle3(NO_STRIPPLE),
	graphDataLineStyle4(NO_STRIPPLE),
	graphDataLineStyle5(NO_STRIPPLE),

	textJustify(TextJusify::CENTER),
	textVerticalAlign(TextVerticalAlign::MIDDLE),

	textPaddingLeft(0),
	textPaddingRight(0),
	textPaddingTop(0),
	textPaddingBottom(0),

	fontName(""),
	fontSize(12),
	fontColor(1, 1, 1, 1),
	fontShadowPosition(),
	fontShadowColor(0, 0, 0, 1),

	_fontValid(0),
	_fontHasShadow(0),

	fontHandle()

{	}

GUIStyle::~GUIStyle()	{	}

void GUIGradientColor::Deserialize(CFONode* node)
{
	node->getValueFloat("angle", angle);

	CFONode* object = node->GetFirstChild();
	while (object) {
		string classname = object->GetName();
		if (classname != "angle") {
			string value = object->GetValue();
			Vector4 color = Utils::StringToVector4(value);
			float pos = (float)atof(classname.c_str());
			add(color, pos);
		}
		object = object->GetNextNode();
	}

}

void GUIStyle::Deserialize(CFONode* node, ResourceManager* resourceManager)
{
	if (node->getValueFloat("radius", radius)) {
		radiusTopLeft = radiusTopRight = radiusBottomLeft = radiusBottomRight = radius;
	}

	node->getValueFloat("radiusTopLeft", radiusTopLeft);
	node->getValueFloat("radiusTopRight", radiusTopRight);
	node->getValueFloat("radiusBottomLeft", radiusBottomLeft);
	node->getValueFloat("radiusBottomRight", radiusBottomRight);

	if (node->getValueFloat("borderSize", borderSize)) {
		borderSizeTop = borderSizeBottom = borderSizeLeft = borderSizeRight = borderSize;
	}

	node->getValueFloat("borderSizeTop", borderSizeTop);
	node->getValueFloat("borderSizeBottom", borderSizeBottom);
	node->getValueFloat("borderSizeLeft", borderSizeLeft);
	node->getValueFloat("borderSizeRight", borderSizeRight);

	if (node->getValueVector4("borderColor", borderColor)) {
		borderColorLeft = borderColorRight = borderColorTop = borderColorBottom = borderColor;
	}

	node->getValueVector4("borderColorLeft", borderColorLeft);
	node->getValueVector4("borderColorRight", borderColorRight);
	node->getValueVector4("borderColorTop", borderColorTop);
	node->getValueVector4("borderColorBottom", borderColorBottom);

	node->getValueFloat("shadowSize", shadowSize);
	node->getValueFloat("shadowHardness", shadowHardness);
	node->getValueVector2("shadowPosition", shadowPosition);
	node->getValueVector4("shadowColor", shadowColor);

	if (node->getValueVector4("backgroundColor", backgroundColor)) {
		backgroundFill = FillType::SOLID;
	}
	else if (node->GetFirstChild("backgroundGradientColor")) {
		backgroundFill = FillType::GRADIENT;
		backgroundGradientColor.Deserialize(node->GetFirstChild("backgroundGradientColor"));
		backgroundGradientColor.generate();
	}
	else {
		backgroundFill = FillType::NONE;
	}

	node->getValueFloat("lineHeight", lineHeight);

	if (node->getValueString("fontName", fontName)) {

		resourceManager->Get(fontHandle, fontName);

		node->getValueFloat("fontSize", fontSize);
		node->getValueVector4("fontColor", fontColor);
		node->getValueVector2("fontShadowPosition", fontShadowPosition);
		if (node->getValueVector4("fontShadowColor", fontShadowColor)) {
			_fontHasShadow = true;
		}

		_fontValid = true;

	}

	//Graph styles
	node->getValueVector4("graphLineColor", graphLineColor);
	node->getValueVector4("graphDividerLineColor", graphDividerLineColor);
	node->getValueVector4("graphRefLineColor", graphRefLineColor);
	node->getValueVector4("graphDataLineColor", graphDataLineColor);
	node->getValueVector4("graphDataLineColor2", graphDataLineColor2);
	node->getValueVector4("graphDataLineColor3", graphDataLineColor3);
	node->getValueVector4("graphDataLineColor4", graphDataLineColor4);
	node->getValueVector4("graphDataLineColor5", graphDataLineColor5);

	string stripName = "";
	if (node->getValueString("graphLineStyle", stripName))graphLineStyle = parseLineStripple(stripName);
	if (node->getValueString("graphDividerLineStyle", stripName))graphDividerLineStyle = parseLineStripple(stripName);
	if (node->getValueString("graphRefLineStyle", stripName))graphRefLineStyle = parseLineStripple(stripName);
	if (node->getValueString("graphDataLineStyle", stripName))graphDataLineStyle = parseLineStripple(stripName);
	if (node->getValueString("graphDataLineStyle2", stripName))graphDataLineStyle2 = parseLineStripple(stripName);
	if (node->getValueString("graphDataLineStyle3", stripName))graphDataLineStyle3 = parseLineStripple(stripName);
	if (node->getValueString("graphDataLineStyle4", stripName))graphDataLineStyle4 = parseLineStripple(stripName);
	if (node->getValueString("graphDataLineStyle5", stripName))graphDataLineStyle5 = parseLineStripple(stripName);

	if (node->getValueString("textAlign", stripName))textJustify = parseTextJustify(stripName);
	if (node->getValueString("textVerticalAlign", stripName))textVerticalAlign = parseTextVerticalAlign(stripName);

	Vector4 padding(0,0,0,0);
	if (node->getValueVector4("textPadding", padding)) {
		textPaddingLeft = padding.x;
		textPaddingRight = padding.y;
		textPaddingTop = padding.z;
		textPaddingBottom = padding.w;
	}
}

GUIStyle::LineStripple GUIStyle::parseLineStripple(string value) {

	if (value == "DOTTED")return DOTTED;
	if (value == "DASHED")return DASHED;

	return NO_STRIPPLE;
}

GUIStyle::TextJusify GUIStyle::parseTextJustify(string value) {

	if (value == "LEFT")return LEFT;
	if (value == "RIGHT")return RIGHT;

	return CENTER;
}
GUIStyle::TextVerticalAlign GUIStyle::parseTextVerticalAlign(string value) {

	if (value == "TOP")return TOP;
	if (value == "BOTTOM")return BOTTOM;
	if (value == "BASELINE")return BASELINE;

	return MIDDLE;
}


#endif
