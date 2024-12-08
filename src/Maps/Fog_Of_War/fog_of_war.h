//
// Created by desktop on 12/7/24.
//
#pragma once
#include "array"
#include "SDL2/SDL.h"
#include "components.h"
#include "camera.h"

namespace Fog_Of_War {
	float lightRadiusF = 64.0f;
	Uint8 maxDarkness = 250;
	Uint8 minDarkness = 100;

	const long cellSize = 512/16;
	const long SIZE = ((cellSize * 100) * (cellSize * 100));
	std::array<bool, SIZE> fogOfWarPixels;


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

				if (distance > (lightRadiusF * 0.5f) * (lightRadiusF * 0.5f))
					continue;
				else
					fogOfWarPixels[index] = true;
			}
		}
	}

	void Init() {
		for (int i = 0; i < SIZE; ++i) {
			fogOfWarPixels[i] = false;
		}
	}

	void Update(int x, int y, int viewW, int lightStr) {
		Set_Pixels(x, y, viewW, lightStr);
	}

	void asd(entt::registry &zone, Component::Camera &camera) {
		auto view = zone.view<Component::Position, Component::Light_Radius, Component::Input>();
		for (auto entity : view) {
			auto light = view.get<Component::Light_Radius>(entity);
			auto position = view.get<Component::Position>(entity);
			auto screenPosition = Camera_Control::Convert_Position_To_Screen_Coods(camera, position);
			Set_Pixels(screenPosition.x, screenPosition.y, camera.screen.w, light.lightRadius);
		}
	}
}