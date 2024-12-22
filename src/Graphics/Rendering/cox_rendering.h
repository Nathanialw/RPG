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

    std::string Append_Zero(int n) {
	if (n < 10)
	    return "0" + std::to_string(n);
	return std::to_string(n);
    }

    //for multi-frame icons/map sprites
   struct Graphic {
       bool active = false;
       bool norepeat = false;
       int voidFrame = 0;
       float voidRender = 0.0f;

       void Set_Active() {
	   active = true;
       }

       std::string Update(std::string name) {
	   voidRender += Timer::timeStep;
	   if (voidRender >= 40.0f) {
	       voidRender -= 40.0f;

	       voidFrame++;
	   }

	   name += Append_Zero(voidFrame);

	   if (voidFrame > 95) {
	       voidFrame = 0;
	       if (norepeat)
		   active = false;
	   }
	   return name;
       }
   };

   Graphic voidGraphic = Graphic{true, false, 0, 0.0f};
   Graphic levelUpGraphic = Graphic{false, true, 0, 0.0f};

   void Render_Void() {
       if (!voidGraphic.active)
	   return;

       std::string nameStr = voidGraphic.Update("etheralvoid_000");
       SDL_Rect iconClip = Icons::iconClipRects[nameStr].clipRect;
       SDL_FRect rect = {0.0f, 0.0f, Graphics::Screen.w, Graphics::Screen.h};

       SDL_RenderCopyF(Graphics::renderer, Texture::cox_void, &iconClip, &rect);
   }

    void Render_XP() {
	if (!levelUpGraphic.active)
	    return;

	std::string nameStr = levelUpGraphic.Update("Level Up_000");
	SDL_Rect iconClip = Icons::iconClipRects[nameStr].clipRect;
	SDL_FRect rect = {0.0f, 0.0f, Graphics::Screen.w, Graphics::Screen.h};

	SDL_RenderCopyF(Graphics::renderer, Texture::cox_level_up, &iconClip, &rect);
    }

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

}