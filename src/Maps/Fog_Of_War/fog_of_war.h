#pragma once

#include "Input/camera.h"
#include "SDL2/SDL.h"
#include "array"
#include "components.h"
#include "entt/entt.hpp"

namespace Fog_Of_War {

	const int numCells = 128;
	int cellSize = 2;
	int lightRadius = 7;
	Uint8 maxDarkness = 245;
	Uint8 minDarkness = 100;
	SDL_Texture *texture;

	std::vector<Uint32> Set_Pixels(SDL_FRect &screen) {
		std::vector<Uint32> pixels;
		pixels.resize(screen.w * screen.h);

		for (int i = 0; i < screen.h; i++) {
			for (int j = 0; j < screen.w; j++) {
				int index = i * screen.w + j;
				Uint8 darkness;

				int centerX = screen.w / 2;
				int centerY = screen.h / 2;

				int dx = j - centerX;
				int dy = i - centerY;
				float distance = (dx * dx) + (dy * dy);
				if (distance <= lightRadius * lightRadius)
					darkness = minDarkness;
				else if (distance > (64.0f*lightRadius / 2) * (64.0f*lightRadius / 2))
					darkness = maxDarkness;
				else
					darkness = (Uint8) (minDarkness + (sqrt(distance)) * (maxDarkness - minDarkness) / ((64.0f*lightRadius) / 2));

				Uint8 alpha = darkness;
				Uint8 red = 0;
				Uint8 green = 0;
				Uint8 blue = 0;

				Uint32 color = (alpha << 24) | (red << 16) | (green << 8) | blue;
				pixels[index] = color;
			}
		}
		return pixels;
	}

	void Set_Screen(const std::vector<Uint32> &pixelData) {
		if (!texture) {
			std::cerr << "Invalid texture: texture is null" << std::endl;
			return;
		}

		void* pixels;
		int pitch;
		if (SDL_LockTexture(texture, nullptr, &pixels, &pitch) != 0) {
			std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
			return;
		}

		Uint32* dst = (Uint32*)pixels;
		std::copy(pixelData.begin(), pixelData.end(), dst);

		SDL_UnlockTexture(texture);
	}

	bool setFogOfWar = false;

	void Render(Component::Camera &camera) {
		if (!setFogOfWar) {
			SDL_DestroyTexture(texture);
			texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, camera.screen.w, camera.screen.h);
			if (!texture) {
				std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
				return;
			}
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			auto pixels = Set_Pixels(camera.screen);
			Set_Screen(pixels);
			setFogOfWar = true;
		}


		// Debug: Check if the texture is being rendered
		SDL_FRect rect = {0, 0, camera.screen.w, camera.screen.h};
		SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &rect);
	}


}// namespace Fog_Of_War