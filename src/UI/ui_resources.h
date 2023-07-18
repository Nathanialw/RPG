#pragma once
#include "camera.h"
#include "components.h"
#include "graphics.h"
#include "unit_frames.h"
#include "utilities.h"

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
  
  void Init_Frames () {
    playerFrame.background.frame =  {256.0f, 256.0f, 256.0f, 64.0f};    
  }

  void Show_Frames (entt::registry &zone, Component::Camera &camera) {    
    Init_Frames ();
    playerFrame.background.frame = UI::Update_Scale(camera.scale, playerFrame.background.frame);
    
    auto view = zone.view<Component::Name, Component::Health, Component::Input>();
    for (auto entity : view) {
      auto [fullName, health] = view.get(entity);
      Unit_Frames::Build_Target_Frame(playerFrame);
      Unit_Frames::Update_Frame_Data(camera.scale, fullName, health, playerFrame);
      Unit_Frames::Render_Target_Frame(playerFrame);
    }
  }  
}
