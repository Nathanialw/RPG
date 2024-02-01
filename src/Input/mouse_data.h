#pragma once
#include "spell_data.h"


namespace Mouse_Struct {
  struct Mouse_Data {
    entt::entity mouseItem;
    entt::entity cursor_ID;
    //    Spell_Data::Spell spell;
    int index;
    Spell_Data::Skill_Tree tree;
    Component::Icon_Type type = Component::Icon_Type::none;
    std::string name;
    int direction;
    bool itemCurrentlyHeld = false;
  };
  Mouse_Data mouseData;

}// namespace Mouse_Struct