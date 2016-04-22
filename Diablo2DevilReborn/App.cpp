#include <iostream> 
#include <sstream> 

//#include "SDL\include\SDL.h" //Must remove this include in the future

#include "App.h"
#include "p2Log.h"

#include "FileSystem.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Console.h"
#include "Audio.h"
#include "Map.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "UIManager.h"
#include "FadeToBlack.h"
#include "Attributes.h"

#include "TestingScene.h"
#include "MenuScene.h"

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	LOG("App: Creating.");
	//All modules new and add modules
	fs = new FileSystem();
	win = new Window();
	input = new Input();
	render = new Render();
	map = new Map();
	textures = new Textures();
	
	audio = new Audio();
	eManager = new EntityManager();
	fonts = new Fonts();
	uiManager = new UIManager();

	fade = new FadeToBlack();
	console = new Console();

	tS = new TestingScene();

	addModule(fs);
	addModule(win);
	addModule(input);
	addModule(map);
	addModule(textures);	
	addModule(audio);	
	addModule(fonts);
		
	addModule(uiManager);
	addModule(tS);
	
	addModule(eManager);
	addModule(fade);

	addModule(console);	

	addModule(render); //Must be the last one
	
	
}

// Destructor
App::~App()
{
	LOG("App: Destroying.");
	//Release all modules
	std::list<Module*>::reverse_iterator tmp = modules.rbegin();

	while (tmp != modules.rend())
	{
		RELEASE(*tmp);
		tmp++;
	}
}

/** Add a module to the list initialitzated by default.  */
void App::addModule(Module* module, bool init)
{
	modules.push_back(module);
	if (init == true)
		module->init();
}

// Called before render is available
bool App::awake()
{
	LOG("App: Awake.");

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	config = loadConfig(config_file);

	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.assign(app_config.child("title").child_value());
		organization.assign(app_config.child("organization").child_value());

		int cap = app_config.attribute("framerate_cap").as_int(-1);

		if (cap > 0)
		{
			capped_ms = 1000 / cap;
		}
	}
	
	if(ret == true)
	{
		std::list<Module*>::iterator tmp = modules.begin();
		for (tmp; tmp != modules.end() && ret == true; tmp++)
		{
			ret = tmp._Mynode()->_Myval->awake(config.child(tmp._Mynode()->_Myval->name.c_str()));
		}
	}

	return ret;
}

// Called before the first frame
bool App::start()
{
	LOG("App: Start.");

	bool ret = true;

	std::list<Module*>::iterator tmp = modules.begin();
	for (tmp; tmp != modules.end() && ret == true; tmp++)
	{
		//if (tmp._Mynode()->_Myval->active == true)
		//	ret = tmp._Mynode()->_Myval->start();
		if ((*tmp)->active == true)
			ret = (*tmp)->start();
	}

	return ret;
}

// Called each loop iteration
bool App::update()
{
	bool ret = true;
	
	prepareUpdate();

	if (input->getWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = preUpdate();

	if (ret == true)
		ret = doUpdate();

	if (ret == true)
		ret = postUpdate();

	if (input->getKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		debug = !debug;
	}


	finishUpdate();

	return ret;
}

// ---------------------------------------------
pugi::xml_node App::loadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = app->fs->load("config.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");
	
	return ret;
}

// ---------------------------------------------
void App::prepareUpdate()
{
	frame_count++;
	last_sec_frame_count++;

	dt = frame_time.ReadSec();
	frame_time.Start();
}

// ---------------------------------------------
void App::finishUpdate()
{
	if (wantToSave == true)
		saveGameNow();

	if (wantToLoad == true)
		loadGameNow();

	// Framerate calculations --

	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	float avg_fps = float(frame_count) / startup_time.ReadSec();
	float seconds_since_startup = startup_time.ReadSec();
	uint32 last_frame_ms = frame_time.Read();
	uint32 frames_on_last_update = prev_last_sec_frame_count;

	/*static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %lu ",
		avg_fps, last_frame_ms, frames_on_last_update, dt, seconds_since_startup, frame_count);
	App->win->SetTitle(title);
	*/
	if (capped_ms > 0 && last_frame_ms < capped_ms)
	{
		j1PerfTimer t;
		SDL_Delay(capped_ms - last_frame_ms);
		LOG("We waited for %d milliseconds and got back in %f", capped_ms - last_frame_ms, t.ReadMs());
	}
}

