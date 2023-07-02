#pragma once
#include "graphics.h"


namespace Char_Create {

    bool customization = false;
    struct Character {

    };

    Character Create_Character() {
        Character character;
        while (customization) {
//        char customization logic

        }

//        return customization data selected by user
        return character;
    }

    void Character_Selection() {
        Character character = Create_Character();
//        pass or send character choice into game init
    }
}