#pragma once
#include "graphics.h"

namespace UI_Spellbook {
    bool b_isOpen = false;
    SDL_Texture *panel = NULL;
    SDL_Rect clip = {0, 0, 1137, 700};
    SDL_FRect panelRect;

    SDL_FRect Center_Rect(Component::Camera &camera, SDL_Rect &clip) {
        return {((Graphics::resolution.w / 2.0f) - (clip.w / 2.0f)), ((Graphics::resolution.h / 2.0f) - (clip.h / 2.0f)), (float)clip.w, (float)clip.h};
    }

    SDL_FRect Update_Scale(Component::Camera &camera, SDL_FRect textBox) {
        textBox.x = textBox.x / camera.scale.x;
        textBox.y = textBox.y / camera.scale.y;
        textBox.w = textBox.w / camera.scale.x;
        textBox.h = textBox.h / camera.scale.y;
        return textBox;
    }

    void init(){
        panel = Graphics::spellbook;
    }

//    run once the camera is attached to a unit
    void Init_UI(Component::Camera &camera){
        panelRect = Center_Rect(camera, clip);
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
            SDL_FRect draw = Update_Scale(camera, panelRect);
            SDL_RenderCopyF(Graphics::renderer, panel, &clip, &draw);
        }
    }
}