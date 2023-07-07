#pragma once
#include "graphics.h"
#include "ui_elements.h"

namespace UI_Spellbook {
    /*
     *
     *  spells
     *
     */


    entt::entity spells[2];

    void Create_Spells () {

    }

    void Position_Spells () {

    }


    /*
     *
     *  Spell book frame
     *
     */
    bool b_isOpen = false;
    SDL_Texture *panel = NULL;
    SDL_Rect clip = {0, 0, 1137, 700};
    SDL_FRect panelRect;


//    run once the camera is attached to a unit
    void Init_UI(){
        panelRect = UI::Center_Rect(clip);
        panel = Graphics::spellbook;
    }


    void Toggle() {
        if (b_isOpen){
            b_isOpen = false;
        }
        else {
            b_isOpen = true;
        }
    }


    void Draw_Spellbook(Component::Camera &camera) {
        if (b_isOpen) {
            SDL_FRect draw = UI::Update_Scale(camera.scale, panelRect);
            SDL_RenderCopyF(Graphics::renderer, panel, &clip, &draw);
        }
    }
}