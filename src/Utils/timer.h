#pragma once

#include "SDL2/SDL.h"
#include "math.h"
#include "utilities.h"
#include <SDL2/SDL_timer.h>
#include <iostream>

namespace Timer {

  float timeStep = 0;
  int64_t avgFPS = 0;
  Uint64 startPerf = 0;
  Uint64 endPerf = 0;
  bool pause = false;

  float fps_timeStep = 0;
  float fps_avgFPS = 0;
  int64_t fps_prevFrame = 0;
  int64_t fps_currentFrame = 0;

  bool lockFramerate = false;

  enum GameState {
    movement,
    collision,
    status,
    render,
    update_quad_tree,
    renderpresent,
    SIZE
  };
  std::array<float, SIZE> GameStateValue;
  std::array<std::string, SIZE> GameStateText = {"movement", "collision", "status", "render", "update quad tree", "render present"};

  struct Game_Loop_Timer {
    Uint64 startPerf = 0;
    Uint64 endPerf = 0;
  };
  Game_Loop_Timer gameLoopTimer;

  void Update_Game_Loop_Timers(float &stateTime, Game_Loop_Timer &timer) {
    timer.endPerf = SDL_GetPerformanceCounter();
    Uint64 sstateTime = (timer.endPerf - timer.startPerf);
    Uint64 dd = SDL_GetPerformanceFrequency();
    float gg = (float)sstateTime / (float)dd;
    stateTime = gg * 1000.0f;
    timer.startPerf = timer.endPerf;
  }

  void Pause_Control() {
    if (pause) {
      pause = false;
    } else {
      pause = true;
    }
  }

  void Calculate_FPS() {
    fps_avgFPS = (1.0f / timeStep) * 1000.0f;
    fps_timeStep = fps_avgFPS / 1000.0f;
    if (pause) {
      timeStep = 0;
      fps_timeStep = 0;
    }
  }

  void Calculate_Timestep() {
    endPerf = SDL_GetPerformanceCounter();
    Uint64 sstateTime = (endPerf - startPerf);
    Uint64 dd = SDL_GetPerformanceFrequency();
    float gg = (float)sstateTime / (float)dd;
    timeStep = gg * 1000.0f;
    startPerf = endPerf;
    if (lockFramerate) {
      if (timeStep < 16.66f) {
        Uint32 nn = floor(16.66f - timeStep);
        SDL_Delay(nn);
        timeStep = nn + timeStep;
      }
    }
    Calculate_FPS();

    if (pause) {
      timeStep = 0;
    }
  }

  class Frame_Timer {
    float fTime_between;
    float fCounter_MS;

  public:
    Frame_Timer(float frequency) {
      fTime_between = frequency;
      fCounter_MS = 0.0f;
    }

    bool Calc() {//controls how often collision calculates
      fCounter_MS -= Timer::timeStep;
      if (fCounter_MS <= 0.0f) {
        fCounter_MS = fTime_between;//every this many milliseconds
        return true;
      } else
        return false;
    }
  };
}// namespace Timer
