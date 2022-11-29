#pragma once
#include "entt/entt.hpp"

namespace World
{
	entt::registry zone;

    //close in data, like plants and objects
    struct Close_Tile{
        int x;
        int y;
    };


    struct Far_Tile {
        Close_Tile farTile [200];
    };

    //
    //far tile what kind of tile each subtile will generate, as in how many trees and what tileset to use
    struct Farther_Tile {
        Far_Tile fartherTile[200];
    };


}

