/*
Geo4 engine

	2017/01/01	
		Refactoring core classes
	2019/03/20
		Creating GUI classes

*/
#pragma once

//#define NO_OPENGL			//Compile with no OpenGL. Basic console window

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <list>
#include <vector>

#ifndef NO_OPENGL
	#include "../Lib/GLEW/include/GL/glew.h" 
	#include <gl/GL.h>
	#include <gl/GLU.h>
#endif

	#include "math/HMath.h"
	#include "math/HVector2.h"
	#include "math/HVector3.h"
	#include "math/HMatrix4.h"
	#include "math/HColor.h"
	#include "StringUtils.h"
	#include "Serial.h"
	#include "CFODocument.h"
	#include "ObjectFactory.h"
	#include "ResourceManager.h"
	#include "Entity.h"
	#include "EventReceiver.h"
	#include "events/SceneManagerEvent.h"
	#include "objects/SceneManager.h"
	#include "events/WindowEvent.h"

#ifndef NO_OPENGL
	#include "Renderer.h"
	#include "gui/GUIStyle.h"
	#include "gui/GUIRenderable.h"	
	#include "events/InputEvent.h"
	#include "gui/GUIEvent.h"
	#include "gui/GUIInputEvent.h"
	#include "objects/Window.h"
	#include "objects/Viewport.h"
	#include "gui/GUIEntity.h"
	#include "gui/GUIViewport.h"
	#include "gui/GUIWindow.h"
	#include "gui/GUIButton.h"
	#include "gui/GUIText.h"
	#include "gui/GUITextInput.h"
	#include "gui/GUIList.h"
	#include "gui/GUIListItem.h"
	#include "gui/GUILayer.h"
	#include "gui/GUISlider.h"	
	#include "gui/GUIColorPicker.h"
	#include "gui/GUIGraph.h"	
	#include "gui/GUIWings.h"	
	#include "TrueTypeFont.h"
	#include "gui/GUIStyleResource.h"
	#include "Interface.h"
#endif
	
	#include "DataPacketParse.h"
	#include "DataPacketRequest.h"	
//	#include "ExturbatorRequests.h"
//	#include "Hardware.h"


