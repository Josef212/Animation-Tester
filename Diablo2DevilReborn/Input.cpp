#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Input.h"
#include "Window.h"
#include "SDL/include/SDL.h"
#include "Render.h"

#define MAX_KEYS 300

Input::Input() : Module()
{
	LOG("Input: Creating.");

	name.assign("input");

	keyboard = new keyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(keyState) * MAX_KEYS);
	memset(mouseButtons, KEY_IDLE, sizeof(keyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
Input::~Input()
{
	LOG("Input: Destroying");
	delete[] keyboard;
}

// Called before render is available
bool Input::awake(pugi::xml_node& config)
{
	LOG("Input: Awake.");
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Input::start()
{
	LOG("Input: Start.");
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool Input::preUpdate()
{
	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouseButtons[i] == KEY_DOWN)
			mouseButtons[i] = KEY_REPEAT;

		if (mouseButtons[i] == KEY_UP)
			mouseButtons[i] = KEY_IDLE;
	}

	mouseMotionX = mouseMotionY = 0;

	lastLastTextInput = lastTextInput;
	lastCursorTextInput = cursorTextInput;

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		//   Too fix cut method
		case SDL_KEYDOWN:
		{
			// Special case of micro controlling text input
			// TODO
			if (textInput == true)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_BACKSPACE:
					cutString(lastTextInput, cursorTextInput - 1);
					if (cursorTextInput > 0)
						cursorTextInput--;
					break;
				case SDLK_DELETE:
					if (cursorTextInput < lastTextInput.length())
						cutString(lastTextInput, cursorTextInput);
					break;
				case SDLK_KP_ENTER:
				case SDLK_RETURN2:
				case SDLK_RETURN:
					selectionTextInput = 1;
					break;
				case SDLK_LEFT:
					if (cursorTextInput > 0)
						cursorTextInput--;
					break;
				case SDLK_RIGHT:
					if (cursorTextInput < lastTextInput.length())
						cursorTextInput++;
					break;
				case SDLK_HOME:
					cursorTextInput = 0;
					break;
				case SDLK_END:
					cursorTextInput = lastTextInput.length();
					break;
				}
			}

			//LOG("Key %d changes state to %d", code, state);
		}
		break;

		// TODO 4: Capture SDL_TEXTINPUT event. You can ignore SDL_TEXTEDITING for now
		// read and store what you receive so you can return it
		case SDL_TEXTINPUT:
			lastTextInput.insert(cursorTextInput, event.text.text);
			cursorTextInput += strlen(event.text.text);
			//LOG("Input event: %s", event.edit.text);
			break;

			// TODO
		case SDL_TEXTEDITING:
			//last_text_input = event.edit.text;
			//cursor_text_input = event.edit.start;
			//selection_text_input = event.edit.length;
			//LOG("Edit event: %s cursor %d selection %d", event.edit.text, event.edit.start, event.edit.length);
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouseButtons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouseButtons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_MOUSEMOTION:
			int scale = app->win->getScale();
			mouseMotionX = event.motion.xrel / scale;
			mouseMotionY = event.motion.yrel / scale;
			mouseX = event.motion.x / scale;
			mouseY = event.motion.y / scale;
			//LOG("Mouse motion x %d y %d", mouse_motion_x, mouse_motion_y);
			break;


		}
	}

	return true;
}

// Called before quitting
bool Input::cleanUp()
{
	LOG("Input: CleanUp.");
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool Input::getWindowEvent(eventWindow ev)
{
	return windowEvents[ev];
}

void Input::getMousePosition(int& x, int& y)
{
	x = mouseX;
	y = mouseY;
}
int* Input::getMousePosition()
{
	int position[2];
	position[0] = mouseX;
	position[1] = mouseY;

	return position;
}


void Input::getMouseMotion(int& x, int& y)
{
	x = mouseMotionX;
	y = mouseMotionY;
}

// TODO 3: Add method to start / stop / get text from SDL_TextInput
void Input::startTextInput(SDL_Rect* rect, std::string input)
{
	textInput = true;
	SDL_StartTextInput();
	lastTextInput = input;
	cursorTextInput = lastTextInput.length();

	if (rect != NULL)
		SDL_SetTextInputRect(rect);
}

void Input::endTextInput()
{
	SDL_StopTextInput();
	textInput = false;
	//last_text_input.Clear();;
}

const char* Input::getTextInput(int& cursor, int& selection) const
{
	cursor = cursorTextInput;
	selection = selectionTextInput;

	return lastTextInput.c_str();
}

void Input::textInputTooLong()
{
	lastTextInput = lastTextInput;
	cursorTextInput = lastCursorTextInput;
}

iPoint Input::getMouseWorldPosition()
{
	return{ mouseX - app->render->camera.x , mouseY - app->render->camera.y };
}