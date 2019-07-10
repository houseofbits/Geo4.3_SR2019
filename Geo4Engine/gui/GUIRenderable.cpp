#include "../Geo4.h"

#ifndef NO_OPENGL

GLuint GUIRenderable::stencilIndexCounter = 0;

GUIRenderable::GUIRenderable() : style(0),
	size(),
	vertexArrayId(0),
	bodyIndexArrayId(0),
	bodyNumIndices(0),
	shadowIndexArrayId(0),
	shadowNumIndices(0),
	borderIndexArrayId(0),
	borderNumIndices(0),
	stencilIndex(0)
{


}

GUIRenderable::~GUIRenderable()
{
}

void GUIRenderable::Draw() {

	if (bodyIndexArrayId > 0 && style->backgroundFill == GUIStyle::FillType::GRADIENT) {

		if (shadowIndexArrayId > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowIndexArrayId);
			glDrawElements(GL_TRIANGLES, shadowNumIndices, GL_UNSIGNED_INT, (void*)0);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, GUIGradientColor::gradientTextureMap);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));
		glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 6));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyIndexArrayId);
		glDrawElements(GL_TRIANGLES, bodyNumIndices, GL_UNSIGNED_INT, (void*)0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDisable(GL_TEXTURE_2D);

		if (borderIndexArrayId > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
			glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderIndexArrayId);
			glDrawElements(GL_TRIANGLES, borderNumIndices, GL_UNSIGNED_INT, (void*)0);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
	else {

		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, sizeof(GLfloat) * 8, (void*)0);
		glColorPointer(4, GL_FLOAT, sizeof(GLfloat) * 8, (float*)(sizeof(GLfloat) * 2));

		if (shadowIndexArrayId > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shadowIndexArrayId);
			glDrawElements(GL_TRIANGLES, shadowNumIndices, GL_UNSIGNED_INT, (void*)0);
		}
		if (bodyIndexArrayId > 0 && style->backgroundFill == GUIStyle::FillType::SOLID) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyIndexArrayId);
			glDrawElements(GL_TRIANGLES, bodyNumIndices, GL_UNSIGNED_INT, (void*)0);
		}
		if (borderIndexArrayId > 0) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, borderIndexArrayId);
			glDrawElements(GL_TRIANGLES, borderNumIndices, GL_UNSIGNED_INT, (void*)0);
		}

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	if (style->_fontValid && splitText.size() > 0) {
		glEnable(GL_TEXTURE_2D);
		float offset = 0;
		float line = max(style->lineHeight, textLineHeight);
		for (unsigned int i = 0; i < splitText.size(); i++) {
			Vector2 off = _getTextOffsets(splitText[i], style->textJustify, 
				style->textVerticalAlign, 
				style->textPaddingLeft,
				style->textPaddingRight,
				style->textPaddingTop,
				style->textPaddingBottom);

			if (style->_fontHasShadow) {
				Vector2 pos = off + style->fontShadowPosition + Vector2(0,-offset);
				glPushMatrix();
				glTranslatef(pos.x, pos.y, 0);
				glColor4f(style->fontShadowColor.x, style->fontShadowColor.y, style->fontShadowColor.z, style->fontShadowColor.w);
				style->fontHandle->Draw(splitText[i], (unsigned int)style->fontSize);
				glPopMatrix();
			}
			glPushMatrix();
			Vector2 pos = off + Vector2(0, -offset);
			glTranslatef(pos.x, pos.y, 0);
			glColor4f(style->fontColor.x, style->fontColor.y, style->fontColor.z, style->fontColor.w);
			style->fontHandle->Draw(splitText[i], (unsigned int)style->fontSize);
			glPopMatrix();
			offset += line;
		}
		glDisable(GL_TEXTURE_2D);
	}
}
/*
void GUIRenderable::DrawStaticText(string text, Vector2 pos) {

	if (style->_fontValid) {

		Vector2 off = _getTextOffsets(text) + pos;

		glEnable(GL_TEXTURE_2D);
			if (style->_fontHasShadow) {
				glPushMatrix();
				glTranslatef(off.x + style->fontShadowPosition.x, off.y + style->fontShadowPosition.y, 0);
				glColor4f(style->fontShadowColor.x, style->fontShadowColor.y, style->fontShadowColor.z, style->fontShadowColor.w);

				style->fontHandle->Draw(text, (unsigned int)style->fontSize);
				
				glPopMatrix();
			}
			glPushMatrix();
			glTranslatef(off.x, off.y, 0);
			glColor4f(style->fontColor.x, style->fontColor.y, style->fontColor.z, style->fontColor.w);
			style->fontHandle->Draw(text, (unsigned int)style->fontSize);
			glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
}
*/

