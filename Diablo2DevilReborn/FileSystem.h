#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "Module.h"

struct SDL_RWops;

int closeSdlRwops(SDL_RWops* rw);

class FileSystem : public Module
{
public:
	FileSystem();

	virtual ~FileSystem();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before quitting
	bool cleanUp();

	// Utility functions
	bool addPath(const char* path_or_zip, const char* mount_point = NULL);

	bool exists(const char* file) const;

	bool isDirectory(const char* file) const;

	const char* getSaveDirectory() const
	{
		return "save/";
	}

	// Open for Read/Write
	unsigned int load(const char* file, char** buffer) const;

	SDL_RWops* load(const char* file) const;

	unsigned int save(const char* file, const char* buffer, unsigned int size) const;

private:

};



#endif