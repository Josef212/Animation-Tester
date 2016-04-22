#include "Console.h"

#include "p2Log.h"
#include "App.h"
#include "Input.h"


// -----------Console----------------
Console::Console()
{
	LOG("Console: Creating");

	name.assign("console");
}

Console::~Console()
{
	LOG("Console: Destroying.");
}

bool Console::awake(pugi::xml_node& config)
{
	LOG("Console: Awake.");

	bool ret = true;

	//Add commands here
	addCommand(&cQuit);
	addCommand(&cClose);

	return ret;
}

bool Console::start()
{
	LOG("Console: Start.");

	bool ret = true;

	// Create ui console here


	return ret;
}

bool Console::cleanUp()
{
	LOG("Console: CleanUp");
	bool ret = true;

	return ret;
}

bool Console::update(float dt)
{
	bool ret = true;

	if (app->input->getKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (isActive())
			close();
		else
			open();
	}

	return ret;
}

bool Console::postUpdate()
{
	bool ret = true;

	ret = !closeGame;

	return ret;
}

/*bool Console::onUi(Gui* ui, GuiEvents event)
{
	if (ui == inputText)
	{
		if (event == GuiEvents::return_down)
		{
			GuiInputText* input = (GuiInputText*)ui;
			getInput(input->GetString());
			InputText->Clear();
		}
	}
}*/

void Console::addCommand(Command* command)
{
	commandList.push_back(command);

	bool found = false;
	for (std::vector<std::string>::iterator i = tags.begin(); i != tags.end(); ++i)
	{
		if (command->tag == *i)
			found = true;
	}
	if (!found)
		tags.push_back(command->tag);
}

uint Console::addCVar(const char* name, float* reference, Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->setListener(listener);
	CVarList.push_back(ret);
	return CVarList.size() - 1;
}

uint Console::addCVar(const char* name, int* reference, Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->setListener(listener);
	CVarList.push_back(ret);
	return CVarList.size() - 1;
}

uint Console::addCVar(const char* name, char* reference, Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->setListener(listener);
	CVarList.push_back(ret);
	return CVarList.size() - 1;
}

uint Console::addCVar(const char* name, bool* reference, Module* listener, bool serialize)
{
	CVar* ret = new CVar(name, reference, serialize);
	ret->setListener(listener);
	CVarList.push_back(ret);
	return CVarList.size() - 1;
}

void Console::getInput(const char* src)
{
	std::vector<std::string> sInput;
	cCutString(src, &sInput);
	std::string cName = *sInput.begin();


	Command* com = findCommand(cName.c_str(), sInput.size() - 1);
	if (com)
	{
		com->function(&sInput);
	}
	else
	{
		CVar* cVar = findCVar(cName.c_str());
		if (cVar)
		{
			if (sInput.size() == 1)
			{
				cVar->displayLog();
			}
			else if (sInput.size() == 2 || sInput.size() == 3)
			{
				if (sInput.size() == 3)
				{
					std::string ser = *sInput.end();
					cVar->serialize = std::stoi(ser.c_str());
				}
				std::string* str = &sInput[1];
				cVar->set(str);
			}
			else
			{
				LOG("Sent too many arguments");
			}
		}
		else
		{
			LOG("'%s' is not an intern command.", sInput[0].c_str());
		}
	}
	sInput.clear();
}

void Console::output(char* str)
{

}

void Console::open()
{
	active = true;
	// Active all ui elements and focus input text
}

void Console::close()
{
	active = false;
	// Enable all ui elements and clear input text
}

void Console::clear()
{
	//outputLabbels.clear();
}

void Console::displayCommands(std::string str)const
{

}

void Console::displayAllCommands()const
{

}

void Console::displayTags()const
{

}

bool Console::isActive()const
{
	return active;
}

bool Console::saveCVars(pugi::xml_node&)const
{
	bool ret = true;

	return ret;
}

bool Console::loadCVars(pugi::xml_node&)
{
	bool ret = true;

	return ret;
}

void Console::cCutString(const char* str, std::vector<std::string>* dst)
{
	std::string strr(str);
	std::string tmp;

	bool onComas = false;

	for (uint i = 0; i < strr.size(); i++)
	{
		if (!onComas && strr.c_str()[i] == ' ')
		{
			tmp = strr;
			cutString2(tmp, i, 0);
			dst->push_back(tmp);
			cutString2(strr, 0, i);
			i = 0;
		}

		if (strr.c_str()[i] == '"')
		{
			onComas = !onComas;
			if (onComas)
			{
				cutString2(strr, 0, 0);
				i--;
			}
			else
			{
				cutString2(strr, i, i);
				i--;
			}
		}
	}
	dst->push_back(strr);
}