// Call modules before each loop iteration
bool App::preUpdate()
{
	bool ret = true;
	
	std::list<Module*>::iterator tmp = modules.begin();
	for (tmp; tmp != modules.end() && ret == true; tmp++)
	{
		if (tmp._Mynode()->_Myval->active == true)
			ret = tmp._Mynode()->_Myval->preUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::doUpdate()
{
	bool ret = true;
	
	std::list<Module*>::iterator tmp = modules.begin();
	for (tmp; tmp != modules.end() && ret == true; tmp++)
	{
		if (tmp._Mynode()->_Myval->active == true)
			ret = tmp._Mynode()->_Myval->update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::postUpdate()
{
	bool ret = true;
	
	std::list<Module*>::iterator tmp = modules.begin();
	for (tmp; tmp != modules.end() && ret == true; tmp++)
	{
		if (tmp._Mynode()->_Myval->active == true)
			ret = tmp._Mynode()->_Myval->postUpdate();
	}

	return ret;
}

// Called before quitting
bool App::cleanUp()
{
	LOG("App: CleanUp.");

	bool ret = true;

	std::list<Module*>::reverse_iterator tmp = modules.rbegin();
	for (tmp; tmp != modules.rend() && ret == true; tmp++)
	{
		ret = (*tmp)->cleanUp();
	}

	return ret;
}

// ---------------------------------------
int App::getArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::getArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::getTitle() const
{
	return title.c_str();
}

// ---------------------------------------
float App::getDT() const
{
	return dt;
}

// ---------------------------------------
const char* App::getOrganization() const
{
	return organization.c_str();
}

// Load / Save
void App::load_Game(const char* file)
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	wantToLoad = true;

	std::string sFile(fs->getSaveDirectory());
	loadGame.assign(sFile + file);
}

// ---------------------------------------
void App::save_Game(const char* file) const
{
	wantToSave = true;
	saveGame.assign(file);
}

// ---------------------------------------
void App::getSaveGames(std::list<std::string> list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool App::loadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = fs->load(loadGame.c_str(), &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if (result != NULL)
		{
			LOG("Loading new Game State from %s...", loadGame.c_str());

			root = data.child("game_state");

			std::list<Module*>::iterator item = modules.begin();
			ret = true;

			while (item != modules.end() && ret == true)
			{
				ret = item._Mynode()->_Myval->load(root.child(item._Mynode()->_Myval->name.c_str()));
				item++;
			}

			data.reset();
			if (ret == true)
				LOG("...finished loading");
			else
				LOG("...loading process interrupted with error on module %s", (item._Mynode() != NULL) ? item._Mynode()->_Myval->name.c_str() : "unknown");
		}
		else
			LOG("Could not parse game state xml file %s. pugi error: %s", loadGame.c_str(), result.description());
	}
	else
		LOG("Could not load game state xml file %s", loadGame.c_str());

	wantToLoad = false;
	return ret;
}

bool App::saveGameNow() //Must check, it shuold be constant method
{
	bool ret = true;

	LOG("Saving Game State to %s...", saveGame.c_str());
	
	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	std::list<Module*>::iterator item;
	item = modules.begin();

	while (item != modules.end() && ret == true)
	{
		ret = item._Mynode()->_Myval->save(root.append_child(item._Mynode()->_Myval->name.c_str()));
		item++;
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		fs->save(saveGame.c_str(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving", saveGame.c_str());
	}
	else
		LOG("Save process halted from an error in module %s", (item._Mynode() != NULL) ? item._Mynode()->_Myval->name.c_str() : "unknown");

	data.reset();
	wantToSave = false;
	
	return ret;
}