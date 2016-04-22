#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "Module.h"
#include "p2Defs.h"

enum cVarTypes
{
	cFloat,
	cInt,
	cChar,
	cBool
};

// -------------------------------

struct Command
{
public:
	Command()
	{}

	Command(char* str, char* des, uint args, char* newTag = "Miscellaneous")
	{
		commandName = str;
		description = des;
		nArgs = args;
		tag = newTag;
	}

	virtual void function(const std::vector<std::string>* args);

public:
	std::string commandName;
	std::string description;
	std::string tag;

	uint nArgs;
};


// ----------------------------

class CVar
{
public:
	CVar(const char*  _name, float* ref, bool _serialize);
	CVar(const char*  _name, int* ref, bool _serialize);
	CVar(const char*  _name, char* ref, bool _serialize);
	CVar(const char*  _name, bool* ref, bool _serialize);

public:
	bool serialize;

private:
	std::string name;
	std::string description;
	std::string tag;
	cVarTypes type;
	Command* command;
	Module* listener;

	union
	{
		float* fRef;
		int* iRef;
		char* cRef;
		bool* bRef;
	}Reference;

	union
	{
		float fVal;
		int iVal;
		char* cVal;
		bool bVal;
	}Value;

public:
	void linkCommand(Command* _command);

	void set(float val);
	void set(int val);
	void set(char* val);
	void set(bool val);

	void setListener(Module* listener);

	cVarTypes getType();
	std::string getName();
	const Module* getListener()const;

	void read(void* val, cVarTypes expected);
	bool read(float* val);
	bool read(int* val);
	bool read(char* val);
	bool read(bool* val);

	void* forceRead();

	void displayLog();
};


// ---------------------------------

class Console : public Module
{
public:

	Console();

	// Destructor
	virtual ~Console();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Call before first frame
	bool start();

	// Called every frame
	bool update(float dt);

	// Called after all Updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Called when UI event is raised
	//void onGui(Gui* ui, GuiEvents event);

	void addCommand(Command* command);
	uint addCVar(const char* name, float* reference, Module* listener = NULL, bool serialize = false);
	uint addCVar(const char* name, int* reference, Module* listener = NULL, bool serialize = false);
	uint addCVar(const char* name, char* reference, Module* listener = NULL, bool serialize = false);
	uint addCVar(const char* name, bool* reference, Module* listener = NULL, bool serialize = false);

	void getInput(const char* src);

	void output(char* str);

	void open();
	void close();
	void clear();

	void displayCommands(std::string str)const;
	void displayAllCommands()const;
	void displayTags()const;

	bool isActive()const;

	bool saveCVars(pugi::xml_node&)const;
	bool loadCVars(pugi::xml_node&);

private:
	void cCutString(const char* str, std::vector<std::string>* dst);
	Command* findCommand(const char* str, uint nArgs);
	CVar* findCVar(const char* str);
	void setCVar(const char* value);

private:
	std::list<Command*> commandList;
	std::list<CVar*> CVarList;
	std::vector<std::string> tags;
	/*
	//UI
	Gui* BigRectangle;
	Gui* SmallRectangle;
	GuiInputText* InputText = NULL;*/

	//std::vector<GuiLabel*> outputLabbels;

	bool active = false;
	bool dragText = false; //Not sure to use it right now

	int textStart = 0;
	int outputHeight = 0;

	bool closeGame = false;



	struct CQuit : public Command
	{
		CQuit() : Command("quit", "Quit the game.", 0, "Console"){}
		void function(const std::vector<std::string>* arg);
	}cQuit;

	struct CClose : public Command
	{
		CClose() : Command("close", "Close console.", 0, "Console"){}
		void function(const std::vector<std::string>* arg);
	}cClose;


};

#endif