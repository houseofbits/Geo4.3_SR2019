#pragma once

#include <cmath>
#include <map>
#include <list>
#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>

#include "Math/HVector2.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H

class TrueTypeActiveGlyph {
public:
	TrueTypeActiveGlyph():fontSize(0),
		charIndex(0),
		uvPos(0,0),
		uvSize(0,0),
		size(0,0),
		bearing(0,0),
		advance(0)
	{	}
	TrueTypeActiveGlyph(unsigned int charIndex) : charIndex(charIndex){
		TrueTypeActiveGlyph();
	}
	unsigned int fontSize;
	unsigned int charIndex;
	Vector2 uvPos;
	Vector2 uvSize;
	Vector2 size;
	Vector2 bearing;
	float	advance;
};

/*
Init
1.Load true type font face
2.Create empty texture of predefined size (white, opaque)
3.Initialize vertex and index buffers

Rendering
1.Get text char indexes
2.Get character from cache
3.Add character to cache
	3.1 Render true type gluph
	3.2 Convert it to texture alpha
	3.3 Blit it into texture and increment position
5.Update vertex vbo
6.Render vbo (numIndices = char count)
*/

class TrueTypeFontFace : public Resource
{
public:
	TrueTypeFontFace();
	virtual ~TrueTypeFontFace();

	enum VecticalAlignment{
		BASELINE,
		BOTTOM,
		TOP,
		AVERAGE_CENTER,
	};

	bool Load(std::string filename);
	void	Unload() {
		if (glIsTexture(faceTextureMap)) {
			glDeleteTextures(1, &faceTextureMap);
		}
	}

	std::string		getExtenionStr(unsigned int n = 0) { return "ttf"; }
	unsigned int	getNumExt() { return 1; }

	void Draw(std::string, unsigned int);
	void DrawCached();

	float getWidth(std::string, unsigned int);
	float getVerticalOffset(std::string, unsigned int, VecticalAlignment);

	bool _addGlyph(unsigned int charIndex, unsigned int size, TrueTypeActiveGlyph&);
	bool _getGlyph(unsigned int charIndex, unsigned int size, TrueTypeActiveGlyph& out);
	void _createEmptyTextureMap();
	void _createBufferObjects();
	void _setSize(unsigned int);

	void _drawCharacterMap();

	//Glyph map
	std::vector<TrueTypeActiveGlyph> activeGlyphs;	
	unsigned int	textureMapCurrentLineU;
	unsigned int	textureMapCurrentLineV;
	unsigned int	textureMapNextLineV;
	GLuint			faceTextureMap;
	FT_Face			ftFace;

	//Text rendering
	unsigned int	currentIndexCount;
	GLuint			indexArrayId;
	GLuint			vertexArrayId;
	GLfloat*		tmpVertexArray;
	
	const static unsigned int maxCharactersCount;
	const static unsigned int textureMapWidth;
	const static unsigned int textureMapHeight;
	const static unsigned int fontMapPadding;

	static TrueTypeFontFace m_DefaultInstance;
};

typedef ResourceHandle<TrueTypeFontFace> TrueTypeFontFaceHandle;