#pragma once
#include "SQLite_unit_data.h"
#include "components.h"
#include "entt/entt.hpp"
#include "graphics.h"
#include <iostream>

#include "Maps/World/world.h"

namespace Units {

  void Create_Archer(entt::registry &zone, float x, float y) {
    for (auto j = 0; j < x; ++j) {
      for (auto i = 0; i < y; ++i) {
        auto archer = zone.create();//creates a unique name for an entity
        //			zone.emplace_or_replace<Component::Sprite_Sheet_Info>(archer, Graphics::archer_0); /// need to load the texture only onceand pass the pointer into this function
        //			zone.get<Component::Sprite_Sheet_Info>(archer).sheet = { //populate the vector
        //				{ NULL },
        //				{ {0   , 0, 128, 128}, 0,    512,  1, 0, 75, 0 },//idle
        //				{ {512,  0, 128, 128}, 512,  1024, 0, 0, 75, 0 },//walk
        //				{ {1536, 0, 128, 128}, 1536, 512,  0, 0, 75, 0 },//attack
        //
        //				{ {2048, 0, 128, 128}, 2048, 256,  0, 0, 75, 0 },//struck
        //				{ {2304, 0, 128, 128}, 2560, 768,  0, 0, 75, 0 }, //dying
        //				{ {3072, 0, 128, 128}, 2816, 512,  0, 0, 75, 0 }, //cheer
        //				{ {3584, 0, 128, 128}, 3584, 512,  1, 0, 75, 0 },//range
        //			};
        zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(archer, 60.0f, 95.0f);

        zone.emplace_or_replace<Action_Component::Action>(archer, Action_Component::idle);
        //			zone.get<Component::Action>(archer).frameCount = {{0, 0}, {4, 0}, {8, 0}, {4, 0}, {0, 0}, {2, 0}, {5, 0}, {4, 0}, {4, 0} };

        zone.emplace_or_replace<Component::Position>(archer, 400.0f, 500.0f);

        zone.emplace_or_replace<Component::Radius>(archer, 15.0f);
        zone.emplace_or_replace<Component::Velocity>(archer, 0.f, 0.0f, 0.f, 0.0f, 0.175f);

        zone.emplace_or_replace<Component::Direction>(archer, Component::Direction::SE);
        zone.emplace_or_replace<Component::Name>(archer, "archer");
        zone.emplace_or_replace<Component::Mass>(archer, 200.0f);

        zone.emplace_or_replace<Component::Health>(archer, 5);
        zone.emplace_or_replace<Component::Alive>(archer, true);

        zone.emplace_or_replace<Component::Soldier>(archer);
        zone.emplace_or_replace<Component::Commandable>(archer);
      }
    }
  }


}// namespace Units
