#pragma once
#include "timer.h"

namespace Pause {

    void Overlay()
    {
        SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);
        SDL_RenderFillRectF(Graphics::renderer, &Graphics::resolution);
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
        pos[0].x = (camera.screen.w / 2) - (menu[0]->clip_rect.w / 2);
        pos[0].y = (camera.screen.h / 5) - (menu[0]->clip_rect.h / 2);
        pos[0].w = menu[0]->clip_rect.w;
        pos[0].h = menu[0]->clip_rect.h;


//      render menu
        for (int i = 0; i < NUMMENU; i++)
        {
            SDL_Texture * texture = SDL_CreateTextureFromSurface(Graphics::renderer, menu[i]);
            SDL_FreeSurface(menu[i]);
            SDL_RenderCopyF(Graphics::renderer, texture, NULL, &pos[i]);
        }
        return 3;
    }

    void Pause_Control(Component::Camera &camera)
    {
        //pause with no inout
        if (paused)
        {
            Overlay();
            int i = Show_Menu(camera);
            if (i == 0)
            {
                Toggle();
            }
        }
        //toggle pause with input
    }
}