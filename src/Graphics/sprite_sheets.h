#pragma once
#include "Graphics/XML_Parsers/texture_packer.h"
#include "SQLite_spritesheets.h"

namespace Sprite_Sheet {

  //    std::unordered_map<std::string, Component::Sheet_Data>* Get_From_Sheet_Data(std::string &name, std::string &spritesheet, Spritesheet_Structs::sheetData &sheetData, SDL_Texture* texture, SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare) {
  //        if (spritesheet == "texture_packer") {
  //                ///run texture packer
  //            return Texture_Packer::TexturePacker_Import(name, spritesheet, texture, sheetData);
  //        }
  //        else if (spritesheet == "flare") {
  //            return Populate_Flare_SpriteSheet(name, sheetDataFlare, texture);
  //        }
  //        else {
  //            SQLite_Spritesheets::Get_From_DB(spritesheet, sheetData);
  //            return 0;
  //        }
  //        return 0;
  //    }


}
