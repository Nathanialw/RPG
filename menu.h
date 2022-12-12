#pragma once
#include "timer.h"
#include "graphics.h"
#include "SDL2/SDL.h"
#include "SDL_FontCache.h"
#include "base_structs.h"
#include "events.h"
#include "pause.h"

namespace Menu
{
//    dispay buttons on screen within a context menu
//    when you click on a button it triggers an function that changes some state

//    components
    struct Size {
        f2 x, y;
        f2 w, h;
    };

    struct Button
    {
        Size size;
        std::string text;
        SDL_Texture *texture;
    };

//    item list that holds the button structs, needs to be a set size for each instance of its use
    struct Menu
    {
        Size size;
        SDL_Texture *texture;
        std::vector<Button> list;
    };

    //get the instance of the meni from a key, not neccessarily a string
    std::unordered_map<std::string, Menu> menus;

    void Overlay()
    {
        SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);
        SDL_RenderFillRectF(Graphics::renderer, &Graphics::resolution);
    }
//
//    Button Create_Button (Size size, std::string text)
//    {
//        Button button;
//        button.size = size;
//        button.text = text;
//    }
//
//    std::string text[4] = {"ddd", "aaa", ";;;", "lll"};
//
//    void Create_Menu (Size size, SDL_Texture *texture)
//    {
//        Menu menu;
//        menu.size = size;
//
//
//    }

    bool toggleMenu = false;
    void Toggle()
    {
        if (!toggleMenu)
        {
            toggleMenu = true;
            if (!Pause::paused) { Pause::Toggle(); }
        }
        else
        {
            toggleMenu = false;
            Pause::Toggle();
        }
    }

    int Show_Menu(Component::Camera &camera)
    {
        const int NUMMENU = 2;
        const char *labels[NUMMENU] = {"Continue", "Exit"};
        SDL_Surface *menu[NUMMENU];
        bool selected[NUMMENU] = {0, 0};
        SDL_Color color[2] = {{255, 255, 255},
                              {255, 0,   0}};
        int spacing = Mouse::cursorRadius * 2;

        menu[0] = TTF_RenderText_Solid(Graphics::font, labels[0], color[0]);
        menu[1] = TTF_RenderText_Solid(Graphics::font, labels[1], color[0]);
        SDL_FRect pos[NUMMENU];
        pos[0].x = (camera.screen.w / 2) - (menu[0]->clip_rect.w / 2);
        pos[0].y = (camera.screen.h / 4) - (menu[0]->clip_rect.h / 2);
        pos[0].w = menu[0]->clip_rect.w;
        pos[0].h = menu[0]->clip_rect.h;

        pos[1].x = (camera.screen.w / 2) - (menu[1]->clip_rect.w / 2);
        pos[1].y = pos[0].y + pos[0].h + spacing;
        pos[1].w = menu[1]->clip_rect.w;
        pos[1].h = menu[1]->clip_rect.h;

        for (int i = 0; i < NUMMENU; i++)
        {
            if (Mouse::FRect_inside_Screen_Cursor(pos[i]))
            {
                if (!selected[i])
                {
                    selected[i] = 1;
                    SDL_FreeSurface(menu[i]);
                    menu[i] = TTF_RenderText_Solid(Graphics::font, labels[i], color[1]);
                }

            }
            else
            {
                if (selected[i])
                {
                    selected[i] = 0;
                    SDL_FreeSurface(menu[i]);
                    menu[i] = TTF_RenderText_Solid(Graphics::font, labels[i], color[0]);
                }
            }
        }

        while (SDL_PollEvent(&Events::event))
        {
            switch (Events::event.type)
            {
                case SDL_QUIT: {
                    for (int i = 0; i < NUMMENU; i++)
                    {
                        SDL_FreeSurface(menu[i]);
                    }
                    return 1;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    for (int j = 0; j < NUMMENU; j++)
                    {
                        if (Mouse::FRect_inside_Screen_Cursor(pos[j]))
                        {
//                            returns the index of the array the mouse has clicked on
                            return j;
                        }
                    }
                    break;
                }

                case SDL_KEYDOWN:
                {
                    if (Events::event.key.keysym.sym == SDLK_ESCAPE)
                    {
                        Toggle();
                        break;
                    }
                    break;
                }
            }
        }

//      render menu
        for (int i = 0; i < NUMMENU; i++)
        {
            SDL_Texture * texture = SDL_CreateTextureFromSurface(Graphics::renderer, menu[i]);
            SDL_FreeSurface(menu[i]);
            SDL_RenderCopyF(Graphics::renderer, texture, NULL, &pos[i]);
        }
        return 3;
    }

    void Render_Menu(Component::Camera &camera)
    {
        //pause with no inout
        if (toggleMenu)
        {
            Overlay();
            int i = Show_Menu(camera);
            if (i == 0)
            {
                Toggle();
            }
            if (i == 1)
            {
//                exit program
                Graphics::closeContext();
            }
        }
        //toggle pause with input
    }
}