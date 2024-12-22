#pragma once

#include "SDL2/SDL.h"
#include "components.h"
#include "entt/entt.hpp"
#include "graphics.h"
#include "portraits.h"
#include <array>
#include <string>
#include "Rendering/cox_rendering.h"

namespace Unit_Frames {


    struct UI_Frame {
	UI::Image_Frame background;
	//std::vector<Frame_Element> frames;
	Rendering_Components::Unit_Frame_Portrait gear;
	UI::Image_Frame img;
	UI::Text_Frame name;
	UI::Text_Frame health;
	bool selected = false;
	bool occupied = false;
    };

    // std::unordered_map<entt::entity, UI_Frame> frames;
    UI::Image_Frame targetFrame;
    std::array<UI_Frame, 12> targets;

    SDL_FRect Scale_Text(f2 &scale, SDL_FRect rect, std::string text, float textSize) {
	rect.w = (text.length() * (textSize / 2.0f));
	rect.h = (textSize);
	return UI::Update_Rect_Size(scale, rect);
    }

    void Populate_Frame(UI::Text_Frame &frame, std::string text, SDL_Color color) {
	frame.text = text;
	auto d = Graphics::Load_Text_Texture(text, color);
	frame.textTexture = d.pTexture;
    }

    void Update_Frame_Text(f2 scale, UI::Text_Frame &frame, std::string text, float size) {
	frame.textFrame = Scale_Text(scale, frame.textFrame, text, size);
	frame.textFrame = UI::Center_Rect_In_frame(frame.textFrame, frame.frame);
    }

    void Build_Target_Frames() {
	//only rebuild if changed
	float yPos = targetFrame.frame.y;
	for (auto &frame: targets) {
	    frame.health.frame = {targetFrame.frame.x + targetFrame.frame.h, yPos + targetFrame.frame.h / 2.0f, targetFrame.frame.w - targetFrame.frame.h, targetFrame.frame.h / 2.0f};
	    frame.img.frame = {targetFrame.frame.x, yPos, targetFrame.frame.h, targetFrame.frame.h};
	    frame.name.frame = {targetFrame.frame.x + targetFrame.frame.h, yPos, targetFrame.frame.w - targetFrame.frame.h, targetFrame.frame.h / 2.0f};
	    yPos += targetFrame.frame.h;
	}
    }

    void Update_Frame_Data(f2 &scale, Component::Name &fullName, Component::Health &health, UI_Frame &frame, Rendering_Components::Unit_Frame_Portrait portrait, Component::Sprite_Icon &spriteIcon) {

	//only update if changed
	std::string healthText = std::to_string(health.currentHealth) + " / " + std::to_string(health.maxHealth);
	if (frame.health.text != healthText || frame.health.textTexture == NULL) {
	    SDL_DestroyTexture(frame.health.textTexture);
	    frame.health.textTexture = NULL;
	    frame.health.healthFrameWidth = (float) health.currentHealth / (float) health.maxHealth;
	    Populate_Frame(frame.health, healthText, Color::gray);
	    frame.health.backgroundTexture = Texture::tooltipBackground;
	}
	Update_Frame_Text(scale, frame.health, frame.health.text, 20.0f);

	COX_Render::Update_Sprite(spriteIcon);
//        if (frame.img.texture != portrait.texture || frame.img.texture == NULL) {
//            frame.img.texture = portrait.texture;
//        }
	if (frame.img.texture != spriteIcon.texture || spriteIcon.texture == NULL) {
	    frame.img.texture = spriteIcon.texture;
	}
	frame.img.clipRect = spriteIcon.clipRect;

	std::string name = fullName.first + " " + fullName.last;
	if (frame.name.text != name || frame.name.textTexture == NULL) {
	    SDL_DestroyTexture(frame.name.textTexture);
	    frame.name.textTexture = NULL;
	    frame.name.backgroundTexture = Texture::tooltipBackground;
	    Populate_Frame(frame.name, name, Color::orange);
	}
	Update_Frame_Text(scale, frame.name, frame.name.text, 20.0f);
    }

    void Render_Target_Frame(UI_Frame &frame) {
	//background
	SDL_RenderCopyF(Graphics::renderer, frame.background.texture, NULL, &frame.background.frame);
	//health background
	SDL_RenderCopyF(Graphics::renderer, frame.health.backgroundTexture, NULL, &frame.health.frame);
	//health foreground
	frame.health.healthFrame = frame.health.frame;
	frame.health.healthFrame.w = frame.health.frame.w * frame.health.healthFrameWidth;
	Color::Set_Health_Color(Graphics::renderer, frame.health.healthFrameWidth);
	SDL_RenderFillRectF(Graphics::renderer, &frame.health.healthFrame);
	//health
	SDL_RenderCopyF(Graphics::renderer, frame.health.textTexture, NULL, &frame.health.textFrame);
	//name background
	SDL_RenderCopyF(Graphics::renderer, frame.name.backgroundTexture, NULL, &frame.name.frame);
	//name
	SDL_RenderCopyF(Graphics::renderer, frame.name.textTexture, NULL, &frame.name.textFrame);
	//img
	SDL_RenderCopyF(Graphics::renderer, frame.img.texture, &frame.img.clipRect, &frame.img.frame);
    }

    void Init_Frames(Component::Camera &camera) {
	SDL_FRect position = {32.0f, 256.0f, 256.0f, 64.0f};
	targetFrame.frame  = { (Graphics::Screen.w * 0.5f) + (position.x / camera.scale.x), Graphics::Screen.h - (position.y / camera.scale.y), position.w / camera.scale.x, position.h / camera.scale.y};
    }

    void Show_Frames(entt::registry &zone, Component::Camera &camera) {
	if (1) {//if scale resized
	    Init_Frames(camera);
	    Build_Target_Frames();
	}

	for (auto &i: targets) {
	    i.selected = false;
	}

	auto view = zone.view<Component::Selected, Component::Name, Component::Health, Rendering_Components::Unit_Frame_Portrait, Component::Sprite_Icon>();
	for (auto entity: view) {
	    auto [selected, fullName, health, portrait, spriteIcon] = view.get(entity);
	    //place in new unit
	    if (!selected.selected) {
		for (int i = 0; i < targets.size(); i++) {
		    if (!targets[i].occupied) {
			selected.targetIndex = i;
			targets[i].occupied = true;
			selected.selected = true;
			break;
		    };
		}
	    }
	    targets[selected.targetIndex].selected = true;
	    Update_Frame_Data(camera.scale, fullName, health, targets[selected.targetIndex], portrait, spriteIcon);
	    Render_Target_Frame(targets[selected.targetIndex]);
	}

	//clear the frame, (move all the others an index up?)
	for (auto &i: targets) {
	    if (!i.selected) {
		i.occupied = false;
		SDL_DestroyTexture(i.name.textTexture);
		i.name.textTexture = NULL;
		i.name.text = "";
		SDL_DestroyTexture(i.health.textTexture);
		i.health.textTexture = NULL;
		i.health.text = "";
		i.img.texture = NULL;
	    }
	}
    }
}// namespace Unit_Frames
