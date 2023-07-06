#pragma once
#include "graphics.h"
#include "ui_elements.h"

namespace UI_Spellbook {
    bool b_isOpen = false;
    SDL_Texture *panel = NULL;
    SDL_Rect clip = {0, 0, 1137, 700};
    SDL_FRect panelRect;


    void init(){
        panel = Graphics::spellbook;
    }

//    run once the camera is attached to a unit
    void Init_UI(Component::Camera &camera){
        panelRect = UI::Center_Rect(camera, clip);
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
            SDL_FRect draw = UI::Update_Scale(camera, panelRect);
            SDL_RenderCopyF(Graphics::renderer, panel, &clip, &draw);
        }
    }
}