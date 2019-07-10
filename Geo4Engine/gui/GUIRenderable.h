#pragma once

#include <vector>
#include "../ResourceManager.h"

/*
TODO:
- 0 size border geometry generating (triangulation tolerances)
- gradient orientation
- foolproof input corner radiuses
- text justification
*/

class GUIRenderable
{
public:
	GUIRenderable();
	virtual ~GUIRenderable();

	void Draw();
	
	//void DrawStaticText(std::string, Vector2);
	void DrawStaticText(std::string text, Vector2 pos,
							Vector4 color = Vector4(0, 0, 0, 1),
							GUIStyle::TextJusify halign = GUIStyle::TextJusify::CENTER,
							GUIStyle::TextVerticalAlign valign = GUIStyle::TextVerticalAlign::MIDDLE,
							float paddingLeft = 0,
							float paddingRight = 0,
							float paddingTop = 0,
							float paddingBottom = 0);

	void update();
	void setTextWithWrapping(string);
	void setText(string);

	void _generateGeometryData();
	void _generateVertexBuffer(vector<Vector2>&, vector<Vector4>&, vector<Vector2>&);
	void _generateIndexBuffer(vector<unsigned int>&, GLuint&, unsigned int&);

	Vector2	_getTextOffsets(string, GUIStyle::TextJusify, GUIStyle::TextVerticalAlign, float,float,float,float);

	static GLuint stencilIndexCounter;
	static GLuint createStencilIndex() {
		stencilIndexCounter = (stencilIndexCounter + 1) % 0xFF;
		if (stencilIndexCounter == 0)stencilIndexCounter++;
		return stencilIndexCounter;
	}

	vector<string> splitText;

	float		textLineHeight;

	GLuint		stencilIndex;

	GUIStyle*	style;

	Vector2		size;

	GLuint		vertexArrayId;

	GLuint			bodyIndexArrayId;
	unsigned int	bodyNumIndices;

	GLuint			shadowIndexArrayId;
	unsigned int	shadowNumIndices;

	GLuint			borderIndexArrayId;
	unsigned int	borderNumIndices;
};

