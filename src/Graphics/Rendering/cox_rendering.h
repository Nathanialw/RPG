//
// Created by nathanial on 12/21/24.
//
#pragma once
#include "icons.h"
#include "textures.h"
#include "graphics.h"

namespace COX_Render {


    //for multiframe icons/map sprites
   struct {
	float voidRender = 0.0f;
	int voidFrame = 0;
    } UI_timer;

    void Update_Sprite(Component::Sprite_Icon &spriteIcon) {
	if (!spriteIcon.texture)
		std::cout << "No texture for " << spriteIcon.name << std::endl;
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

    std::string Append_Zero(int n) {
	if (n < 10)
	    return "0" + std::to_string(n);

	return std::to_string(n);
    }


    void Render_Void() {
	UI_timer.voidRender += Timer::timeStep;
	if (UI_timer.voidRender >= 50.0f) {
	    UI_timer.voidRender -= 50.0f;

	    int end = 95;
	    UI_timer.voidFrame++;

	    if (UI_timer.voidFrame > end)
		UI_timer.voidFrame = 0;
	}

	std::string name = "etheralvoid_000";
	std::string nameStr = name + Append_Zero(UI_timer.voidFrame);
	SDL_Rect iconClip = Icons::iconClipRects[nameStr].clipRect;
	SDL_FRect rect = {0.0f, 0.0f, Graphics::Screen.w, Graphics::Screen.h};

	SDL_RenderCopyF(Graphics::renderer, Texture::cox_void, &iconClip, &rect);
    }
}