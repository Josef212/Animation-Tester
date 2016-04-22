#include "p2Defs.h"
#include "p2Log.h"
#include "UIElement.h"
#include "UIManager.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"

UIManager::UIManager() : Module()
{
	LOG("UIManager: Creating.");
	name.assign("uiManager");
}

UIManager::~UIManager()
{

}

bool UIManager::awake(pugi::xml_node& config)
{
	LOG("UIManager: Awake.");
	bool ret = true;

	atlasName = config.child("atlas").attribute("file").as_string();
	uiAtlasName = config.child("uiAtlas").attribute("file").as_string();

	return ret;
}

bool UIManager::start()
{
	atlas = app->textures->load(atlasName.c_str());
	uiAtlas = app->textures->load(uiAtlasName.c_str());

	return true;
}

bool UIManager::preUpdate()
{
	if (app->input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	const UIElement* mouseHover = findMouseHover();

	if (mouseHover && mouseHover->focusable == true && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == keyState::KEY_DOWN)
		focus = mouseHover;

	//TBD Tab system

	std::list<UIElement*>::iterator item = UI_Elements.begin();

	for (; item != UI_Elements.end(); ++item) 
		if ((*item)->interactive == true)
			(*item)->checkInput(mouseHover, focus);

	item = UI_Elements.begin();

	for (; item != UI_Elements.end(); ++item)
		(*item)->update(mouseHover, focus);

	return true;

}


const UIElement* UIManager::findMouseHover()
{
	iPoint mouse;
	app->input->getMousePosition(mouse.x, mouse.y);

/*	std::list<UIElement*>::iterator item = --UI_Elements.end();
	if (UI_Elements.size() > 0)
	{	
		for (; (*item); --item)
		{
			if ((*item)->interactive == true)
			{
				SDL_Rect r = (*item)->getScreenRect();
				if ((mouse.x >= r.x) && (mouse.x < (r.x + r.w)) && (mouse.y >= r.y) && (mouse.y < (r.y + r.h)))
					return (*item);
			}
		}
	}
	*/
	int i = 0;
	for (std::list<UIElement*>::iterator item = --UI_Elements.end(); i<UI_Elements.size(); --item){
		if ((*item)->interactive == true)
		{
			SDL_Rect r = (*item)->getScreenRect();
			if ((mouse.x >= r.x) && (mouse.x < (r.x + r.w)) && (mouse.y >= r.y) && (mouse.y < (r.y + r.h)))
				return (*item);
		}
		i++;
	}

	return NULL;
}

bool UIManager::postUpdate()
{
	std::list<UIElement*>::iterator item = UI_Elements.begin();

	for (; item != UI_Elements.end(); ++item)
	{
		(*item)->draw();
		if (app->debug)
			(*item)->debugDraw();

	}
	return true;
}

bool UIManager::cleanUp()
{
	LOG("Freeing UI");

	std::list<UIElement*>::iterator item = UI_Elements.begin();

	while (item != UI_Elements.end())
	{
		RELEASE((*item));
		item++;
	}

	UI_Elements.clear();

	return true;

}

SDL_Texture* UIManager::getAtlas() const
{
	return atlas;
}

SDL_Texture* UIManager::getUIAtlas() const
{
	return uiAtlas;
}

UIImage* UIManager::addImage(SDL_Texture* text)
{
	UIImage* ret = NULL;

	if (text != NULL)
	{
		ret = new UIImage(text);
		UI_Elements.push_back(ret);
	}

	return ret;
}

UIImage* UIManager::addImage(SDL_Rect section, Module* listener, UIElement* parent)
{
	UIImage* ret = new UIImage();

	ret->setTexture(uiAtlas);
	ret->setSection(section);
	ret->setListener(listener);
	ret->parent = parent;
	ret->rect.w = section.w;
	ret->rect.h = section.h;
	UI_Elements.push_back(ret);
	
	return ret;

}
UILabel* UIManager::addLabel(const char* text, Module* listener, UIElement* parent)
{
	UILabel* ret = new UILabel();

	ret->setText(text);
	ret->setListener(listener);
	ret->parent = parent;
	UI_Elements.push_back(ret);

	return ret;

}

UIButton*  UIManager::addButton(SDL_Rect section, Module* listener, UIElement* parent)
{
	UIButton* ret = new UIButton();
	ret->setTexture(uiAtlas);
	ret->setSection(section);
	ret->parent = parent;
	ret->setListener(listener);
	ret->setRect(section);
	ret->interactive = true;
	ret->focusable = true;
	UI_Elements.push_back(ret);
	return ret;
}

UILoadBar* UIManager::addLoadBar(const int value, SDL_Texture* text){
	UILoadBar* ret = NULL;

	ret = new UILoadBar(value, text);
	UI_Elements.push_back(ret);

	return ret;
}

