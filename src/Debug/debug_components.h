#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

namespace Debug {

//  unsigned long int numEntities;
//  unsigned long int numRendered;
//  unsigned long int interactionRects;
//  unsigned long int renderChecks;
//  unsigned long int entityCount;
//  unsigned long int renderChecks;
//  unsigned long int collisionChecks;
//  unsigned long int renderComponent;
//  unsigned long int UpdateQuadTreeDebug;
//  unsigned long int treeSize;
//  bool showPosition = false;

  enum Settings {
    Framerate = 0,
    NumRendered,
    InteractionRects,
    EntityCount,
    RenderChecks,
    CollisionChecks,
    RenderComponent,
    UpdateQuadTreeDebug,
    TreeSize,
    entityPositions,
    loopTimers,
    lockFramerate,
    SIZE
  };

  std::array<bool, Settings::SIZE> settings;
  std::array<unsigned long int, Settings::SIZE> settingsValue;
  std::array<std::string, Debug::Settings::SIZE> labels = {"Framerate", "Num Rendered", "Interaction Rects", "entity count", "Render Checks", "Collision Checks", "Num w/ Render Components", "Update Quad Tree Debug", "Quad Tree Size", "Entity Positions", "Loop Timers", "Lock Framerate"};

  void Load_Settings() {
    for (auto &i: settings) {
      i = false;
    }
  };

  struct DEBUG_Position {

  };
}