
#include "../Geo4.h"

#ifndef NO_OPENGL

//#pragma comment(lib, "../Lib/SDL2/lib/x86/SDL2main.lib")
//#pragma comment(lib, "../Lib/SDL2/lib/x86/SDL2.lib")

#pragma comment(lib, "../Lib/SDL2/lib/x64/SDL2main.lib")
#pragma comment(lib, "../Lib/SDL2/lib/x64/SDL2.lib")

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "../../Lib/SDL2/include/SDL_syswm.h"

#include "../../Lib/SDL2/include/SDL_syswm.h"

using namespace std;

CLASS_DECLARATION(SDLWindow);

/*
IDirectInput8* dev;
static BOOL CALLBACK staticEnumerateGameControllers(LPCDIDEVICEINSTANCE devInst, LPVOID pvRef)
{
	wcout <<"Prod name: "<<devInst->tszProductName << ", " << devInst->tszInstanceName << endl;

	HRESULT result;
	LPDIRECTINPUTDEVICE8 device;
	LPDIRECTINPUTDEVICE8 InputDevice;
	DIPROPGUIDANDPATH dipdw2;

	result = dev->CreateDevice(devInst->guidInstance, &device, NULL);	// IDirectInput8_CreateDevice(dinput, &(pdidInstance->guidInstance), &device, NULL);
	if (FAILED(result)) {
		cout << "failed CreateDevice" << endl;
		return DIENUM_CONTINUE;
	}

	result = device->QueryInterface(IID_IDirectInputDevice8, (LPVOID *)&InputDevice);// IDirectInputDevice8_QueryInterface(device, &IID_IDirectInputDevice8, (LPVOID *)&InputDevice);
	IDirectInputDevice8_Release(device);
	if (FAILED(result)) {
		cout << "failed QueryInterface" << endl;
		return DIENUM_CONTINUE;
	}
	dipdw2.diph.dwSize = sizeof(dipdw2);
	dipdw2.diph.dwHeaderSize = sizeof(dipdw2.diph);
	dipdw2.diph.dwObj = 0; // device property
	dipdw2.diph.dwHow = DIPH_DEVICE;

	result = InputDevice->GetProperty(DIPROP_GUIDANDPATH, &dipdw2.diph);	////IDirectInputDevice8_GetProperty(InputDevice, DIPROP_GUIDANDPATH, &dipdw2.diph);
	IDirectInputDevice8_Release(InputDevice);
	if (FAILED(result)) {
		cout << "failed GetProperty" << endl;
		return DIENUM_CONTINUE; 
	}

	wcout << "GUID Path: "<<dipdw2.wszPath << endl;


	return true;
}/**/

SDLWindow::SDLWindow(void) : Entity(), 
	EventHandler(),
	window_title("GEO4"),
	rotate(0),
	width(1024),
	height(768),
	window(0),
	glcontext(),
	time(0),
	frame_time(0),
	joystick(0),
	guiViewports()
{ 

}

SDLWindow::~SDLWindow(void)
{
	SDL_GL_DeleteContext(glcontext);
	SDL_Quit();
}

void SDLWindow::Initialise(EventManager*const event_manager, ResourceManager*const resourceManager)
{	
	event_manager->RegisterEventHandler(this);
	event_manager->RegisterEventReceiver(this, &SDLWindow::OnWindowEvent);

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);

	cout << "SDL Joysticks: " << SDL_NumJoysticks() << endl;

	if (SDL_NumJoysticks() > 0) {
		joystick = SDL_JoystickOpen(0);

		if (joystick) {
			printf("Opened Joystick 0\n");
			printf(" Name: %s\n", SDL_JoystickNameForIndex(0));
			printf(" Number of Axes: %d\n", SDL_JoystickNumAxes(joystick));
			//printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
			//printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
		}
		else {
			printf("Couldn't open Joystick 0\n");
		}

	}


	Uint32 flags = SDL_WINDOW_OPENGL;

	if (fullscreen == "FULL") {
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}
	else if (fullscreen == "FULL_DESKTOP") {
		flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP;
		width = 0;
		height = 0;
	}

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 6);

	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow(window_title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height,
		flags);

	SDL_GetWindowSize(window, &width, &height);

	glcontext = SDL_GL_CreateContext(window);

	GLenum errr = glewInit();

	SendEvent(new WindowEvent(WindowEvent::WINDOW_CREATED, 0, 0, width, height));

	SendEvent(new WindowEvent(WindowEvent::FRAME_TIME, 0, 0, width, height));

