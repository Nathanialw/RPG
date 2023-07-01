#pragma once
#include "components.h"
#include "graphics.h"
#include "scene.h"
#include "sstream"
#include "timer.h"
#include "mouse_control.h"
#include <algorithm>
using namespace Scene;


namespace Debug_System {

	int64_t iFramePollRate = 0;
	


	void Entity_Data_Debug(float x, float y, float sx, float sy) {
	//displays :
		//x,y position
		//collision radius
	//	mass
		//SDL_Rect j = { sx, sy, 30, 30 };
		////SDL_RenderDrawRect(Graphics::renderer, &j);			
		Graphics::Surface_Data x_Position = Graphics::Load_Text_Texture(std::to_string(int(x)), { 255,0,133 });
		Graphics::Surface_Data y_Position = Graphics::Load_Text_Texture(std::to_string(int(y)), { 255,133,0	});
		//Surface_Data collision_Radius = Graphics::Load_Text_Texture(std::to_string(mas), { 133,133,133 }, renderer);
		//Surface_Data mass = Graphics::Load_Text_Texture(std::to_string(radius), { 133,133,133 }, renderer);

		//SDL_Rect e = { sx - 5, sy - 5, 27, 50 };
		//SDL_RenderDrawRect(renderer, &e);
		////SDL_RenderCopy(renderer, mass.pTexture, , );

		SDL_FRect a = {sx+25, sy-20, 15, 15};
		SDL_RenderCopyF(Graphics::renderer, x_Position.pTexture, &x_Position.k, &a);
		SDL_FRect b = { sx+25, sy-10, 15, 15 };
		SDL_RenderCopyF(Graphics::renderer, y_Position.pTexture, &y_Position.k,  &b);
		//SDL_Rect c = { sx, sy+20, 15, 10 };
		//SDL_RenderCopy(renderer, collision_Radius.pTexture, &collision_Radius.k, &c);
		//SDL_Rect d = { sx, sy+30, 15, 10 };
		//SDL_RenderCopy(renderer, mass.pTexture, &mass.k, &d);			
		//SDL_DestroyTexture(collision_Radius.pTexture);
		//SDL_DestroyTexture(mass.pTexture);
		SDL_DestroyTexture(x_Position.pTexture);
		SDL_DestroyTexture(y_Position.pTexture);
	}

	Graphics::Surface_Data framerate;
	Graphics::Surface_Data timeStep;
	bool frameRateMode = true;
	bool frameTimeMode = false;

	void Toggle_Frame_Rate_Mode() {
		if (frameRateMode) {
			frameRateMode = false;
			frameTimeMode = true;			
		}
		else {
			frameRateMode = true;
			frameTimeMode = false;
		}
		iFramePollRate = 500;
	}

	void Framerate(Component::Camera &camera) {		
		iFramePollRate += Timer::timeStep;
		if (iFramePollRate >= 1000) {
			iFramePollRate = 0;
			if (frameRateMode) {
				SDL_DestroyTexture(framerate.pTexture);
				framerate = Graphics::Load_Text_Texture(std::to_string(Timer::fps_avgFPS), { 133,255,133 });
			}
			if (frameTimeMode) {
				SDL_DestroyTexture(timeStep.pTexture);
				timeStep = Graphics::Load_Text_Texture(std::to_string(Timer::fps_timeStep / (float)SDL_GetPerformanceFrequency()), { 133,255,133 });
			}
		}
		SDL_FRect c = { 0.0f, 0.0f, 200.0f / camera.scale.x, 100.0f / camera.scale.y };
		if (frameRateMode) {
			SDL_RenderCopyF(Graphics::renderer, framerate.pTexture, &framerate.k, &c);
		}
		if (frameTimeMode) {
			SDL_RenderCopyF(Graphics::renderer, timeStep.pTexture, &timeStep.k, &c);
		}
	}


	

		void Debugger() {
		//Framerate();
		}
}





