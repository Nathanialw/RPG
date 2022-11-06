#pragma once
#include "SQLite_spritesheets.h"
#include "texture_packer.h"

namespace Sprite_Sheet {

    std::unordered_map<std::string, Component::Sheet_Data>* Get_From_Sheet_Data(std::string &name, std::string &spritesheet, Spritesheet_Structs::sheetData &sheetData, SDL_Texture* texture) {
        if (spritesheet != "texture_packer") {
            SQLite_Spritesheets::Get_From_DB(spritesheet, sheetData);
            return 0;
        }
        else {
                ///run texture packer
            return Texture_Packer::TexturePacker_Import(name, spritesheet, texture, sheetData);
        }
        return 0;
    }





}
