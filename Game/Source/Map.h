#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"
#include "Pathfinding.h"

#include "PugiXml\src\pugixml.hpp"

//Define a property to store the MapType and Load it from the map
enum MapOrientation
{
    ORTOGRAPHIC = 0,
    ISOMETRIC
};

//Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
    int firstgid;
    SString name;
    int tilewidth;
    int tileheight;
    int spacing;
    int margin;
    int tilecount;
    int columns;

    SDL_Texture* texture;

    //Implement the method that receives the gid and returns a Rect

    SDL_Rect GetRect(uint gid) {
        SDL_Rect rect = { 0 };

        int relativeIndex = gid - firstgid;
        rect.w = tilewidth;
        rect.h = tileheight;
        rect.x = margin + (tilewidth + spacing) * (relativeIndex % columns);
        rect.y = margin + (tileheight + spacing) * (relativeIndex / columns);

        return rect;
    }
};

//Add attributes to the property structure
struct Properties
{
    struct Property
    {
        SString name;
        bool value; //We assume that we are going to work only with bool for the moment
    };

    List<Property*> propertyList;

    ~Properties()
    {
        //...
        ListItem<Property*>* property;
        property = propertyList.start;

        while (property != NULL)
        {
            RELEASE(property->data);
            property = property->next;
        }

        propertyList.Clear();
    }

    //Method to ask for the value of a custom property
    Property* GetProperty(const char* name);

};

//Create a struct needed to hold the information to Map node
struct MapLayer
{
    //Add the info to the MapLayer Struct
    int id;
    SString name;
    int width;
    int height;
    uint* tiles;
    Properties properties;

    //Short function to get the gid value of x,y
    uint Get(int x, int y) const
    {
        return tiles[(y * width) + x];
    }
};

struct MapData
{
    int width;
    int height;
    int tilewidth;
    int tileheight;
    List<TileSet*> tilesets;

    //Define a property to store the MapType and Load it from the map
    MapOrientation orientation; 

    //Add a list/array of layers to the map
    List<MapLayer*> layers;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node config);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(SString mapFileName);

    //Create a method that translates x,y coordinates from map positions to world positions
    iPoint MapToWorld(int x, int y) const;

    //Add method WorldToMap to obtain  map coordinates from screen coordinates 
    iPoint WorldToMap(int x, int y);

    //Implement function to the Tileset based on a tile id
    TileSet* GetTilesetFromTileId(int gid) const;

    //Load a group of properties 
    bool LoadProperties(pugi::xml_node& node, Properties& properties);

    //Create navigation map for pathfinding
    void CreateNavigationMap(int& width, int& height, uchar** buffer) const;

    int GetTileWidth();
    int GetTileHeight();

    int getMapHeght() {
        return mapData.height * mapData.tileheight;
    }

    int getMapWidth() {
        return mapData.width * mapData.tilewidth;
    }

    void InitMap();

public: 
    SString name;
    SString path;
    PathFinding* pathfinding;

private:
    uint winW, winH;
    MapData mapData;
    bool mapLoaded;
    MapLayer* navigationLayer;
    int blockedGid = 26; //!!!! make sure that you assign blockedGid according to your map
 };

#endif // __MAP_H__