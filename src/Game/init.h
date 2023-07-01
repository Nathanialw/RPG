#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//#include "Joystick.h"

namespace Init {
	std::string batch = "1";

	void init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		//	SDL_Log("init error", SDL_GetError());
		}
	//	Joystick::JoystickInit();
		TTF_Init();


		SDL_SetRelativeMouseMode(SDL_TRUE);

		Graphics::running = true;

	}
}
