#pragma once
#include "SDL2/SDL.h"

namespace UI {

  SDL_FRect ui_box(float x, float y, float w, float h) { // take in size and location, return a box		
    SDL_FRect box;
    return box;
  }

  SDL_FRect Center_Rect(SDL_Rect &clip) {
    int w;
    int h;
    SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
    SDL_FRect rect = { ((float)(w / 2.0f) - (clip.w / 2.0f)), ((float)(h / 2.0f) - (clip.h / 2.0f)), 0, 0};
    rect.w = clip.w;
    rect.h = clip.h;
    return rect;
  }

  SDL_FRect Update_Scale(f2 &scale, SDL_FRect &rect) {
    return { rect.x / scale.x, rect.y / scale.y, rect.w / scale.x, rect.h / scale.y };
  }

  SDL_FRect Get_Resolution() {
    int w;
    int h;
    SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
    return {0.0f, 0.0f, (float)w, (float)h};
  }

  void Overlay(f2 &scale)
  {
    SDL_SetRenderDrawBlendMode(Graphics::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Graphics::renderer, 0, 0, 0, 75);

    int w;
    int h;
    SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
    SDL_FRect overlay = {0, 0, (float)w, (float)h};
    overlay = Update_Scale(scale, overlay);
    SDL_RenderFillRectF(Graphics::renderer, &overlay);
  }
}
