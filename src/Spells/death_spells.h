#pragma once

#include "Maps/World/map.h"
#include "SQLite_unit_data.h"
#include "collision.h"
#include "components.h"
#include "entt/entt.hpp"
#include "texture_packer.h"

namespace Death_Spells {

  /*
  The x and y can be got rid of and the Position components can be updated elswhere; no reason to couple like that here
  */
  void Summon_Skeleton(entt::registry &zone, int &state, float x, float y, std::string name) {

    int i = rand() % 10 + 1;
    int j = rand() % 10 + 1;
    int k = rand() % 10 + 1;
    int l = rand() % 10 + 1;

    Entity_Loader::Data data = Entity_Loader::parse_data(name);

    auto skeleton0 = zone.create();

    int unit_ID = Create_Entities::Check_For_Template_ID(name);

    SQLite_Spritesheets::Sheet_Data_Flare sheetDataFlare = {};
    std::string sheetname = Entity_Loader::Get_Sprite_Sheet(name);
    std::unordered_map<std::string, Rendering_Components::Sheet_Data_Flare> *flareSheetData = NULL;

    SQLite_Spritesheets::Get_Flare_From_DB(sheetname, sheetDataFlare);
    flareSheetData = Populate_Flare_SpriteSheet(name, sheetDataFlare, Graphics::unitTextures[unit_ID]);

    auto &sprite = zone.emplace_or_replace<Rendering_Components::Sprite_Sheet_Info>(skeleton0);
    sprite.flareSpritesheet = flareSheetData;
    sprite.sheet_name = name;
    sprite.type = sheetDataFlare.sheet_type;
    zone.emplace_or_replace<Rendering_Components::Sprite_Offset>(skeleton0, sheetDataFlare.x_offset, sheetDataFlare.y_offset);

    auto &scale = zone.emplace_or_replace<Component::Scale>(skeleton0, 1.0f);

    zone.emplace_or_replace<Action_Component::Action>(skeleton0, Action_Component::idle);

    auto &position = zone.emplace_or_replace<Component::Position>(skeleton0, x + i - j - k + l, y + j - i - k + l);
    zone.emplace_or_replace<Component::Radius>(skeleton0, data.radius);
    zone.emplace_or_replace<Component::Velocity>(skeleton0, 0.f, 0.0f, 0.f, 0.0f, data.speed);

    zone.emplace_or_replace<Component::Direction>(skeleton0, Component::Direction::SE);
    zone.emplace_or_replace<Component::Name>(skeleton0, name);
    zone.emplace_or_replace<Component::Mass>(skeleton0, data.mass);

    zone.emplace_or_replace<Component::Alive>(skeleton0, true);
    zone.emplace_or_replace<Component::Health>(skeleton0, 30);
    zone.emplace_or_replace<Component::Melee_Damage>(skeleton0, 2, 5);
    zone.emplace_or_replace<Component::Attack_Speed>(skeleton0, 2000.0f, 0.0f);

    zone.emplace_or_replace<Component::Melee_Range>(skeleton0, ((data.radius * 2.0f)));
    zone.emplace_or_replace<Component::Sight_Range>(skeleton0, position.x - 250.0f, position.y - 250.0f, 500.0f, 500.0f);

    zone.emplace_or_replace<Component::Soldier>(skeleton0);
    zone.emplace_or_replace<Component::Commandable>(skeleton0);
    zone.emplace_or_replace<Component::Spellbook>(skeleton0);
    zone.emplace_or_replace<Component::Entity_Type>(skeleton0, Component::Entity_Type::unit);

    bool yes = true;
    Collision::Create_Dynamic_Body(zone, state, skeleton0, position.x, position.y, data.radius, data.mass, yes);
  }

}// namespace Death_Spells