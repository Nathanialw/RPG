#pragma once
#include "SQLite_spritesheets.h"
#include "texture_packer.h"

namespace Sprite_Sheet {


//    std::map <Component::Action_State, Frame_Data> *Create_Spritesheet(sheetData &data) {
//        frameData[Component::isStatic] = data.isStatic;
//        frameData[Component::idle] = data.idle;
//        frameData[Component::walk] = data.walk;
//        frameData[Component::run] = data.run;
//        frameData[Component::attack] = data.attack;
//        frameData[Component::attack2] = data.attack2;
//        frameData[Component::cast] = data.cast;
//        frameData[Component::struck] = data.struck;
//        frameData[Component::block] = data.block;
//        frameData[Component::evade] = data.evade;
//        frameData[Component::stunned] = data.stunned;
//        frameData[Component::dead] = data.dying;
//        frameData[Component::dead] = data.corpse;
//        frameData[Component::low_hp] = data.low_hp;
//        frameData[Component::resting] = data.resting;
//        frameData[Component::ranged] = data.ranged;
//        frameData[Component::cheer] = data.cheer;
//        frameData[Component::behavior] = data.behavior;
//        frameData[Component::summoned] = data.summoned;
//        return &frameData;
//    };
//

    std::unordered_map<std::string, Texture_Packer::Sheet_Data>* Get_From_Sheet_Data(std::string &name, std::string &spritesheet, Spritesheet_Structs::sheetData &sheetData) {
        if (spritesheet != "texture_packer") {
            SQLite_Spritesheets::Get_From_DB(spritesheet, sheetData);
            return 0;
        }
        else {
                ///run texture packer
            return Texture_Packer::TexturePacker_Import(name, spritesheet);
        }
        return 0;
    }





}
