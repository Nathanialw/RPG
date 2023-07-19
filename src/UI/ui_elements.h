#pragma once
#include "SDL2/SDL.h"
#include "graphics.h"
#include <string>

namespace UI {
  
  struct Text_Frame {
    SDL_FRect frame;
    SDL_Texture* backgroundTexture = NULL;
    SDL_FRect textFrame;
    SDL_Texture* textTexture = NULL;
    std::string text;
  };

  struct Image_Frame {
    SDL_Texture* texture = NULL;
    SDL_FRect frame;
  };
 
  SDL_FRect ui_box(float x, float y, float w, float h) { // take in size and location, return a box		
    SDL_FRect box;
    return box;
  }

  SDL_FRect Center_Rect(SDL_Rect clip) {
    int w;
    int h;
    SDL_GetRendererOutputSize(Graphics::renderer, &w, &h);
    SDL_FRect rect;
    rect.x = (w / 2.0f) - (clip.w / 2.0f);
    rect.y = (h / 2.0f) - (clip.h / 2.0f);
    rect.w = clip.w;
    rect.h = clip.h;
    return rect;
  }
  
  SDL_FRect Center_Rect_In_frame(SDL_FRect rect, SDL_FRect parentRect) {
    rect.x = parentRect.x + (parentRect.w / 2.0f) - (rect.w / 2.0f);
    rect.y = parentRect.y + (parentRect.h / 2.0f) - (rect.h / 2.0f);
    rect.w = rect.w;
    rect.h = rect.h;
    return rect;
  }

  SDL_FRect Update_Scale(f2 &scale, SDL_FRect &rect) {
    return { rect.x / scale.x, rect.y / scale.y, rect.w / scale.x, rect.h / scale.y };
  }

  SDL_FRect Update_Rect_Size(f2 &scale, SDL_FRect &rect) {
    return {rect.x, rect.y, rect.w / scale.x, rect.h / scale.y };
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
