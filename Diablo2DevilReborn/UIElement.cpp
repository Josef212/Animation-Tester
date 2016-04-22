#include "Textures.h"
#include "App.h"
#include "p2Log.h"
#include "Render.h"
#include "Fonts.h"
#include "Input.h"
#include "UIManager.h"
#include "UIElement.h"
#include "MenuScene.h"

UIElement::UIElement()
{
	rect = { 0, 0, 0, 0 };
}


void UIElement::setLocalPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void UIElement::center()
{
	int frame_w = (parent) ? parent->getLocalRect().w : app->render->camera.w;
	int frame_h = (parent) ? parent->getLocalRect().h : app->render->camera.h;

	setLocalPos(frame_w / 2 - rect.w / 2, frame_h / 2 - rect.h / 2);
}

void UIElement::alignBot()
{
	int frame_w = (parent) ? parent->getLocalRect().w : app->render->camera.w;
	int frame_h = (parent) ? parent->getLocalRect().h : app->render->camera.h;
	
	setLocalPos(frame_w / 2 - rect.w / 2, frame_h - rect.h);
}

void UIElement::alignLeft()
{
	int frame_w = (parent) ? parent->getLocalRect().w : app->render->camera.w;
	int frame_h = (parent) ? parent->getLocalRect().h : app->render->camera.h;

	setLocalPos(0, rect.y);
}


void UIElement::alignRight()
{
	int frame_w = (parent) ? parent->getLocalRect().w : app->render->camera.w;
	int frame_h = (parent) ? parent->getLocalRect().h : app->render->camera.h;
	
	setLocalPos(frame_w - rect.w, rect.y);
}

void UIElement::setSize(int w, int h)
{
	rect.w = w;
	rect.h = h;
}

SDL_Rect UIElement::getLocalRect() const
{
	return rect;
}

SDL_Rect UIElement::getScreenRect() const
{
	if (parent != nullptr)
	{
		iPoint p = getScreenPos();
		return{ p.x, p.y, rect.w, rect.h };
	}
	return rect;
}

iPoint UIElement::getScreenPos() const
{
	if (parent != nullptr)
		return parent->getScreenPos() + iPoint(rect.x, rect.y);
	else
		return iPoint(rect.x, rect.y);
}

iPoint UIElement::getLocalPos() const
{
	return iPoint(rect.x, rect.y);
}

// --------------------------------

void UIElement::debugDraw() const
{
	SDL_Rect r = getScreenRect();
	app->render->drawQuad({ r.x, r.y, r.w, r.h }, 255, (focused) ? 255 : 0, 0, 255, false, false);
	//app->render->drawQuad(rect, 255, 0, 0, 255, false);

}

void UIElement::setListener(Module* module)
{
	if (listener != nullptr)
		listener->onGui(this, UIEvents::LISTENINGENDS);

	listener = module;
}

void UIElement::checkInput(const UIElement* mouse_hover, const UIElement* focus)
{
	bool inside = (mouse_hover == this);

	if (mouse_in != inside)
	{
		mouse_in = inside;
		if (listener != nullptr)
			listener->onGui(this, (inside) ? UIEvents::MOUSEIN : UIEvents::MOUSEOUT);
	}

	if (inside == true)
	{
		if (listener != nullptr)
		{
			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == keyState::KEY_DOWN)
				//listener->onGui(this, UIEvents::MOUSELEFTDOWN);
			app->menuscene->OnGui(this, UIEvents::MOUSELEFTDOWN);

			if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == keyState::KEY_UP)
				listener->onGui(this, UIEvents::MOUSELEFTUP);

			if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == keyState::KEY_DOWN)
				listener->onGui(this, UIEvents::MOUSERIGHTDOWN);

			if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == keyState::KEY_UP)
				listener->onGui(this, UIEvents::MOUSERIGHTUP);
				
		}

		if (draggable == true && app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == keyState::KEY_REPEAT)
		{
			iPoint p = getLocalPos();
			iPoint motion;
			app->input->getMouseMotion(motion.x, motion.y);
			setLocalPos(p.x + motion.x, p.y + motion.y);
		}
	}

	if (focused != (focus == this))
	{
		if (listener != nullptr)
		{
			if (focus == this)
				listener->onGui(this, UIEvents::GAINFOCUS);
			else
				listener->onGui(this, UIEvents::LOSEFOCUS);
				
		}
		focused = (focus == this);
	}

	if (focused == true && listener != nullptr)
	{
		if (app->input->getKey(SDL_SCANCODE_RETURN) == keyState::KEY_DOWN)
			listener->onGui(this, UIEvents::MOUSELEFTDOWN);

		if (app->input->getKey(SDL_SCANCODE_RETURN) == keyState::KEY_UP)
			listener->onGui(this, UIEvents::MOUSELEFTUP);
	}
}

