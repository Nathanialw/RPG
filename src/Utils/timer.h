#pragma once

#include "SDL2/SDL.h"
#include <SDL2/SDL_timer.h>
#include <iostream>
#include "math.h"
#include "utilities.h"

namespace Timer {

  float timeStep = 0;
  int64_t avgFPS = 0;
  int64_t startPerf = 0;
  int64_t endPerf = 0;
  bool pause = false;

  float fps_timeStep = 0;
  float fps_avgFPS = 0;
  int64_t fps_prevFrame = 0;
  int64_t fps_currentFrame = 0;

  bool lockFramerate = false;

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
    timeStep = ((double) endPerf - (double) startPerf) / (double) SDL_GetPerformanceFrequency() * 1000.0f;
    startPerf = SDL_GetPerformanceCounter();
    if (lockFramerate) {
      if (timeStep < 16.66f) {
        SDL_Delay(floor(16.66f - timeStep));
        timeStep = 16.66f;
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

    bool Calc() { //controls how often collision calculates
      fCounter_MS -= Timer::timeStep;
      if (fCounter_MS <= 0.0f) {
        fCounter_MS = fTime_between; //every this many milliseconds
        return true;
      } else return false;
    }
  };
}
