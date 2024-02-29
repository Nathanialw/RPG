#pragma once
#include "SDL2/SDL.h"
#include "array"
#include "camera.h"
#include "components.h"
#include "entt/entt.hpp"

namespace Fog_Of_War {

  const int numCells = 128;
  int cellSize = 4;
  int lightRadius = 4;
  Uint8 maxDarkness = 245;
  Uint8 minDarkness = 100;
  SDL_Texture *texture;
  SDL_Texture *fogScreenBuffer;

  std::array<std::array<Uint8, numCells>, numCells>
      fogCells;

  void Render_Void(float x, float y, float w, float h) {
    SDL_FRect renderRect;
    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;

    SDL_Color color = {0, 0, 0, maxDarkness};
    SDL_SetTextureAlphaMod(texture, maxDarkness);
    //render to texture
    Graphics::Render_FRect(texture, color, nullptr, &renderRect);
  }

  void Set() {
    texture = Graphics::fogOfWar;
    SDL_DestroyTexture(fogScreenBuffer);
    fogScreenBuffer = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (numCells * (cellSize + lightRadius)), (numCells * (cellSize + lightRadius)));
    SDL_SetTextureBlendMode(fogScreenBuffer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(Graphics::renderer, fogScreenBuffer);

    for (int k = 0; k < (numCells / 2); ++k) {
      for (int i = 0 + k; i < fogCells.size() - k; ++i) {
        for (int j = 0 + k; j < fogCells[i].size() - k; ++j) {
          fogCells[i][j] = (numCells / 2) - k;
        }
      }
    }

    float sightRange = cellSize + lightRadius;

    //    auto position = zone.get<Component::Position>(entity);
    //    float x = position.x - ((float) sightRange * (float) numCells * 0.5f);
    //    float y = position.y - ((float) sightRange * (float) numCells * 0.5f);

    for (int i = 0; i < numCells; i++) {
      for (int j = 0; j < numCells; j++) {
        SDL_FRect renderRect;
        renderRect.x = ((float) i * sightRange);
        renderRect.y = ((float) j * sightRange);
        renderRect.w = (float) sightRange;
        renderRect.h = (float) sightRange;

        //linear
        Uint8 gradient = (128.0f / ((float) numCells * 0.5f));
        Uint8 darkness = 127 + (gradient * fogCells[i][j]);

        if (darkness > maxDarkness) darkness = maxDarkness;
        if (darkness < minDarkness) darkness = minDarkness;

        SDL_Color color = {0, 0, 0, darkness};

        SDL_SetTextureAlphaMod(texture, darkness);
        //render to texture
        Graphics::Render_FRect(texture, color, nullptr, &renderRect);
      }
    }
    SDL_SetRenderTarget(Graphics::renderer, nullptr);
  }


  bool setFogOfWar = false;
  void Render(Component::Camera &camera) {
    if (!setFogOfWar) {
      setFogOfWar = true;
      Set();
    }
    float d = (float) numCells * ((float) cellSize + (float) lightRadius);
    float x = (camera.screen.w * 0.5f) - (d * 0.5f);
    float y = (camera.screen.h * 0.5f) - (d * 0.5f);

    SDL_FRect rect = {x, y, d, d};

    SDL_RenderCopyF(Graphics::renderer, fogScreenBuffer, nullptr, &rect);
    //top
    Render_Void(0.0f, 0.0f, camera.screen.w, ((camera.screen.h * 0.5f) - (d * 0.5f)));
    //left
    Render_Void(0.0f, (camera.screen.h * 0.5f) - (d * 0.5f), (camera.screen.w - d) * 0.5f, d);
    //bottom
    Render_Void(0.0f, y + d, camera.screen.w, camera.screen.h - (y + d));
    //right
    Render_Void(x + d, y, camera.screen.w - (y + d), d);
  }


}// namespace Fog_Of_War