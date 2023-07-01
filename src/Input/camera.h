#pragma once
#include "entt/entt.hpp"
#include <SDL2/SDL.h>
#include "../Components/components.h"


namespace Camera_Control {

	class camera_object {
	};

	void Update_Camera_Follow(Component::Camera &camera, Component::Position &position, SDL_FRect &resolution) {
			//center camera on the entity with the component
			camera.screen.w = resolution.w / camera.scale.x;
			camera.screen.h = resolution.h / camera.scale.y;
			camera.screen.x = ((position.x) - (camera.screen.w / 2));
			camera.screen.y = ((position.y) - (camera.screen.h / 2));
	}

    SDL_FRect Maintain_Scale(entt::registry& zone, SDL_FRect& rect, Component::Camera& camera) {

        SDL_FRect fRenderToScreen = {
                float(rect.x) * camera.scale.x,
                float(rect.y) * camera.scale.y,
                (float(rect.w) * camera.scale.x),
                (float(rect.h) * camera.scale.y) };

        return fRenderToScreen;
    }

	SDL_FRect Convert_Rect_To_Screen_Coods(entt::registry& zone, SDL_FRect& frect, Component::Camera& camera) {

		SDL_FRect screenRect = {};

		screenRect.x = frect.x - camera.screen.x;
		screenRect.y = frect.y - camera.screen.y;
		screenRect.w = frect.w;
		screenRect.h = frect.h;

		return screenRect;
}


	SDL_Rect Convert_Rect_To_Scale(entt::registry &zone, SDL_Rect& rect, Component::Camera& camera) {

		SDL_FRect fRenderToScreen = {
			float(rect.x) / camera.scale.x,
			float(rect.y) / camera.scale.y,
			(float(rect.w) / camera.scale.x),
			(float(rect.h) / camera.scale.y) };

		return Utilities::SDL_FRect_To_SDL_Rect(fRenderToScreen);

	}

	SDL_FRect Convert_FRect_To_Scale(entt::registry& zone, SDL_FRect& rect, Component::Camera& camera) {

		SDL_FRect fRenderToScreen = {
			float(rect.x) / camera.scale.x,
			float(rect.y) / camera.scale.y,
			(float(rect.w) / camera.scale.x),
			(float(rect.h) / camera.scale.y) };

		return fRenderToScreen;

	}

	SDL_Point Convert_Point_To_Scale(SDL_Point& rect, Component::Camera &camera) {

		SDL_FPoint fRenderToScreen = {
			float(rect.x) / camera.scale.x,
			float(rect.y) / camera.scale.y };

		return Utilities::SDL_FPoint_to_Point(fRenderToScreen);

	}


	SDL_FPoint Convert_FPoint_To_Scale(SDL_FPoint& rect, Component::Camera& camera) {

		SDL_FPoint fRenderToScreen = {
			float(rect.x) / camera.scale.x,
			float(rect.y) / camera.scale.y };

		return fRenderToScreen;

	}
}
