#pragma once
#include "graphics.h"

namespace UI_Spellbook {
    bool b_isOpen = false;
    SDL_Texture *panel = NULL;
    SDL_Rect clip = {0, 0, 1137, 700};
    SDL_FRect draw;

    void init(){
        draw = {(Graphics::resolution.w - 1137.0f)/4.0f/2.0f, (Graphics::resolution.h - 700.0f)/8.0f/2.0f, 1137.0f/1.5f, 700.0f/1.5f};
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

    void Draw_Spellbook() {
        if (b_isOpen) {
            SDL_RenderCopyF(Graphics::renderer, panel, &clip, &draw);
        }
    }
}