void GUIRenderable::DrawStaticText(string text, Vector2 pos, 
	Vector4 color, 
	GUIStyle::TextJusify halign,
	GUIStyle::TextVerticalAlign valign,
	float paddingLeft, 
	float paddingRight,
	float paddingTop,
	float paddingBottom) {

	if (style->_fontValid) {

		Vector2 off =  _getTextOffsets(text, halign, valign, paddingLeft, paddingRight, paddingTop, paddingBottom) + pos;

		glEnable(GL_TEXTURE_2D);
		if (style->_fontHasShadow) {
			glPushMatrix();
			glTranslatef(off.x + style->fontShadowPosition.x, off.y + style->fontShadowPosition.y, 0);
			glColor4f(style->fontShadowColor.x, style->fontShadowColor.y, style->fontShadowColor.z, style->fontShadowColor.w);

			style->fontHandle->Draw(text, (unsigned int)style->fontSize);

			glPopMatrix();
		}
		glPushMatrix();
		glTranslatef(off.x, off.y, 0);
		glColor4f(color.x, color.y, color.z, color.w);
		style->fontHandle->Draw(text, (unsigned int)style->fontSize);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	}
}

Vector2 GUIRenderable::_getTextOffsets(string text, 
	GUIStyle::TextJusify justify, 
	GUIStyle::TextVerticalAlign valign,
	float offsetLeft,
	float offsetRight,
	float offsetTop,
	float offsetBottom) {

	Vector2 off(0,0);

	if (style->_fontValid) {
		float width = style->fontHandle->getWidth(text, (unsigned int)style->fontSize);
		float hw = size.x * 0.5f;
		float hh = size.y * 0.5f;
		switch(justify)
		{
		case GUIStyle::TextJusify::RIGHT:
			off.x = -(width * 0.5f) + hw - offsetRight;
			break;
		case GUIStyle::TextJusify::LEFT:
			off.x = (width * 0.5f) - hw + offsetLeft;
			break;
		default:
			break;
		}
		switch (valign)
		{
		case GUIStyle::TextVerticalAlign::BOTTOM:
			off.y = -style->fontHandle->getVerticalOffset(text, (unsigned int)style->fontSize, TrueTypeFontFace::VecticalAlignment::BOTTOM) - hh + offsetBottom;
			break;
		case GUIStyle::TextVerticalAlign::TOP:
			off.y = -style->fontHandle->getVerticalOffset(text, (unsigned int)style->fontSize, TrueTypeFontFace::VecticalAlignment::TOP) + hh - offsetTop;
			break;
		case GUIStyle::TextVerticalAlign::BASELINE:
			off.y = -style->fontHandle->getVerticalOffset(text, (unsigned int)style->fontSize, TrueTypeFontFace::VecticalAlignment::BASELINE);
			break;
		default:
			off.y = -style->fontHandle->getVerticalOffset(text, (unsigned int)style->fontSize, TrueTypeFontFace::VecticalAlignment::AVERAGE_CENTER);
			break;
		}
	}

	return off;
}

