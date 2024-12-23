//
// Created by nathanial on 12/21/24.
//
#pragma once

#include <utility>

#include "icons.h"
#include "textures.h"
#include "graphics.h"
#include "timer.h"

namespace COX_Render {

    std::string Prepend_Zero(int n) {
	if (n < 10)
	    return "0" + std::to_string(n);

	return std::to_string(n);
    }

    class Graphic {
	bool noRepeat = false;
	int frame = 0;
	int numFrames = 0;
	float voidRender = 0.0f;
	std::string name;
	bool active = false;
	SDL_Texture* texture = nullptr;

    public:
	Graphic() = default;
	Graphic(std::string name, SDL_Texture* texture, bool active, bool noRepeat, int numFrames) : name(std::move(name)), texture(texture), active(active), noRepeat(noRepeat), numFrames(numFrames) {
	    	//
	}

    private:
	std::string Update_Frame() {
	    voidRender += Timer::timeStep;
	    if (voidRender >= 40.0f) {
		voidRender -= 40.0f;

		frame++;
	    }

	    std::string nameStr = name + Prepend_Zero(frame);

	    if (frame > numFrames) {
		frame = 0;
		if (noRepeat)
		    active = false;
	    }
	    return nameStr;
	}

    public:
	void Set_Active() {
	    active = true;
	}

	void Render(SDL_FRect renderRect = {0.0f, 0.0f, Graphics::Screen.w, Graphics::Screen.h}) {
	    if (!active)
		return;

	    std::string nameStr = Update_Frame();
	    SDL_Rect iconClip = Icons::iconClipRects[nameStr].clipRect;

	    SDL_RenderCopyF(Graphics::renderer, texture, &iconClip, &renderRect);
	}
    };

    Graphic voidGraphic{};
    Graphic levelUpGraphic{};


    void Init() {
	voidGraphic = Graphic("etheralvoid_000", Texture::cox_void, true, false, 95);
	levelUpGraphic = Graphic("Level Up_000", Texture::cox_level_up, false, true, 95);
    }


    void Update_Sprite_Frame(Component::Sprite_Icon &spriteIcon) {
	if (!spriteIcon.texture)
	    std::cout << "No texture for " << spriteIcon.name << std::endl;

	if (spriteIcon.maxFrames == 1) {
	    spriteIcon.clipRect = Icons::iconClipRects[spriteIcon.name].clipRect;
	    return;
	}

	//cycle through the frames
	spriteIcon.timer += Timer::timeStep;
	if (spriteIcon.timer >= 150.0f) {
	    spriteIcon.timer -= 150.0f;

	    spriteIcon.frame++;

	    if (spriteIcon.frame > spriteIcon.maxFrames)
		spriteIcon.frame = 1;
	}

	spriteIcon.clipRect = Icons::iconClipRects[spriteIcon.name + std::to_string(spriteIcon.frame)].clipRect;
    }

    void Update_Icon_Frames(entt::registry &zone) {

	auto view = zone.view<Component::Position, Component::Sprite_Icon>();
	for (auto entity: view) {
	    auto &spriteIcon = view.get<Component::Sprite_Icon>(entity);

	    Update_Sprite_Frame(spriteIcon);
	}

    }
}

