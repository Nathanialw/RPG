#pragma once
#include "timer.h"

namespace Pause {

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

    void Overlay(Component::Camera &camera)
    {
        SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);
        SDL_FRect overlay = Update_Scale(camera, Graphics::resolution);
        SDL_RenderFillRectF(Graphics::renderer, &overlay);
    }

    bool paused = false;
    void Toggle()
    {
        if (!paused)
        {
            paused = true;
            Timer::Pause_Control();
        }
        else
        {
            paused = false;
            Timer::Pause_Control();
        }
    }

    int Show_Menu(Component::Camera &camera)
    {
        const int NUMMENU = 1;
        const char *labels[NUMMENU] = {"Paused"};
        SDL_Surface *menu[NUMMENU];
        SDL_Color color[1] = {255, 255, 255};

        menu[0] = TTF_RenderText_Solid(Graphics::font, labels[0], color[0]);
        SDL_FRect pos[NUMMENU];

        pos[0] = Center_Rect(camera,  menu[0]->clip_rect);
        pos[0].y /= 3.0f;
        SDL_FRect pause = Update_Scale(camera, pos[0]);

//      render menu
        for (int i = 0; i < NUMMENU; i++)
        {
            SDL_Texture * texture = SDL_CreateTextureFromSurface(Graphics::renderer, menu[i]);
            SDL_FreeSurface(menu[i]);
            SDL_RenderCopyF(Graphics::renderer, texture, NULL, &pause);
        }
        return 3;
    }

    void Pause_Control(Component::Camera &camera)
    {
        //pause with no inout
        if (paused)
        {
            Overlay(camera);
            int i = Show_Menu(camera);
            if (i == 0)
            {
                Toggle();
            }
        }
        //toggle pause with input
    }
}