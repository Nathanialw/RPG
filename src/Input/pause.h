#pragma once

#include "components.h"
#include "timer.h"
#include "ui_elements.h"
#include "colors.h"

namespace Pause {

    bool paused = false;

    void Toggle() {
        if (!paused) {
            paused = true;
            Timer::Pause_Control();
        } else {
            paused = false;
            Timer::Pause_Control();
        }
    }

    int Show_Menu(Component::Camera &camera) {
        const int NUMMENU = 1;
        const char *labels[NUMMENU] = {"Paused"};
        SDL_Surface *menu[NUMMENU];


        menu[0] = TTF_RenderText_Solid(Graphics::font, labels[0], Color::white);
        SDL_FRect pos[NUMMENU];

        pos[0] = UI::Center_Rect(menu[0]->clip_rect);
        pos[0].y /= 3.0f;
        SDL_FRect pause = UI::Update_Scale(camera.scale, pos[0]);

        //      render menu
        for (int i = 0; i < NUMMENU; i++) {
            SDL_Texture *texture = SDL_CreateTextureFromSurface(Graphics::renderer, menu[i]);
            SDL_FreeSurface(menu[i]);
            SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &pause);
            SDL_DestroyTexture(texture);
        }
        return 3;
    }

    void Pause_Control(Component::Camera &camera) {
        //pause with no inout
        if (paused) {
            UI::Overlay(camera.scale);
            int i = Show_Menu(camera);
            if (i == 0) {
                Toggle();
            }
        }
        //toggle pause with input
    }
}// namespace Pause