// ---------------------------------

UIImage::UIImage()
{
	section = { 0, 0, 0, 0 };
	type = UIType::IMAGE;
}
UIImage::UIImage(const SDL_Texture* texture) : UIElement(), texture(texture)
{
	section.x = section.y = 0;
	app->textures->getSize(texture, (uint&) section.w, (uint&)section.h);
	setTexture(texture);
	setSize(section.w, section.h);
	type = UIType::IMAGE;
}

UIImage::~UIImage()
{
}

void UIImage::setTexture(const SDL_Texture* text)
{
	texture = text;
}

void UIImage::setSection(const SDL_Rect& section)
{
	this->section = section;
}

SDL_Rect& UIImage::getRect(){
	return (this->section);
}

void UIImage::draw()const
{
	iPoint p = getScreenPos();
	if (parent)
	{
		SDL_Rect r = parent->getScreenRect();
		app->render->setViewPort({ r.x, r.y, r.w, r.h });
		p = getLocalPos();
	}
	SDL_Texture* t = (SDL_Texture*)texture;
	SDL_Rect sect = (SDL_Rect)section;
	Sprite* s = new Sprite(t, p.x, p.y, &sect, 0, 0, 0, 255);
	//app->render->blit(texture, p.x, p.y, (SDL_Rect*)&section, 0.0f);
	app->render->blit(s, true);

	if (parent)
		app->render->resetViewPort();

	//LOG("UI Position: (%d, %d)", rect.x, rect.y);
}

// -------------------

UILabel::UILabel()
{
	text = NULL;
	type = UIType::LABEL;
}

UILabel::~UILabel()
{

}

void UILabel::setText(const char* text)
{
	texture = app->fonts->print(text);
	int w, h;
	app->textures->getSize(texture, (uint&)w, (uint&)h);
	setSize(w, h);
}

void UILabel::draw()const
{

	iPoint p = getScreenPos();
	app->render->blit(texture, p.x, p.y, NULL, 0.0f);

}


// --------------------------


UIButton::UIButton()
{
	type = UIType::BUTTON;
}
UIButton::~UIButton()
{
}

void UIButton::setRect(SDL_Rect r)
{
	setSize(r.w, r.h);
}

UILoadBar::UILoadBar(int value, const SDL_Texture* texture) :
value(value), bar(texture), barSection(bar.getRect())
{
	type = UIType::LOADBAR;
	barSize.x = bar.getRect().w;
	barSize.y = bar.getRect().h;
}

UILoadBar::~UILoadBar()
{
}

void UILoadBar::Update(const UIElement* mouse_hover, const UIElement* focus)
{
	barSection.w = (barSize.x * (actualValuePercentage / 100.0f));
	barSection.h = barSize.y;
	bar.setSection(barSection);
}

void UILoadBar::Length(int actualValue){
	actualValuePercentage = (actualValue * 100) / value;
}

void UILoadBar::Grow(int percentage)
{
	if (actualValuePercentage + percentage >= 100)
	{
		actualValuePercentage = 100;
	}
	else
		actualValuePercentage += percentage;
}

void UILoadBar::Decrease(int percentage)
{
	if (actualValuePercentage - percentage <= 0)
	{
		actualValuePercentage = 0;
	}
	else
		actualValuePercentage -= percentage;
}

void UILoadBar::draw()const
{
	bar.draw();
}