//
// Created by desktop on 12/12/24.
//
#pragma once

#include "SDL2/SDL.h"

namespace Color {

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color red = {255, 0, 0, 255};
    SDL_Color green = {0, 255, 0, 255};

    SDL_Color gray = {200, 200, 200, 255};
    SDL_Color grayLight = {55, 55, 55, 255};
    SDL_Color cyan = {0, 125, 125, 255};
    SDL_Color teal = {0, 50, 155, 255};
    SDL_Color orange = {255, 155, 255};
    SDL_Color darkBlue = {0, 0, 155, 255};
    SDL_Color blue = {0, 0, 255, 100};
    SDL_Color magenta = {255, 0, 255, 255};


    SDL_Color redMagical = {255, 20, 20, 255};
    SDL_Color blueMagical = {51, 153, 255, 255};
    SDL_Color yellowMagical = {255, 128, 0, 255};

    SDL_Color Set_Color_With_Alpha(SDL_Color color, Uint8 alpha) {
        color.a = alpha;
        return color;
    }

    SDL_Color Get_Random_Color(Uint8 alpha = 255) {
        SDL_Color color = {};
        color.r = rand() % 254 + 1;
        color.g = rand() % 254 + 1;
        color.b = rand() % 254 + 1;
        color.a = alpha;
        return color;
    }

    void Set_Render_Draw_Color(SDL_Renderer *renderer) {
        SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    }

    void Set_Render_Draw_Color(SDL_Renderer *renderer, SDL_Color color, Uint8 alpha = 255) {
        if (alpha != 255)
            color.a = alpha;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    }

    void Set_Health_Color(SDL_Renderer *renderer, float health) {
        Uint8 l_red = 150 * (1 - health);
        Uint8 l_green = 150 * health;
        SDL_Color color = {l_red, l_green, 0, 255};
        Set_Render_Draw_Color(renderer, color);
    }
}