#pragma once
#include "SDL2/SDL.h"
#include "graphics.h"
#include "camera.h"

namespace UI {

	SDL_FRect ui_box(float x, float y, float w, float h) { // take in size and location, return a box
		
		SDL_FRect box;
		return box;
	}

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
}