void GUIRenderable::_generateGeometryData() {

//Why do I need to generate gradient map here?
//moved to style Deserialize
//	if (style->backgroundFill == GUIStyle::FillType::GRADIENT) {
//		style->backgroundGradientColor.generate();
//	}

	Vector2 halfSize = size * 0.5;
	Vector2 radiusCenterPoint;
	float radiusShadowOuterEdge = 0;
	float radiusBorderInnerEdge = 0;
	Vector2 cornerBorderInnerEdge;

	vector<Vector2>			bodyOutline;
	vector<Vector2>			shadowInline;
	vector<Vector2>			shadowOutline;
	vector<Vector2>			borderInline;
	vector<bool>			borderChangeColor;

	vector<Vector2>			vertexArray;
	vector<Vector4>			colorArray;
	vector<Vector2>			uvArray;

	vector<unsigned int>	indexArrayBody;
	vector<unsigned int>	indexArrayBorder;
	vector<unsigned int>	indexArrayShadow;
	
	float radiusBody[] = {
		style->radiusTopLeft,
		style->radiusBottomLeft,
		style->radiusBottomRight,
		style->radiusTopRight
	};
	Vector2 corners[] = {
		Vector2(-halfSize.x, halfSize.y),	//top left
		Vector2(-halfSize.x, -halfSize.y),	//bottom left
		Vector2(halfSize.x, -halfSize.y),	//bottom right
		Vector2(halfSize.x, halfSize.y)		//top right
	};
	Vector2 quadrants[] = {
		Vector2(-1, 1),
		Vector2(-1, -1),
		Vector2(1, -1),
		Vector2(1, 1)
	};
	Vector4 borderColors[] = {
		style->borderColorLeft,
		style->borderColorBottom,
		style->borderColorRight,
		style->borderColorTop
	};

	float offx = style->borderSizeLeft - style->borderSizeRight;
	float offy = style->borderSizeBottom - style->borderSizeTop;
	float w = size.x - style->borderSizeLeft - style->borderSizeRight;
	float h = size.y - style->borderSizeBottom - style->borderSizeTop;

	Vector2 borderInnerScale = Vector2(w/size.x, h/size.y);
	Vector2 borderInnerTranslate = Vector2(offx / 2, offy / 2);

	for (int i = 0; i < 4; i++) {

		double step = Math::HALF_PI / max(4.0f, Math::Ceil(Math::HALF_PI * radiusBody[i] / 5));
		
		float shadowOuterSize = style->shadowSize;
		float shadowInnerSize = (style->shadowSize * style->shadowHardness) - 1.0f;

		//1. Generate body siluette conventional way
		if (radiusBody[i] > 0) {
			radiusCenterPoint = corners[i] - Vector2(radiusBody[i] * quadrants[i].x, radiusBody[i] * quadrants[i].y);
			bool changeColor = false;
			for (double u = 0; u <= Math::HALF_PI; u += step) {

				if (u > (Math::HALF_PI * 0.5f) && !changeColor) {
					changeColor = true;
					borderChangeColor.push_back(true);
				}
				else {
					borderChangeColor.push_back(false);
				}

				float ux = (float)cos(Math::HALF_PI + u + (Math::HALF_PI * i));
				float uy = (float)sin(Math::HALF_PI + u + (Math::HALF_PI * i));
				float x = radiusCenterPoint.x + (ux * radiusBody[i]);
				float y = radiusCenterPoint.y + (uy * radiusBody[i]);

				//Body outer edge
				bodyOutline.push_back(Vector2(x, y));

				//Border outer edge
				borderInline.push_back(Vector2(borderInnerTranslate.x + (x * borderInnerScale.x), borderInnerTranslate.y + (y * borderInnerScale.y)));

				//Shadow inner edge
				x = radiusCenterPoint.x + (ux * (radiusBody[i] + shadowInnerSize));
				y = radiusCenterPoint.y + (uy * (radiusBody[i] + shadowInnerSize));
				shadowInline.push_back(Vector2(x, y) + style->shadowPosition);

				//Shadow outer edge
				x = radiusCenterPoint.x + (ux * (radiusBody[i] + shadowOuterSize));
				y = radiusCenterPoint.y + (uy * (radiusBody[i] + shadowOuterSize));
				shadowOutline.push_back(Vector2(x, y) + style->shadowPosition);
			}
		}
		else {
			bodyOutline.push_back(corners[i]);
			borderInline.push_back(Vector2(borderInnerTranslate.x + (corners[i].x * borderInnerScale.x),
				borderInnerTranslate.y + (corners[i].y * borderInnerScale.y)));
			borderChangeColor.push_back(true);
			shadowInline.push_back(Vector2((halfSize.x + shadowInnerSize) * quadrants[i].x, (halfSize.y + shadowInnerSize) * quadrants[i].y) + style->shadowPosition);
			shadowOutline.push_back(Vector2((halfSize.x + shadowOuterSize) * quadrants[i].x, (halfSize.y + shadowOuterSize) * quadrants[i].y) + style->shadowPosition);
		}
	}
	//4. Triangulate body
	if (style->backgroundFill != GUIStyle::FillType::NONE) {
		unsigned int offset = (unsigned int)vertexArray.size();
		unsigned int nexti = 0;
		unsigned int midpointIndex = offset;
		float umin = 0, umax = 1;
		if (style->backgroundFill == GUIStyle::FillType::GRADIENT) {
			umin = style->backgroundGradientColor.textureU;
			umax = style->backgroundGradientColor.textureU1;
		}
		vertexArray.push_back(Vector2(0, 0));
		colorArray.push_back(Vector4(1, 1, 1, 1));
		uvArray.push_back(Vector2(umin + ((umax - umin) * 0.5f), 0.5f));
		for (unsigned int i = 0; i < bodyOutline.size(); i++) {
			nexti = (i + 1) % bodyOutline.size();
			indexArrayBody.push_back(midpointIndex);
			indexArrayBody.push_back(midpointIndex + i + 1);
			indexArrayBody.push_back(midpointIndex + nexti + 1);
			vertexArray.push_back(bodyOutline[i]);
			
			float ux = 0.5f + (bodyOutline[i].x / size.x);
			float uy = 0.5f + (bodyOutline[i].y / size.y);
			ux = umin + ((umax - umin) * ux);
			uvArray.push_back(Vector2(ux,uy));

			if(style->backgroundFill == GUIStyle::FillType::SOLID)
				colorArray.push_back(style->backgroundColor);
			else
				colorArray.push_back(Vector4(1,1,1,1));
		}
		_generateIndexBuffer(indexArrayBody, bodyIndexArrayId, bodyNumIndices);
	}

	//5. Generate shadow geometry
	if (style->shadowSize > 0) {
		unsigned int nexti = 0;
		unsigned int midpointIndex = (unsigned int)vertexArray.size();
		vertexArray.push_back(Vector2(0, 0));
		colorArray.push_back(style->shadowColor);
		uvArray.push_back(Vector2(0, 0));
		unsigned int offset = (unsigned int)vertexArray.size();
		for (unsigned int i = 0; i < shadowInline.size(); i++) {
			vertexArray.push_back(shadowInline[i]);
			vertexArray.push_back(shadowOutline[i]);
			colorArray.push_back(style->shadowColor);
			colorArray.push_back(Vector4(style->shadowColor.xyz(), 0));
			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));
			nexti = (i + 1) % bodyOutline.size();
			indexArrayShadow.push_back(midpointIndex);
			indexArrayShadow.push_back(offset + (i * 2));
			indexArrayShadow.push_back(offset + (nexti * 2));
			indexArrayShadow.push_back(offset + (i * 2));
			indexArrayShadow.push_back(offset + (nexti * 2));
			indexArrayShadow.push_back(offset + (i * 2) + 1);
			indexArrayShadow.push_back(offset + (nexti * 2));
			indexArrayShadow.push_back(offset + (i * 2) + 1);
			indexArrayShadow.push_back(offset + (nexti * 2) + 1);
		}
		_generateIndexBuffer(indexArrayShadow, shadowIndexArrayId, shadowNumIndices);
	}
	
	//6. Triangulate border geometry
	if (style->borderSizeBottom > 0 || 
		style->borderSizeTop > 0 || 
		style->borderSizeLeft > 0 || 
		style->borderSizeRight > 0) {
		unsigned int offset = (unsigned int)vertexArray.size();
		unsigned int nexti = 0;
		unsigned int colorIndex = 3;
		for (unsigned int i = 0; i < bodyOutline.size(); i++) {
			if (borderChangeColor[i]) {
				colorIndex = (colorIndex + 1) % 4;
			}
			nexti = (i + 1) % bodyOutline.size();
			vertexArray.push_back(bodyOutline[i]);
			vertexArray.push_back(borderInline[i]);
			vertexArray.push_back(bodyOutline[nexti]);
			vertexArray.push_back(borderInline[nexti]);

			colorArray.push_back(borderColors[colorIndex]);
			colorArray.push_back(borderColors[colorIndex]);
			colorArray.push_back(borderColors[colorIndex]);
			colorArray.push_back(borderColors[colorIndex]);

			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));
			uvArray.push_back(Vector2(0, 0));

			indexArrayBorder.push_back(offset);
			indexArrayBorder.push_back(offset + 1);
			indexArrayBorder.push_back(offset + 2);
			indexArrayBorder.push_back(offset + 2);
			indexArrayBorder.push_back(offset + 1);
			indexArrayBorder.push_back(offset + 3);
			offset = (unsigned int)vertexArray.size();
		}
		_generateIndexBuffer(indexArrayBorder, borderIndexArrayId, borderNumIndices);
	}

	//7. Generate VBO data
	_generateVertexBuffer(vertexArray, colorArray, uvArray);
}

