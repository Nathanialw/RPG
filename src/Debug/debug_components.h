#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

namespace Debug {

  unsigned long int numEntities;
  unsigned long int numRendered;
  unsigned long int renderChecks;
  unsigned long int collisionChecks;
  unsigned long int renderComponent;
  bool showPosition = false;

  enum Settings {
    Framerate = 0,
    NumRendered,
    InteractionRects,
    EntityCount,
    RenderChecks,
    CollisionChecks,
    RenderComponent,
    UpdateQuadTreeDebug,
    SIZE
  };

  std::array<bool, Debug::Settings::SIZE> settings;

  void Load_Settings() {
    for (auto &i: settings) {
      i = false;
    }
  };

  struct DEBUG_Position {

  };
}