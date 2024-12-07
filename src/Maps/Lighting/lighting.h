#pragma once

#include "Input/camera.h"
#include "SDL2/SDL.h"
#include "array"
#include "components.h"
#include "entt/entt.hpp"

namespace Lighting {

	float lightRadiusF = 64.0f;
	Uint8 maxDarkness = 250;
	Uint8 minDarkness = 100;
	SDL_Texture *texture;
	std::vector<Uint32> fogOfWarPixels;
	bool setFogOfWar = false;

	void Sight_Increase(entt::registry &zone) {
		std::for_each(zone.view<Component::Light_Radius, Component::Input>().begin(), zone.view<Component::Light_Radius, Component::Input>().end(), [&](const auto &item) {
			auto &lightRadius = zone.get<Component::Light_Radius>(item).lightRadius;
			setFogOfWar = false;
			lightRadius++;
			if (lightRadius >= 35)
				lightRadius = 35;
		});
	}

	void Sight_Decrease(entt::registry &zone) {
		std::for_each(zone.view<Component::Light_Radius, Component::Input>().begin(), zone.view<Component::Light_Radius, Component::Input>().end(), [&](const auto &item) {
			auto &lightRadius = zone.get<Component::Light_Radius>(item).lightRadius;
			setFogOfWar = false;
			lightRadius--;
			if (lightRadius <= 5)
				lightRadius = 5;
		});
	}

	void Set_Pixels(int x, int y, int viewW, int lightStr) {
		Uint8 red = 0;
		Uint8 green = 0;
		Uint8 blue = 0;

		int radiusW =  lightStr * (lightRadiusF * 0.75f);
		int radiusH =  lightStr * (lightRadiusF * 0.5f);

		int originX = x - (radiusW * 0.5f);
		int originY = y - (radiusH * 0.5f);
		int width = x + (radiusW * 0.5f);
		int height = y + (radiusH * 0.5f);

		for (int i = originY; i < height; i++) {
			for (int j = originX; j < width; j++) {
				int index = i * viewW + j;

				if (index < 0 || index >= fogOfWarPixels.size())
					continue;

				if (i < 0 || i >= viewW || j < 0 || j >= viewW)
					continue;

				Uint8 darkness;
				float dx = (j - x) / (lightStr * 0.75f);
				float dy = (i - y) / (lightStr * 0.5f);
				float distance = (dx * dx) + (dy * dy);

				if (distance <= 1.0f)
					fogOfWarPixels[index] = (minDarkness << 24) | (red << 16) | (green << 8) | blue;
				else if (distance > (lightRadiusF * 0.5f) * (lightRadiusF * 0.5f))
					continue;
				else {
					darkness = (Uint8)(minDarkness + sqrt(distance) * (maxDarkness - minDarkness) / (lightRadiusF * 0.5f));
					Uint8 existingDarkness = fogOfWarPixels[index] >> 24;

					if (existingDarkness < darkness)
						darkness = existingDarkness;

					Uint32 color = (darkness << 24) | (red << 16) | (green << 8) | blue;
					fogOfWarPixels[index] = color;
				}
			}
		}
	}

	void Set_Screen() {
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
		std::copy(fogOfWarPixels.begin(), fogOfWarPixels.end(), dst);

		SDL_UnlockTexture(texture);
	}

	void Init(Component::Camera &camera) {
		if (!setFogOfWar) {
			if (texture)
				SDL_DestroyTexture(texture);
			texture = SDL_CreateTexture(Graphics::renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, camera.screen.w, camera.screen.h);
			if (!texture) {
				std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
				return;
			}
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			Uint8 alpha = maxDarkness;
			Uint8 red = 0;
			Uint8 green = 0;
			Uint8 blue = 0;
			Uint32 color = (alpha << 24) | (red << 16) | (green << 8) | blue;
			fogOfWarPixels.resize(camera.screen.w * camera.screen.h, color);
			for (auto &pixel : fogOfWarPixels)
				pixel = color;
			fogOfWarPixels.shrink_to_fit();
			setFogOfWar = true;
		}
	}

	void Display(Component::Camera &camera) {
		SDL_FRect rect = {0, 0, camera.screen.w, camera.screen.h};
		SDL_RenderCopyF(Graphics::renderer, texture, nullptr, &rect);
	}

	void Get_Emitters(entt::registry &zone, Component::Camera &camera) {
		Uint8 alpha = maxDarkness;
		Uint8 red = 0;
		Uint8 green = 0;
		Uint8 blue = 0;
		Uint32 color = (alpha << 24) | (red << 16) | (green << 8) | blue;
		fogOfWarPixels.resize(camera.screen.w * camera.screen.h, color);
		for (auto &pixel : fogOfWarPixels)
			pixel = color;

		auto view = zone.view<Component::Position, Component::Light_Radius, Component::Renderable>();
		for (auto entity : view) {
			auto light = view.get<Component::Light_Radius>(entity);
			auto position = view.get<Component::Position>(entity);
			auto screenPosition = Camera_Control::Convert_Position_To_Screen_Coods(camera, position);
			Set_Pixels(screenPosition.x, screenPosition.y, camera.screen.w, light.lightRadius);
		}
	}

	void Render(entt::registry &zone, Component::Camera &camera) {
		Init(camera);
		Get_Emitters(zone, camera);
		Set_Screen();
		Display(camera);
	}


}// namespace Fog_Of_War