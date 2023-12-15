#include "SQLite_item_data.h"
#include "graphics.h"

namespace Portraits {

  void Load_Portait(std::string &path) {
  }

  void Load_Body(std::string &path) {
  }

  void Render_Portait(SDL_Texture *backgroundTexture, SDL_Texture *texture, Rendering_Components::Portrait &portrait, SDL_FRect frame) {
    SDL_RenderCopyF(Graphics::renderer, backgroundTexture, NULL, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.texture, NULL, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.horns, NULL, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.beard, NULL, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.chest, NULL, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.hair, NULL, &frame);
    SDL_RenderCopyF(Graphics::renderer, portrait.helm, NULL, &frame);
  }

}// namespace Portraits