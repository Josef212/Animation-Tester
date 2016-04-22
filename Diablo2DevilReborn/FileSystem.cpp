#include "FileSystem.h"
#include "p2Defs.h"
#include "App.h"
#include "p2Log.h"
#include "PhysFS/include/physfs.h"
#include "SDL/include/SDL.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

FileSystem::FileSystem() : Module()
{
	LOG("FileSystem: Creating. \n");

	name.assign("file_system");

	//Need this before all modules awake
	char* basePath = SDL_GetBasePath();
	PHYSFS_init(basePath);
	SDL_free(basePath);

	// By default we include executable's own directory
	// without this we won't be able to find config.xml
	addPath(".");
}

FileSystem::~FileSystem()
{
	LOG("FileSystem: Destroying.");
	PHYSFS_deinit();
}

bool FileSystem::awake(pugi::xml_node& config)
{
	LOG("FileSystem: Awake.");

	bool ret = true;

	// Add all paths in configuration in order
	for (pugi::xml_node path = config.child("path"); path; path = path.next_sibling("path"))
	{
		addPath(path.child_value());
	}

	// Ask SDL for a write dir
	char* write_path = SDL_GetPrefPath(app->getOrganization(), app->getTitle());

	if (PHYSFS_setWriteDir(write_path) == 0)
		LOG("File System error while creating write dir: %s\n", PHYSFS_getLastError());
	else
	{
		// We add the writing directory as a reading directory too with speacial mount point
		LOG("Writing directory is %s\n", write_path);
		addPath(write_path, getSaveDirectory());
	}

	SDL_free(write_path);

	return ret;
}

bool FileSystem::cleanUp()
{
	LOG("FileSystem: CleanUp.");

	return true;
}

// Add a new zip file or folder
bool FileSystem::addPath(const char* pathOrZip, const char* mountPoint)
{
	bool ret = false;

	if (PHYSFS_mount(pathOrZip, mountPoint, 1) == 0)
		LOG("File System error while adding a path or zip(%s): %s\n", pathOrZip, PHYSFS_getLastError());
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool FileSystem::exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

// Check if a file is a directory
bool FileSystem::isDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

// Read a whole file and put it in a new buffer
unsigned int FileSystem::load(const char* file, char** buffer)const
{
	uint ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openRead(file);

	if (fsFile != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(fsFile);
		
		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 readed = PHYSFS_read(fsFile, *buffer, 1, (PHYSFS_sint32)size);

			if (readed != size)
			{
				LOG("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE(buffer);
			}
			else
				ret = (uint)readed;
		}

		if (PHYSFS_close(fsFile)==0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

SDL_RWops* FileSystem::load(const char* file)const
{
	char* buffer;
	int size = load(file, &buffer);

	if (size > 0)
	{
		SDL_RWops* r = SDL_RWFromConstMem(buffer, size);
		if (r != NULL)
			r->close = closeSdlRwops;

		return r;
	}
	else
		return NULL;
}

int closeSdlRwops(SDL_RWops* rw)
{
	RELEASE(rw->hidden.mem.base);
	SDL_free(rw);
	return 0;
}

// Save a whole buffer to disk
unsigned int FileSystem::save(const char* file, const char* buffer, unsigned int size) const
{
	unsigned int ret = 0;

	PHYSFS_file* fsFile = PHYSFS_openWrite(file);

	if (fsFile != NULL)
	{
		PHYSFS_sint64 written = PHYSFS_write(fsFile, (const void*)buffer, 1, size);

		if (written != size)
			LOG("File System error while writing to file %s: %s\n", file, PHYSFS_getLastError());
		else
			ret = (uint)written;

		if (PHYSFS_close(fsFile) == 0)
			LOG("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}