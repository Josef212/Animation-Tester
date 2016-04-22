#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"
#include "p2Point.h"

#include "SDL\include\SDL.h"

//enum UIType;
class UIElement;
class UIImage;
class UILabel;
class UIButton;
class UILoadBar;

class UIManager : public Module
{

public: 
	UIManager();
	
	virtual ~UIManager();

	bool awake(pugi::xml_node&);

	bool start();

	bool preUpdate();

	//bool update(float dt);

	bool postUpdate();
	
	bool cleanUp();

	SDL_Texture* getAtlas() const;
	SDL_Texture* getUIAtlas() const;
	const UIElement* findMouseHover();

	UIImage* addImage(SDL_Texture* text);
	UIImage* addImage(SDL_Rect section, Module* listener, UIElement* parent);
	UILabel* addLabel(const char* text, Module* listener, UIElement* parent);
	UIButton* addButton(SDL_Rect rect, Module* listener, UIElement* parent);
	UILoadBar* addLoadBar(const int value, SDL_Texture* text);


	std::list<UIElement*>	UI_Elements;
	
	
private:
	SDL_Texture*			atlas;
	std::string				atlasName;

	SDL_Texture*			uiAtlas;
	std::string				uiAtlasName;

	const UIElement*		focus;
	bool					debug = false;
	


};
#endif