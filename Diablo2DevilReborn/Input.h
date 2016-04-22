#ifndef __INPUT_H__
#define __INPUT_H__

#include "Module.h"
#include "p2Point.h"
#include "SDL/include/SDL_scancode.h"
#include "SDL/include/SDL_mouse.h"

#define NUM_MOUSE_BUTTONS 5

struct SDL_Rect;

enum eventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum keyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class Input : public Module
{

public:

	Input();

	// Destructor
	virtual ~Input();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();

	// Called before quitting
	bool cleanUp();

	// Gather relevant win events
	bool getWindowEvent(eventWindow ev);

	// Check key states (includes mouse and joy buttons)
	keyState getKey(int id) const
	{
		return keyboard[id];
	}

	keyState getMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1];
	}

	void startTextInput(SDL_Rect* rect = NULL, std::string input = "");
	void endTextInput();
	const char* getTextInput(int& cursor, int& selection) const;
	void textInputTooLong();

	// Check if a certain window event happened
	bool getWindowEvent(int code);

	// Get mouse / axis position
	void getMousePosition(int &x, int &y);
	int* getMousePosition();
	void getMouseMotion(int& x, int& y);

	iPoint Input::getMouseWorldPosition();

private:
	bool		windowEvents[WE_COUNT];
	keyState*	keyboard = nullptr;
	keyState	mouseButtons[NUM_MOUSE_BUTTONS];
	int			mouseMotionX = 0;
	int			mouseMotionY = 0;
	int			mouseX = 0;
	int			mouseY = 0;
	// Text Input
	bool		textInput;
	std::string	lastTextInput;
	std::string	lastLastTextInput;
	int			cursorTextInput;
	int			lastCursorTextInput;
	int			selectionTextInput;
};







#endif