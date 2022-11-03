#pragma once
#include <SDL2/SDL.h>


namespace Joystick {

	SDL_Joystick* joystick = NULL;
	const int JOYSTICK_DEAD_ZONE = 18000;
	int xDir, yDir;

	void JoystickInit() {
		if (SDL_NumJoysticks() > 0) {
			printf("joystick activated!\n");
		}

		joystick = SDL_JoystickOpen(0);
		if (joystick != NULL) {
			printf("joystick Open!\n");
		}
	}

	void JoystickDirection(SDL_Event &event) {
		if (event.jaxis.axis == 0)
		{
			//Left of dead zone
			if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1;
			}
			//Right of dead zone
			else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1;
			}
			else
			{
				xDir = 0;
			}
		}
		//Y axis motion
		else if (event.jaxis.axis == 1)
		{
			//Below of dead zone
			if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1;
			}
			//Above of dead zone
			else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1;
			}
			else
			{
				yDir = 0;
			}
		}

	};

	double JoystickInput(SDL_Event& event) {
		JoystickDirection(event);
		double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);
		if (xDir == 0 && yDir == 0)
		{
			joystickAngle = 0;
		}
		return joystickAngle;
	}
}
