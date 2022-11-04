#pragma once
#include "SDL2/SDL.h"
#include <iostream>


namespace Timer {

	int64_t timeStep = 0;
	int64_t avgFPS = 0;
	int64_t prevFrame = 0;
	int64_t currentFrame = 0;
	bool pause = false;

	void Pause_Control() {
		if (pause) {
			pause = false;
		}
		else {
			pause = true;
		}
	}

	void Calculate_Timestep() {
		prevFrame = currentFrame;
		currentFrame = SDL_GetTicks64();
		timeStep = currentFrame - prevFrame;

		if (pause) {
			timeStep = 0;
		}
	}

	float fps_timeStep = 0;
	int64_t fps_avgFPS = 0;
	int64_t fps_prevFrame = 0;
	int64_t fps_currentFrame = 0;

	void Calculate_FPS() {
		fps_prevFrame = fps_currentFrame;
		fps_currentFrame = SDL_GetPerformanceCounter();
		fps_timeStep = fps_currentFrame - fps_prevFrame;
		float frequency = SDL_GetPerformanceFrequency();
		if (fps_timeStep != 0 && frequency != 0) {
			fps_avgFPS = 1 / (fps_timeStep / frequency);
		}
		if (pause) {
			timeStep = 0;
			fps_timeStep = 0;
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
			}
			else return false;
		}
	};

}
