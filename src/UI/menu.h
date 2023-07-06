#pragma once
#include "SDL2/SDL.h"
#include "../SDL_FontCache.h"
#include "base_structs.h"
#include "events.h"
#include "pause.h"
#include "camera.h"
#include "ui_elements.h"

namespace Menu
{

    struct Size {
        i2 x, y;
        i2 w, h;
    };

    void Overlay(Component::Camera &camera)
    {
        SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);
        SDL_FRect overlay = UI::Update_Scale(camera, Graphics::resolution);
        SDL_RenderFillRectF(Graphics::renderer, &overlay);
    }


    SDL_Color colors[2] = {{255, 255, 255}, {255, 0,   0}};

    struct Button
    {
        SDL_FRect size;
        SDL_FRect scaledSize;
        SDL_Surface* textSurface;
        SDL_Texture* backgroundTexture;
        SDL_Texture* textTexture;
        const char* text;
        bool selected = false;
    };

    struct Menu {
        SDL_Texture *background;
        Uint8 spacing = Mouse::cursorRadius * 2;
        std::vector<Button> buttons;
    };

//    stores menus, access with key
    std::unordered_map<std::string, Menu> menus;

    void Build_Menu(Component::Camera &camera, Menu &menu)
    {
        //        set first index
        menu.buttons[0].size = UI::Center_Rect(camera,  menu.buttons[0].textSurface->clip_rect);
        menu.buttons[0].size.y /= 2.0f;
        //        offset rest
        for (int i = 1; i < menu.buttons.size(); i++)
        {
            menu.buttons[i].size = UI::Center_Rect(camera,  menu.buttons[i].textSurface->clip_rect);
            menu.buttons[i].size.y = menu.buttons[i-1].size.y + menu.buttons[i-1].size.h + menu.spacing;;
        }

    }

    void Create_Menu(Component::Camera &camera) {
        Menu menu;
        std::vector<const char*>labels = {"Continue", "Exit"};

        for (int i = 0; i < labels.size(); i++)
        {
            Button button;
            button.text = labels[i];
            button.textSurface = TTF_RenderText_Solid(Graphics::font, labels[i], colors[0]);
            button.textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, button.textSurface);
            menu.buttons.emplace_back(button);
        }

        Build_Menu(camera, menu);

        menus["menu"] = menu;
    }


    bool toggleMenu = false;
    void Toggle(Component::Camera &camera)
    {
        if (!toggleMenu)
        {
            if (!menus.contains("menu"))
            {
                Create_Menu(camera);
            }
            toggleMenu = true;
            if (!Pause::paused) { Pause::Toggle(); }
        }
        else
        {
            toggleMenu = false;
            Pause::Toggle();
        }
    }

    int Show_Menu(entt::registry &zone, Component::Camera &camera)
    {
        Menu &menu = menus["menu"];

        for (int i = 0; i < menu.buttons.size(); i++)
        {
            Camera_Control::Maintain_Scale(zone, menu.buttons[i].size, camera);
            Build_Menu(camera, menu);
            menu.buttons[i].scaledSize = UI::Update_Scale(camera, menu.buttons[i].size);


            if (Mouse::FRect_inside_Screen_Cursor( menu.buttons[i].scaledSize))
            {
                if (!menu.buttons[i].selected)
                {
                    menu.buttons[i].selected = 1;
                    SDL_FreeSurface(menu.buttons[i].textSurface);
                    menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[1]);
                    menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
                }

            }
            else
            {
                if (menu.buttons[i].selected)
                {
                    menu.buttons[i].selected = 0;
                    SDL_FreeSurface(menu.buttons[i].textSurface);
                    menu.buttons[i].textSurface = TTF_RenderText_Solid(Graphics::font, menu.buttons[i].text, colors[0]);
                    menu.buttons[i].textTexture = SDL_CreateTextureFromSurface(Graphics::renderer, menu.buttons[i].textSurface);
                }
            }

            SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].textTexture, NULL, &menu.buttons[i].scaledSize);
        }

        while (SDL_PollEvent(&Events::event))
        {
            switch (Events::event.type)
            {
                case SDL_QUIT: {
                    for (int i = 0; i < menu.buttons.size(); i++)
                    {
                        SDL_FreeSurface(menu.buttons[i].textSurface);
                    }
                    return 1;
                }

                case SDL_MOUSEBUTTONDOWN:
                {
                    for (int j = 0; j < menu.buttons.size(); j++)
                    {
                        if (Mouse::FRect_inside_Screen_Cursor( menu.buttons[j].scaledSize))
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
                        Toggle(camera);
                        break;
                    }
                    break;
                }
            }
        }

//      render menu
        for (int i = 0; i < menu.buttons.size(); i++)
        {
//            background texture first
//            SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].backgroundTexture, NULL, &menu.buttons[i].size);
//            SDL_RenderCopyF(Graphics::renderer, menu.buttons[i].textTexture, NULL, &listItem);
        }
        return 3;
    }

    void Render_Menu(entt::registry &zone, Component::Camera &camera)
    {
        //pause with no inout
        if (toggleMenu)
        {
            Overlay(camera);
            int i = Show_Menu(zone, camera);
            if (i == 0)
            {
                Toggle(camera);
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