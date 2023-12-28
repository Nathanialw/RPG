#pragma once
#include "components.h"
#include "entt/entt.hpp"
#include "spells.h"

namespace Life {

  int Heal_Self(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    //get data from db
    auto &health = zone.get<Component::Health>(entity);

    health.currentHealth += 10;
    if (health.currentHealth > health.maxHealth) {
      health.currentHealth = health.maxHealth;
    }
    return 0;
  }

  int Heal_Other(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index, float &x, float &y) {
    //get data from db
    SDL_FRect mouseRect = Utilities::Get_FRect_From_Point_Radius(Mouse::cursorRadius, Mouse::iXWorld_Mouse, Mouse::iYWorld_Mouse);
    Quad_Tree::Entity_Data targetData = Quad_Tree::Entity_vs_Mouse_Collision(zone, mouseRect, index);

    if (targetData.b) {
      auto &health = zone.get<Component::Health>(targetData.entity_ID);
      health.currentHealth += 10;
      if (health.currentHealth > health.maxHealth) {
        health.currentHealth = health.maxHealth;
      }
    }
    return 0;
  }
}// namespace Life