//	Font::m_DefaultInstance.Load("arial.glf");

	glEnable(GL_MULTISAMPLE);

	glClearStencil(0);

	cout << GREEN;
	cout << "OpenGL" << endl;
	cout << " vendor: " << glGetString(GL_VENDOR) << endl;
	cout << " version: " << glGetString(GL_VERSION) << endl;
	cout << " shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << " renderer: " << glGetString(GL_RENDERER) << endl;
	cout << WHITE;

	getObjectsByClassName<GUIViewport>(guiViewports);
}

void SDLWindow::Deserialize(CFONode* node)
{	
	Entity::Deserialize(node);

	node->getValueString("fullscreen", fullscreen);
	node->getValueInt("width", width);
	node->getValueInt("height", height);
	node->getValueString("title", window_title);	
}

bool SDLWindow::OnWindowEvent(WindowEvent*const event)
{
	SceneManager* mgr = getSceneManager();

	if (!mgr)return false;

	float tme = (float)SDL_GetTicks() / 1000.0f;
	frame_time = tme - time;
	time = tme;

	SendEvent(new WindowEvent(WindowEvent::FRAME_TIME, frame_time, time, width, height));

	mgr->event_dispatcher.SetCurrentTime(time);

	SDL_Event sdlevent;

	while (SDL_PollEvent(&sdlevent)) {

		//cout << sdlevent.type << endl;
		
		processGUIEvents(&sdlevent);

		switch(sdlevent.type) {
			case SDL_WINDOWEVENT:
				switch (sdlevent.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
					
						//EXIT
						return false;

						break;
				};
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEWHEEL:
			case SDL_JOYAXISMOTION:

				if (sdlevent.type == SDL_KEYUP && sdlevent.key.keysym.sym == SDLK_F4)return false;

				SendEvent(new InputEvent(new SDL_Event(sdlevent)));

				break;
		};
	};

	//Get all pressed keys
	if (guiViewports.size() > 0) {
		int numKeys;
		const Uint8 *state = SDL_GetKeyboardState(&numKeys);
		if (numKeys > 0) {
			for (int i = 0; i < numKeys; i++) {
				if (state[i] > 0) {
					//cout << "keydown: " << i << endl;
					for (unsigned int a = 0; a < guiViewports.size(); a++) {
						guiViewports[a]->keyPressedEvent(i);
					}
				}
			}
		}
	}
	return true;
}

void SDLWindow::processGUIEvents(SDL_Event* sdlevent) {

	Vector2 mouse_pos;
	for (unsigned int i = 0; i < guiViewports.size(); i++) {

		SDL_Keymod modKeys = SDL_GetModState();
		bool isShift = modKeys & KMOD_SHIFT;
		bool isCtrl = modKeys & KMOD_CTRL;
		bool isAlt = modKeys & KMOD_ALT;
		guiViewports[i]->setModifierKeys(isCtrl, isShift, isAlt);

		bool leftButton = false;
		bool rightButton = false;
		switch (sdlevent->type) {
		case SDL_KEYDOWN:
			guiViewports[i]->keyDownEvent(sdlevent->key.keysym.scancode);
			break;
		case SDL_KEYUP:
			guiViewports[i]->keyUpEvent(sdlevent->key.keysym.scancode);
			break;
		case SDL_MOUSEMOTION:
			if (sdlevent->button.button == SDL_BUTTON_LEFT)leftButton = true;
			if (sdlevent->button.button == SDL_BUTTON_RIGHT)rightButton = true;
			guiViewports[i]->mouseMoveEvent(Vector2((float)sdlevent->motion.x, height - (float)sdlevent->motion.y),
				Vector2((float)sdlevent->motion.xrel, -(float)sdlevent->motion.yrel),
				leftButton, 
				rightButton);
			break;
		case SDL_MOUSEBUTTONDOWN:			
			if (sdlevent->button.button == SDL_BUTTON_LEFT)leftButton = true;
			if (sdlevent->button.button == SDL_BUTTON_RIGHT)rightButton = true;
			guiViewports[i]->mouseButtonDownEvent(Vector2((float)sdlevent->motion.x, height - (float)sdlevent->motion.y), leftButton, rightButton);
			break;
		case SDL_MOUSEBUTTONUP:
			if (sdlevent->button.button == SDL_BUTTON_LEFT)leftButton = true;
			if (sdlevent->button.button == SDL_BUTTON_RIGHT)rightButton = true;
			guiViewports[i]->mouseButtonUpEvent(Vector2((float)sdlevent->motion.x, height - (float)sdlevent->motion.y), leftButton, rightButton);
			break;

		case SDL_TEXTINPUT:
			guiViewports[i]->textEnterEvent(string(sdlevent->text.text));
			break;
		};
	}

}

void SDLWindow::PreRender(Renderer*){		}

void SDLWindow::Render(Renderer* rnd){		}

void SDLWindow::PostRender()
{
	SDL_GL_SwapWindow(window);
}

#endif