#pragma once
#include "Common.h"
#include "Camera.h"
#include "Exception.h"
class GLHelper
{
private:
	int w_width;
	int w_height;
	int mouseX;
	int mouseY;
	int WindowHandle;
	unsigned frameCount;

	// singleton
	GLHelper(void);

	GLHelper(GLHelper const&); 
	void operator=(GLHelper const&);
public:
	void (__cdecl *DRAW)();

	Camera* currentCamera;
	void setCamera(Camera* cameraPointer){
		currentCamera = cameraPointer;
	}
	void createNewCamera(){
		currentCamera = new Camera();
		currentCamera->translate(0.f, 0.f, -100.0f);
	}

	static GLHelper& getInstance()
	{
		static GLHelper instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}


	void InitGL(GLvoid);
	void BindGUIFunctions(void);

	/**
	*	Toggles
	*/
	void ToggleDepth();

	void increaseFrameCount(){
		this->frameCount++;
	}

	// obsolete
	//static void Draw();

	static void TimerCallback(int value);
	static void KeyF(unsigned char key, int x, int y);
	static void SpecialKeyF(int key, int x, int y);
	static void IdleFunction(void);
	static void ResizeFunction(int Width, int Height);
	static void Mouse(int x, int y);
	static void timerCallback(int value);

	/**
	 *	Define your Draw function in another place and pass to this.
	 *
	 *  I'm happy to use this at least :)
	 */
	//void DRAWConvention(void (__cdecl *function)());
};

