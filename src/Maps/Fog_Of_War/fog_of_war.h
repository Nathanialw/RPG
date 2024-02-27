#pragma once
#include "array"
#include "camera.h"
#include "components.h"
#include "entt/entt.hpp"

namespace Fog_Of_War {

  const int numCells = 32;
  int cellSize = 16;
  int lightRadius = 0;
  Uint8 maxDarkness = 245;
  Uint8 minDarkness = 100;
  SDL_Texture *texture;

  std::array<std::array<Uint8, numCells>, numCells> fogCells;

  void Render_Void(float x, float y, float w, float h) {
    SDL_FRect renderRect;
    renderRect.x = x;
    renderRect.y = y;
    renderRect.w = w;
    renderRect.h = h;

    SDL_Color color = {0, 0, 0, maxDarkness};
    SDL_SetTextureAlphaMod(texture, maxDarkness);
    Graphics::Render_FRect(texture, color, nullptr, &renderRect);
  }

  void Set() {
    texture = Graphics::fogOfWar;

    for (int k = 0; k < (numCells / 2); ++k) {
      for (int i = 0 + k; i < fogCells.size() - k; ++i) {
        for (int j = 0 + k; j < fogCells[i].size() - k; ++j) {
          fogCells[i][j] = (numCells / 2) - k;
        }
      }
    }
  }

  bool setFogOfWar = false;
  void Render(entt::registry &zone, entt::entity &entity, Component::Camera &camera) {
    if (!setFogOfWar) {
      setFogOfWar = true;
      Set();
    }
    float sightRange = cellSize + lightRadius;

    auto position = zone.get<Component::Position>(entity);
    float x = position.x - ((float) sightRange * (float) numCells / 2.0f);
    float y = position.y - ((float) sightRange * (float) numCells / 2.0f);

    for (int i = 0; i < numCells; i++) {
      for (int j = 0; j < numCells; j++) {
        SDL_FRect renderRect;
        renderRect.x = (float) x + ((float) i * sightRange) - camera.screen.x;
        renderRect.y = (float) y + ((float) j * sightRange) - camera.screen.y;
        renderRect.w = (float) sightRange;
        renderRect.h = (float) sightRange;

        //linear
        Uint8 gradient = (128.0f / ((float) numCells / 2.0f));
        Uint8 darkness = 127 + (gradient * fogCells[i][j]);

        if (darkness > maxDarkness) darkness = maxDarkness;
        if (darkness < minDarkness) darkness = minDarkness;

        SDL_Color color = {0, 0, 0, darkness};
        SDL_SetTextureAlphaMod(texture, darkness);
        Graphics::Render_FRect(texture, color, nullptr, &renderRect);

        if (i == 0 && j == 0) {
          //top
          Render_Void((float) i, (float) j, camera.screen.w, (float) renderRect.y);
          //left
          Render_Void((float) i, (float) renderRect.y, renderRect.x, (float) numCells * (float) sightRange);
        }
        if (i == 0 && j == numCells - 1) {
          //bottom
          Render_Void((float) i, (float) renderRect.y + sightRange, (float) camera.screen.w, camera.screen.h - (float) renderRect.y + sightRange);
        }
        if (i == numCells - 1 && j == 0) {
          //right
          Render_Void((float) renderRect.x + sightRange, (float) renderRect.y, renderRect.x, (float) numCells * (float) sightRange);
        }
      }
    }
  }


}// namespace Fog_Of_War