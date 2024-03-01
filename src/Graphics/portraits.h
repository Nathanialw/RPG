#pragma once
#include "SQLite_item_data.h"
#include "graphics.h"

namespace Portraits {

  void Load_Portrait(std::string &path) {
  }

  void Load_Body(std::string &path) {
  }

  void Render_Portrait(SDL_Texture *backgroundTexture, SDL_Texture *texture, Rendering_Components::Unit_Frame_Portrait &portrait, SDL_FRect frame) {
    SDL_RenderCopyF(Graphics::renderer, backgroundTexture, nullptr, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.texture, nullptr, &frame);

    for (int i = 0; i < portrait.gear.size(); ++i) {
      Graphics::Render_FRect(portrait.gear[i].texture, portrait.gear[i].color, nullptr, &frame);
    }
  }

}// namespace Portraits