#ifndef __MAP_H__
#define __MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "Module.h"
#include "p2Defs.h"
#include "SDL\include\SDL.h" //TODO: No se si es massa good fer aixo



struct Properties
{
	std::pair<std::string, int> property;
	std::list<std::pair<std::string, int>>	propertyList;


	~Properties()
	{
		
		for (std::list<std::pair<std::string, int>>::iterator item = propertyList.begin();
			item != propertyList.end(); 
			item++)
		{
			(*item).first.clear();
			//TODO: Crec que falta borrar la parella...
		}

		propertyList.clear();
	}

	int get(const char* name, int default_value = 0);

	
};
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};
struct TileSet
{
	SDL_Rect getTileRect(int id) const;

	std::string				name;
	int						firstgid;
	int						margin;
	int						spacing;
	int						tile_width;
	int						tile_height;
	SDL_Texture*			texture;
	int						tex_width;
	int						tex_height;
	int						num_tiles_width;
	int						num_tiles_height;
	int						offset_x;
	int						offset_y;
};
struct MapData
{
	int						width;
	int						height;
	int						tile_width;
	int						tile_height;
	SDL_Color				background_color;
	std::list<TileSet*>		tilesets;
	std::list<MapLayer*>	layers;
};

class Map : public Module
{
public:
	Map();
	virtual ~Map();

	bool	awake(pugi::xml_node& conf);
	void	draw();
	bool	cleanUp();
	bool	load(const char* path);
	iPoint	mapToWorld(int x, int y) const;
	iPoint	worldToMap(int x, int y) const;
	bool	createWalkabilityMap(int& width, int& height, uchar** buffer);

private:

	bool loadMap();
	bool loadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool loadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool loadLayer(pugi::xml_node& node, MapLayer* layer);
	bool loadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* getTilesetFromTileId(int id);

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded;
};


#endif // __j1MAP_H__