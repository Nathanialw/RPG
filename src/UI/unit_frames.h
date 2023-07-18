#pragma once
#include "SDL2/SDL.h"
#include "base_structs.h"
#include "components.h"
#include "entt/entt.hpp"
#include "map"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <string>
#include <unordered_map>
#include "graphics.h"
#include "ui_elements.h"
#include "utilities.h"

namespace Unit_Frames {
  //high stamina increases your speed and damage
  //as it reduces your become weaker
  //it is the only resources that naturally replenishes
  
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
  UI_Frame targetFrame;


  SDL_FRect Scale_Text(f2 &scale, SDL_FRect rect, std::string text, float textSize) {
    rect.w = (text.length() * (textSize / 2.0f));
    rect.h = (textSize);
    return UI::Update_Rect_Size(scale, rect);
  }

  void Update_Frame_Text(f2 scale, Frame_Element &frame, std::string text, SDL_Color color) {

    frame.textTexture = Graphics::Load_Text_Texture(text, color).pTexture;
    frame.textFrame = Scale_Text(scale, frame.textFrame, text, 20.0f);
    frame.textFrame = UI::Center_Rect_In_frame(frame.textFrame, frame.frame);
  }

  void Attach_Frame() {
    
  }
  
  void Build_Target_Frame(UI_Frame &frame) {
    //only rebuild if changed
    
    frame.health.frame = {frame.background.frame.x + frame.background.frame.h, frame.background.frame.y + frame.background.frame.h / 2.0f, frame.background.frame.w - frame.background.frame.h, frame.background.frame.h / 2.0f};
    frame.health.textFrame = frame.health.frame;

    frame.img.frame = {frame.background.frame.x, frame.background.frame.y, frame.background.frame.h, frame.background.frame.h} ;
    frame.img.textFrame = frame.img.frame;

    frame.name.frame = {frame.background.frame.x + frame.background.frame.h, frame.background.frame.y, frame.background.frame.w - frame.background.frame.h, frame.background.frame.h / 2.0f};
    frame.name.textFrame = frame.name.frame;    
  }

  void Update_Frame_Data(f2 &scale, Component::Name &fullName, Component::Health &health, UI_Frame &frame) {
    //only update if changed
    std::string healthText = std::to_string(health.currentHealth) + " / " + std::to_string(health.maxHealth);
    Update_Frame_Text(scale, frame.health, healthText, {100,255,50});    
    frame.health.backTexture = Graphics::tooltipBackground;

    frame.img.backTexture = Graphics::default_icon;
    frame.img.textTexture = NULL;


    frame.name.backTexture = Graphics::tooltipBackground;
    std::string name = fullName.first + " " + fullName.last;
    Update_Frame_Text(scale, frame.name, name, {255,155,155});
  }

  void Render_Target_Frame(UI_Frame &frame) {
    //render every frame
    
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

    SDL_DestroyTexture(frame.name.textTexture);
    SDL_DestroyTexture(frame.health.textTexture);
  }

  void Init_Frames () {
    targetFrame.background.frame =  {512.0f, 512.0f, 256.0f, 64.0f};    
  }

  void Show_Frames (entt::registry &zone, Component::Camera &camera) {
    Init_Frames ();
    targetFrame.background.frame = UI::Update_Scale(camera.scale, targetFrame.background.frame);
    UI_Frame frame = targetFrame;
    
    auto view = zone.view<Component::Selected, Component::Name, Component::Health>();
    for (auto entity : view) {
      auto [selected, fullName, health] = view.get(entity);
      Build_Target_Frame(frame);
      Update_Frame_Data(camera.scale, fullName, health, frame);
      Render_Target_Frame(frame);
      frame.background.frame.y += targetFrame.background.frame.h;      
    }
  }
}