Command* Console::findCommand(const char* str, uint nArgs)
{
	std::list<Command*>::iterator item = commandList.begin();
	Command* ret = NULL;

	for (; item != commandList.end() && !ret; ++item)
	{
		if ((*item)->commandName == str)
		{
			ret = *item;
		}
	}

	if (ret)
	{
		if (nArgs > ret->nArgs)
		{
			LOG("Too many arguments. Command %s must recive %i arguments and you sent %i arguments.", (*ret).commandName, (*ret).nArgs, nArgs);
			ret = NULL;
		}
	}

	return ret;
}

CVar* Console::findCVar(const char* str)
{
	CVar* ret = NULL;
	std::list<CVar*>::iterator tmp = CVarList.begin();

	while (tmp != CVarList.end())
	{
		if ((*tmp)->getName() == str)
		{
			ret = *tmp;
			break;
		}
		++tmp;
	}
	return ret;
}

void Console::setCVar(const char* calue)
{

}


// -----------Command----------------
void Command::function(const std::vector<std::string>* args)
{
	LOG("Executing command function!");
}


// ----------CVar--------------------
CVar::CVar(const char*  _name, float* ref, bool _serialize)
{
	name = _name;
	type = cFloat;
	serialize = _serialize;
	Reference.fRef = ref;
	Value.fVal = *ref;
}

CVar::CVar(const char*  _name, int* ref, bool _serialize)
{
	name = _name;
	type = cInt;
	serialize = _serialize;
	Reference.iRef = ref;
	Value.iVal = *ref;
}

CVar::CVar(const char*  _name, char* ref, bool _serialize)
{
	name = _name;
	type = cChar;
	serialize = _serialize;
	Reference.cRef = ref;
	Value.cVal = ref;
}

CVar::CVar(const char*  _name, bool* ref, bool _serialize)
{
	name = _name;
	type = cBool;
	serialize = _serialize;
	Reference.bRef = ref;
	Value.bVal = *ref;
}

void CVar::linkCommand(Command* _command)
{
	command = _command;
}

void CVar::set(float val)
{
	if (type == cFloat)
	{
		*Reference.fRef = val;

		LOG("Set CVar %s to %f", name.c_str(), val);
		if (serialize)
		{
			Value.fVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->function(NULL);
		}
	}
}

void CVar::set(int val)
{
	if (type == cInt)
	{
		*Reference.iRef = val;

		LOG("Set CVar %s to %f", name.c_str(), val);
		if (serialize)
		{
			Value.iVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->function(NULL);
		}
	}
}

void CVar::set(char* val)
{
	if (type == cChar)
	{
		Reference.cRef = val;

		LOG("Set CVar %s to %f", name.c_str(), val);
		if (serialize)
		{
			Value.cVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->function(NULL);
		}
	}
}

void CVar::set(bool val)
{
	if (type == cBool)
	{
		*Reference.bRef = val;

		LOG("Set CVar %s to %f", name.c_str(), val);
		if (serialize)
		{
			Value.bVal = val;
			LOG("Will be serialize");
		}
		if (command)
		{
			command->function(NULL);
		}
	}
}

void CVar::setListener(Module* _listener)
{
	listener = _listener;
}

cVarTypes CVar::getType()
{
	return type;
}

std::string CVar::getName()
{
	return name;
}

const Module* CVar::getListener()const
{
	return listener;
}

void CVar::read(void* val, cVarTypes expected)
{
	if (type == expected)
	{
		val = &Value;
	}
	else
	{
		LOG("Incorrect expected type!");
	}
}

bool CVar::read(float* val)
{
	if (type == cFloat)
	{
		val = &Value.fVal;
		return true;
	}
	else
	{
		return false;
	}
}

bool CVar::read(int* val)
{
	if (type == cInt)
	{
		val = &Value.iVal;
		return true;
	}
	else
	{
		return false;
	}
}

bool CVar::read(char* val)
{
	if (type == cChar)
	{
		val = Value.cVal;
		return true;
	}
	else
	{
		return false;
	}
}

bool CVar::read(bool* val)
{
	if (type == cBool)
	{
		val = &Value.bVal;
		return true;
	}
	else
	{
		return false;
	}
}

void* CVar::forceRead()
{
	return &Value;
}

void CVar::displayLog()
{
	switch (type)
	{
	case cFloat:
		LOG("Float value: %f", Value.fVal);
		break;
	case cInt:
		LOG("Int value: %i", Value.iVal);
		break;
	case cChar:
		LOG("String value: %s", Value.cVal);
		break;
	case cBool:
		LOG("Bool value: %b", Value.bVal);
		break;
	default:
		break;
	}
}

void Console::CQuit::function(const std::vector<std::string>* arg)
{
	app->console->closeGame = true;
}

void Console::CClose::function(const std::vector<std::string>* arg)
{
	app->console->close();
}