#pragma once
#include "SDL2/SDL.h"

namespace UI {

	SDL_FRect ui_box(float x, float y, float w, float h) { // take in size and location, return a box
		
		SDL_FRect box;
		return box;
	}

    SDL_FRect Center_Rect(SDL_FRect rect, SDL_Rect &clip) {
        return { ((rect.w / 2.0f) - (clip.w / 2.0f)), ((rect.h / 2.0f) - (clip.h / 2.0f)), (float)clip.w, (float)clip.h };
    }

    SDL_FRect Update_Scale(f2 &scale, SDL_FRect &rect) {
        return { rect.x / scale.x, rect.y / scale.y, rect.w / scale.x, rect.h / scale.y };
    }
}
