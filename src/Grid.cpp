#include "../include/Grid.h"

#include <raylib.h>

std::vector<std::vector<Tile>> Grid_Init(const char *mapLayoutFileName, MapInfo mapInfo)
{
    Image mapSource = LoadImage(mapLayoutFileName);

    Tile tileDirt = {false};
	Tile tileGrass = {true};
    std::vector<std::vector<Tile>> tiles(mapInfo.columnCount, std::vector<Tile>(mapInfo.rowCount, tileDirt));

    for (int x = 0; x < mapSource.width; x++)
	{
		for (int y = 0; y < mapSource.height; y++)
		{
			Color color = GetImageColor(mapSource, x, y); 

			if(color.r == 122){
				tiles[x][y] = tileDirt;
			} else {
				tiles[x][y] = tileGrass;
			} 
		}
	}

    UnloadImage(mapSource);

    return tiles;
}