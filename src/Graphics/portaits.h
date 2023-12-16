#include "SQLite_item_data.h"
#include "graphics.h"

namespace Portraits {

  void Load_Portait(std::string &path) {
  }

  void Load_Body(std::string &path) {
  }

  void Render_Portait(SDL_Texture *backgroundTexture, SDL_Texture *texture, Rendering_Components::Unit_Frame_Portrait &portrait, SDL_FRect frame) {
    SDL_RenderCopyF(Graphics::renderer, backgroundTexture, NULL, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.texture, NULL, &frame);

    for (int i = 0; i < portrait.gear.size(); ++i) {
      Graphics::Render_FRect(portrait.gear[i].texture, portrait.gear[i].color, NULL, &frame);
    }
  }

}// namespace Portraits