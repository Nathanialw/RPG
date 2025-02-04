#pragma once

#include "components.h"
#include "unit_frames.h"

namespace UI_Resources {
    //health is health

    //mana used for neutral magic

    //shows Faith resource / demon resource,
    //piety increases faith decreases the demon resource and vice versa
    //faith used to heal and damage undead and demons, demon resource for demon magic and summoning

    //high stamina increases your speed and damage
    //as it reduces your become weaker
    //it is the only resources that naturally replenishes

    Unit_Frames::UI_Frame playerFrame;

    void Build_Target_Frame(Unit_Frames::UI_Frame &frame) {
	//only rebuild if changed
	float yPos = frame.background.frame.y;
	frame.health.frame = {frame.background.frame.x + frame.background.frame.h, yPos + frame.background.frame.h / 2.0f, frame.background.frame.w - frame.background.frame.h, frame.background.frame.h / 2.0f};

	frame.img.frame = {frame.background.frame.x, yPos, frame.background.frame.h, frame.background.frame.h};

	frame.name.frame = {frame.background.frame.x + frame.background.frame.h, yPos, frame.background.frame.w - frame.background.frame.h, frame.background.frame.h / 2.0f};
	yPos += frame.background.frame.h;
    }

    void Init_Frames(Component::Camera &camera) {
	SDL_FRect position = {256.0f + 32.0f, 256.0f, 256.0f, 64.0f};
	playerFrame.background.frame = { (Graphics::Screen.w * 0.5f) - (position.x / camera.scale.x), Graphics::Screen.h - (position.y / camera.scale.y), position.w / camera.scale.x, position.h / camera.scale.y};
    }

    void Show_Frames(entt::registry &zone, Component::Camera &camera) {
	Init_Frames(camera);

	auto view = zone.view<Component::Name, Component::Health, Component::Input, Rendering_Components::Unit_Frame_Portrait, Component::Sprite_Icon>();
	for (auto entity: view) {
	    auto [fullName, health, input, portrait, spriteIcon] = view.get(entity);
	    Build_Target_Frame(playerFrame);
	    Unit_Frames::Update_Frame_Data(camera.scale, fullName, health, playerFrame, portrait, spriteIcon);
	    Unit_Frames::Render_Target_Frame(playerFrame);
	}
    }
}
