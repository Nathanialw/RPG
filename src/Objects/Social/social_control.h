#pragma once

#include "action_components.h"
#include "components.h"
#include "dialogue.h"
#include "entt/entt.hpp"
#include "graphics.h"
#include "rendering_components.h"
#include "timer.h"
#include "utilities.h"
#include <type_traits>

namespace Social_Component {

  enum class Race {
    rogue,
    african,
    chinese,
    indian,
    viking,
    human,
    fleshbeast,
    goblin,
    beast,
    demon,
    elf,
    zombie,
    skeleton,
    orc,
    dwarf,
    monster,
    eldritch,
    drow,
    nature,
    animal,
    neutral,
    SIZE
  };


  struct Relationships {
    std::array<int, (int) Race::SIZE> races;
  };

  enum Summon_Type {
    normal,
    reanimated,
    ghost
  };

  struct Summon {
    Relationships relationships;
    Race race;
    Summon_Type blendType = normal;
    bool summon = false;
  };

  struct Speaking {
    std::string text;
    uint64_t duration = 0;
    uint64_t count = 1;
  };
}// namespace Social_Component

namespace Social_Control {

  std::vector<int> Get_Race_Relationsips(std::string race) {// needs to search for  a specific row that I can input in the arguments
    std::vector<int> raceData;
    //check if the name exists??
    std::string raceInput = db::Append_Quotes(race);

    sqlite3_stmt *stmt;
    char buf[300];
    const char *jj = "SELECT race_enum, rogue, african, chinese, indian, viking, human, fleshbeast, goblin, beast, demon, elf, zombie, skeleton, orc, dwarf, monster, eldritch, drow, nature, animal, neutral FROM race_relationships WHERE race = ";
    strcpy(buf, jj);
    strcat(buf, raceInput.c_str());
    sqlite3_prepare_v2(db::db, buf, -1, &stmt, 0);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      for (int i = 0; i < (int) Social_Component::Race::SIZE; ++i) {
        raceData.emplace_back(sqlite3_column_int(stmt, i));
      }
    }
    raceData.shrink_to_fit();
    return raceData;
  }

  Social_Component::Race Get_Race(std::string raceStr) {
    if (raceStr == "rogue") {
      return Social_Component::Race::rogue;
    }
    if (raceStr == "african") {
      return Social_Component::Race::african;
    }
    if (raceStr == "chinese") {
      return Social_Component::Race::chinese;
    }
    if (raceStr == "indian") {
      return Social_Component::Race::indian;
    }
    if (raceStr == "viking") {
      return Social_Component::Race::viking;
    }
    if (raceStr == "human") {
      return Social_Component::Race::human;
    }
    if (raceStr == "fleshbeast") {
      return Social_Component::Race::fleshbeast;
    }
    if (raceStr == "goblin") {
      return Social_Component::Race::goblin;
    }
    if (raceStr == "beast") {
      return Social_Component::Race::beast;
    }
    if (raceStr == "demon") {
      return Social_Component::Race::demon;
    }
    if (raceStr == "elf") {
      return Social_Component::Race::elf;
    }
    if (raceStr == "zombie") {
      return Social_Component::Race::zombie;
    }
    if (raceStr == "skeleton") {
      return Social_Component::Race::skeleton;
    }
    if (raceStr == "orc") {
      return Social_Component::Race::orc;
    }
    if (raceStr == "dwarf") {
      return Social_Component::Race::dwarf;
    }
    if (raceStr == "monster") {
      return Social_Component::Race::monster;
    }
    if (raceStr == "eldritch") {
      return Social_Component::Race::eldritch;
    }
    if (raceStr == "drow") {
      return Social_Component::Race::drow;
    }
    if (raceStr == "nature") {
      return Social_Component::Race::nature;
    }
    if (raceStr == "animal") {
      return Social_Component::Race::animal;
    }
    if (raceStr == "neutral") {
      return Social_Component::Race::neutral;
    }
    Utilities::Log("race data not found in db, setting to 'human'");
    return Social_Component::Race::human;
  }

  bool Check_Relationship(entt::registry &zone, entt::entity &unitID, entt::entity &targetID) {
    Social_Component::Race race = zone.get<Social_Component::Race>(targetID);
    auto &races = zone.get<Social_Component::Relationships>(unitID);
    int state = races.races[(int) race];
    //        zone.get<Social_Component::Relationships>(unitID).races.at(targetID);

    if (state < 33) {
      return true;
    }
    return false;
  }

  bool Enemy_Selected(entt::registry &zone, entt::entity &player_ID) {
    auto view = zone.view<Component::Selected>();
    if (view.empty()) {
      return true;
    } else {
      for (auto unit: view) {
        if (Social_Control::Check_Relationship(zone, player_ID, unit)) {
          return true;
        }
      }
    }
    return false;
  };

  void Show_Dialogue(entt::registry &zone, Component::Camera &camera) {
    auto view = zone.view<Component::Unit, Component::Interaction_Rect, Component::Renderable, Social_Component::Speaking>();
    for (auto entity: view) {
      auto &interactionRect = view.get<Component::Interaction_Rect>(entity);
      auto &speaking = view.get<Social_Component::Speaking>(entity);

      Component::Position itemPosition = {interactionRect.rect.x + (interactionRect.rect.w / 2.0f), interactionRect.rect.y};
      Graphics::Text_Box_Data itemTextBox = Graphics::Create_Text_Background(camera, {255, 255, 255}, speaking.text, itemPosition);

      SDL_RenderFillRect(Graphics::renderer, &itemTextBox.textBoxBackground);
      SDL_RenderCopyF(Graphics::renderer, itemTextBox.textdata.pTexture, &itemTextBox.textdata.k, &itemTextBox.highlightBox);
      SDL_DestroyTexture(itemTextBox.textdata.pTexture);

      speaking.count += Timer::timeStep;
      if (speaking.count >= speaking.duration) {
        zone.remove<Social_Component::Speaking>(entity);
      }
    }
  }

  void Interact(entt::registry &zone, entt::entity &unitID, std::string text_type) {
    if (!zone.any_of<Social_Component::Speaking>(unitID)) {
      auto &text = zone.emplace_or_replace<Social_Component::Speaking>(unitID);
      //set text
      text.text = Dialogue::Get_Random_Dialogue(text_type);
      //set duration
      text.duration = 2000 + (text.text.length() * 100);
    }
  }

  void Greet(entt::registry &zone, entt::entity &unitID, entt::entity &targetID) {
    auto &action = zone.get<Action_Component::Action>(unitID);
    Action_Component::Set_State(action, Action_Component::talk);

    std::string text_type = "greeting";
    Interact(zone, unitID, text_type);
  }

  bool Summon(entt::registry &zone, entt::entity &entity, Social_Component::Summon &summon, Rendering_Components::Blend_Type &blendType) {
    if (summon.summon) {
      if (summon.blendType == Social_Component::ghost) {
        blendType = Rendering_Components::ghost;
      } else if (summon.blendType == Social_Component::reanimated) {
        blendType = Rendering_Components::reanimated;
      } else {
        blendType = Rendering_Components::normal;
      }
      auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
      zone.emplace_or_replace<Social_Component::Race>(entity, summon.race);
      relationships.races = summon.relationships.races;
      return true;
    }
    return false;
  }

  void Entity(entt::registry &zone, entt::entity &entity, std::string &race) {
    zone.emplace_or_replace<Social_Component::Race>(entity, Social_Control::Get_Race(race));
    auto &relationships = zone.emplace_or_replace<Social_Component::Relationships>(entity);
    auto raceData = Get_Race_Relationsips(race);
    for (int i = 0; i < raceData.size(); i++) {
      relationships.races[i] = raceData[i + 1];
    }
  }
}// namespace Social_Control
