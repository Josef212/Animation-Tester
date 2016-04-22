// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __MODULE_H__
#define __MODULE_H__

#include <string>
#include <list>
#include <vector>
#include "PugiXml\src\pugixml.hpp"

class j1App;
class UIElement;
enum UIEvents;

class Module
{
public:

	Module() : active(false)
	{}

	virtual ~Module()
	{}

	void init()
	{
		active = true;
	}

	void unInit()
	{
		active = false;
	}

	void enable()
	{
		active = true;
		this->start();
	}

	void disable()
	{
		active = false;
		this->cleanUp();
	}

	// Called before render is available
	virtual bool awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool preUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool postUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool cleanUp()
	{
		return true;
	}

	virtual bool load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool save(pugi::xml_node&) const
	{
		return true;
	}

	virtual void onGui(UIElement* ui, UIEvents ev)
	{

	}

public:

	std::string	name;
	bool		active;

};

#endif // __MODULE_H__