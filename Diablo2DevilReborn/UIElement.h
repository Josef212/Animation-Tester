#ifndef __UIELEMENT_H__
#define __UIELEMENT_H__

#include "p2Point.h"
#include "Module.h"
#include "SDL\include\SDL.h"

namespace UIType{
	enum UIType
	{
		UNKNOWN,
		IMAGE,
		LABEL,
		BUTTON,
		LOADBAR,
	};
}
enum UIEvents
{
	LISTENINGENDS,
	MOUSEIN,
	MOUSEOUT,
	MOUSERIGHTDOWN,
	MOUSERIGHTUP,
	MOUSELEFTDOWN,
	MOUSELEFTUP,
	GAINFOCUS,
	LOSEFOCUS
};

class UIElement {

public:
	UIElement();
	virtual ~UIElement(){  }

	virtual void draw()const
	{
	}
	virtual void debugDraw()const;
	virtual void update(const UIElement* mouseHover, const UIElement* focus)
	{
	}

	void checkInput(const UIElement* mouseHover, const UIElement* focus);

	void setLocalPos(int x, int y);
	SDL_Rect getScreenRect() const;
	SDL_Rect getLocalRect() const;
	iPoint getScreenPos() const;
	iPoint getLocalPos() const;
	void center();
	void alignBot();
	void alignRight();
	void alignLeft();

	bool			draggable = false;
	bool			draggableX = false;
	bool			draggableY = false;
	bool			interactive = false;
	bool			mouse_in = false;
	bool			focusable = false;
	bool			focused = false;
	UIElement*		parent = NULL;

	void setListener(Module* module);

protected:
	void setSize(int w, int h);

	UIType::UIType			type = UIType::UNKNOWN;
	Module*			listener = NULL;

public:
	SDL_Rect		rect;
		
};

class UIImage : public UIElement
{
private:
	SDL_Rect		section;
	const SDL_Texture*	texture = NULL;

public:
	UIImage();
	UIImage(const SDL_Texture* texture);
	~UIImage();
	void setTexture(const SDL_Texture* texture);
	void setSection(const SDL_Rect &section);
	//void SetImage(const SDL_Rect& section);

	SDL_Rect& getRect();
	
	void draw() const;

};

class UIButton : public UIImage
{
public:
	UIButton();
	~UIButton();

	void setRect(SDL_Rect r);
	//void SetImage(const SDL_Rect& section);

	void update(){  }
};

class UILabel : public UIElement
{
private:
	const char*		text;
	SDL_Texture*	texture = NULL;

public:

	UILabel();
	~UILabel();

	void setText(const char* text);

	void update(){  }
	void draw()const;

};

class UILoadBar : public UIElement
{
public:
	UILoadBar(int value, const SDL_Texture* texture);
	~UILoadBar();

	void Update(const UIElement* mouseHover, const UIElement* focus);
	void draw()const;
	void Grow(int percentage);
	void Decrease(int percentage);
	void Length(int actualValue);

public:
	UIImage bar;
private:
	iPoint barSize;
	int value;	//How much the hole bar represents
	float actualValuePercentage = 100;		//How much the bar each iteration must represent
	SDL_Rect barSection;
};

#endif