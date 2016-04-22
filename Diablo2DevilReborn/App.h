#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"
#include "PugiXml\src\pugixml.hpp"

class FileSystem;
class Window;
class Input;
class Render;
class Textures;
class EntityManager;
class Map;
class TestingScene;
class App
{
public:
	
	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool awake();

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool update();

	// Called before quitting
	bool cleanUp();

	// Add a new module to handle
	void addModule(Module* module, bool init = true);

	// Exposing some properties for reading
	int getArgc() const;
	const char* getArgv(int index) const;
	const char* getTitle() const;
	const char* getOrganization() const;
	float getDT() const;

	void load_Game(const char* file);
	void save_Game(const char* file) const;
	void getSaveGames(std::list<std::string> list_to_fill) const;

private:

	// Load config file
	pugi::xml_node loadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void prepareUpdate();

	// Call modules before each loop iteration
	void finishUpdate();

	// Call modules before each loop iteration
	bool preUpdate();

	// Call modules on each loop iteration
	bool doUpdate();

	// Call modules after each loop iteration
	bool postUpdate();

	// Load / Save
	bool loadGameNow();
	bool saveGameNow();

public:

	// Modules
	FileSystem* fs = NULL;
	Window* win = NULL;
	Input* input = NULL;
	Render* render = NULL;
	Textures* textures = NULL;
	EntityManager* eManager = NULL;
	Map* map = NULL;
	TestingScene* tS = NULL;

	bool debug = false;

private:

	std::list<Module*>	modules;
	int					argc;
	char**				args;

	std::string			title;
	std::string			organization;

	mutable bool		wantToSave = false;
	bool				wantToLoad = false;
	std::string			loadGame;
	mutable std::string	saveGame;

	j1PerfTimer			ptimer;
	uint64				frame_count = 0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	float				dt = 0.0f;
	int					capped_ms = -1;
};

extern App* app;

#endif