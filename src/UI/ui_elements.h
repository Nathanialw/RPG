#pragma once
#include "SDL2/SDL.h"

namespace UI {

	SDL_FRect ui_box(float x, float y, float w, float h) { // take in size and location, return a box
		
		SDL_FRect box;
		return box;
	}

    SDL_FRect Center_Rect(SDL_Rect &clip) {
        SDL_DisplayMode dm;
        SDL_GetWindowDisplayMode(Graphics::window, &dm);
        return { ((dm.w / 2.0f) - (clip.w / 2.0f)), ((dm.h / 2.0f) - (clip.h / 2.0f)), (float)clip.w, (float)clip.h };
    }

    SDL_FRect Update_Scale(f2 &scale, SDL_FRect &rect) {
        return { rect.x / scale.x, rect.y / scale.y, rect.w / scale.x, rect.h / scale.y };
    }

    SDL_FRect Get_Resolution() {
        SDL_DisplayMode dm;
        SDL_GetWindowDisplayMode(Graphics::window, &dm);
        return {0.0f, 0.0f, (float)dm.w, (float)dm.h};
    }



    void Overlay(f2 &scale)
    {
        SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);

        SDL_DisplayMode dm;
        SDL_GetWindowDisplayMode(Graphics::window, &dm);
        SDL_FRect overlay = {0, 0, (float)dm.w, (float)dm.h};
        overlay = Update_Scale(scale, overlay);
        SDL_RenderFillRectF(Graphics::renderer, &overlay);
    }
}
