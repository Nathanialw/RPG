#pragma once

#include "entt/entt.hpp"
#include "dialogue.h"
#include "utilities.h"
#include <type_traits>
#include "components.h"
#include "timer.h"
#include "graphics.h"
#include "action_components.h"

namespace Social_Component {

  enum class Race {
    rogue,
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
    eldtritch,
    drow,
    nature,
    animal,
    neutral,
    SIZE
  };

  struct Relationships {
    std::array<int, (int)Race::SIZE>races;
  };

  struct Speaking {
    std::string text;
    uint64_t duration = 0;
    uint64_t count = 1;
  };
}

namespace Social_Control {

  Social_Component::Race Get_Race(std::string raceStr) {
    if (raceStr == "rogue") {
      return Social_Component::Race::rogue;
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
    if (raceStr == "eldtritch") {
      return Social_Component::Race::eldtritch;
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
    int state = races.races[(int)race];
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
}
