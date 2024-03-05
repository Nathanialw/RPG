#pragma once
#include "spell_data.h"


namespace Mouse_Struct {
  struct Entity_ID_Direction {
    entt::entity entity;
    std::string name;
    int direction;
  };

  struct {
    entt::entity mouseItem{};
    entt::entity cursor_ID{};
    int index{};
    Spell_Data::Skill_Tree tree;
    Component::Icon_Type type = Component::Icon_Type::none;
    std::string name;
    int direction{};
    bool itemCurrentlyHeld = false;
  } mouseData;
}// namespace Mouse_Struct