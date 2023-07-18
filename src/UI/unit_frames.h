#pragma once
#include "SDL2/SDL.h"
#include "components.h"
#include "entt/entt.hpp"
#include "map"
#include <string>
#include <unordered_map>
#include "graphics.h"
#include "ui_elements.h"

namespace Unit_Frames {

  struct Frame_Element {
    SDL_Texture * backTexture;
    SDL_Texture * textTexture;
    SDL_FRect frame;
    SDL_FRect textFrame;
  };
  
  struct UI_Frame {
    Frame_Element background;
    //std::vector<Frame_Element> frames;
    Frame_Element img;
    Frame_Element name;
    Frame_Element health;
  };

  // std::unordered_map<entt::entity, UI_Frame> frames;
  UI_Frame frame;
  
  void Update_Frame_Health(Frame_Element &frame, Component::Health &health) {
    //high stamina increases your speed and damage
    //as it reduces your become weaker
    //it is the only resources that naturally replenishes
    SDL_DestroyTexture(frame.textTexture);

    std::string currentHealth = std::to_string(health.currentHealth);
    std::string maxHealth = std::to_string(health.maxHealth);

    std::string healthText = currentHealth + " / " + maxHealth;

    SDL_Color blue = { 212, 175, 55, 255 };
    Graphics::Surface_Data currentHealthBox = Graphics::Load_Text_Texture(healthText, blue);;

    SDL_FRect textBox = {};
    float textSize = 20.0f;

    textBox.w = (healthText.length() * (textSize / 2.0f));
    textBox.h = (textSize);

    frame.textFrame = textBox;
    frame.textTexture = currentHealthBox.pTexture;
  }
  //I want it fixed to a part of the screen and pop up when you select some units.

  void Attach_Frame() {
    
  }

  
  void Build_Target_Frame(f2 &scale, std::string &name, Component::Health &health) {
    frame.background.frame =  {500.0f, 500.0f, 256.0f, 64.0f};    
    
    frame.background.frame = UI::Update_Scale(scale, frame.background.frame);

    frame.health.frame = {frame.background.frame.x + frame.background.frame.h, frame.background.frame.y + frame.background.frame.h / 2.0f, frame.background.frame.w - frame.background.frame.h, frame.background.frame.h / 2.0f};
    frame.health.backTexture = Graphics::tooltipBackground;
    Update_Frame_Health(frame.health, health);
    frame.health.textFrame = {frame.background.frame.x + frame.background.frame.h, frame.background.frame.y + frame.background.frame.h / 2.0f, frame.background.frame.w - frame.background.frame.h, frame.background.frame.h / 2.0f};

    frame.img.frame = {frame.background.frame.x, frame.background.frame.y, frame.background.frame.h, frame.background.frame.h} ;
    frame.img.backTexture = Graphics::default_icon;
    frame.img.textTexture = NULL;
    frame.img.textFrame = {frame.background.frame.x, frame.background.frame.y, frame.background.frame.h, frame.background.frame.h} ;

    frame.name.frame = {frame.background.frame.x + frame.background.frame.h, frame.background.frame.y, frame.background.frame.w - frame.background.frame.h, frame.background.frame.h / 2.0f};
    frame.name.backTexture = Graphics::tooltipBackground;

    SDL_Color color = { 255, 155, 222};
    Graphics::Surface_Data nameData = Graphics::Load_Text_Texture(name, color);
    frame.name.textFrame = frame.name.frame;
    SDL_DestroyTexture(frame.name.textTexture);
    frame.name.textTexture = nameData.pTexture;   
  }

  void Update_Frame (entt::entity &entity, Frame_Element &frame, Component::Health &health) {
    //update and reprint health values    
    Update_Frame_Health(frame, health);
    //health bar % of current health

    // change img pic to reflect unit emotional state    
    
  }

  void Show_Frames (entt::registry &zone) {
    auto view = zone.view<Component::Selected, Component::Name, Component::Health>();
    auto cam_view = zone.view<Component::Camera>();

    for (auto camera : cam_view) {
      auto [cam] = cam_view.get(camera);
      for (auto entity : view) {
	auto [selected, fullName, health] = view.get(entity);
	std::string name = fullName.first + " " + fullName.last;
	Build_Target_Frame(cam.scale, name, health);
	//      Update_Frame(entity, frame.health, health);

	//background
	SDL_RenderCopyF(Graphics::renderer, frame.background.backTexture, NULL, &frame.background.frame);
	//health background
	SDL_RenderCopyF(Graphics::renderer, frame.health.backTexture, NULL, &frame.health.frame);
	//health
	SDL_RenderCopyF(Graphics::renderer, frame.health.textTexture, NULL, &frame.health.textFrame);
	//name background
	SDL_RenderCopyF(Graphics::renderer, frame.name.backTexture, NULL, &frame.name.frame);
	//name
	SDL_RenderCopyF(Graphics::renderer, frame.name.textTexture, NULL, &frame.name.textFrame);
	//img
	SDL_RenderCopyF(Graphics::renderer, frame.img.backTexture, NULL, &frame.img.frame);
      }
    }
  }
}