void GUIRenderable::update() {

	if (style == 0)return;

	_generateGeometryData();
}

void GUIRenderable::_generateVertexBuffer(vector<Vector2>& vertices, vector<Vector4>& colors, vector<Vector2>& uv) {

	if (vertexArrayId > 0)glDeleteBuffers(1, &vertexArrayId);

	unsigned int vertexCount = (unsigned int)vertices.size();

	GLfloat* tmpVertexArray = new GLfloat[8 * vertexCount];

	unsigned int vi = 0;
	for (unsigned int i = 0; i < vertexCount; i++){
		tmpVertexArray[vi] = vertices[i].x;
		tmpVertexArray[vi + 1] = vertices[i].y;
		tmpVertexArray[vi + 2] = colors[i].x;
		tmpVertexArray[vi + 3] = colors[i].y;
		tmpVertexArray[vi + 4] = colors[i].z;
		tmpVertexArray[vi + 5] = colors[i].w;
		tmpVertexArray[vi + 6] = uv[i].x;
		tmpVertexArray[vi + 7] = uv[i].y;
		vi += 8;
	}

	glGenBuffers(1, &vertexArrayId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * vertexCount, NULL, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 8 * vertexCount, tmpVertexArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpVertexArray;
}

void GUIRenderable::_generateIndexBuffer(vector<unsigned int>& indices, GLuint& indexArrayId, unsigned int& indexCount) {

	if (indexArrayId > 0)glDeleteBuffers(1, &indexArrayId);

	indexCount = (unsigned int)indices.size();

	GLuint* tmpIndicesArray = new GLuint[indexCount];
	
	for (unsigned int i = 0; i < indexCount; i++) {
		tmpIndicesArray[i] = indices[i];
	}

	glGenBuffers(1, &indexArrayId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indexCount, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint)*indexCount, tmpIndicesArray);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] tmpIndicesArray;
}

//TODO Split long words
void GUIRenderable::setTextWithWrapping(string text) {
	splitText.clear();
	//cout << "set text: "<<text << endl;
	if(style && style->_fontValid){
		vector<string> words;
		int count = Utils::Explode(' ', text, words);
		if (count > 1) {
			float counter = 0;
			string line = "";
			//cout << "------------split:" << text << endl;
			for (unsigned int i = 0; i < words.size(); i++) {
				//Vector2 s = Vector2();	// style->font.measure(words[i]);
				//textLineHeight = s.y;
				float width = style->fontHandle->getWidth(words[i], (unsigned int)style->fontSize);
				//cout << words[i] << " / " << width <<" / "<< size.x << endl;
				if (counter + width > size.x) {
					splitText.push_back(line);
					line = words[i];
					counter = width;
				}
				else {
					counter += width;
					if (line.size() > 0)line = line + " " + words[i];
					else line = line + words[i];
				}
			}
			if (line.length() > 0)splitText.push_back(line);
		}
		else {
			splitText.push_back(text);
		}
	}
}
void GUIRenderable::setText(string text) {
	splitText.clear();
	splitText.push_back(text);
}

#endif