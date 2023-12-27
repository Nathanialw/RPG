#pragma once
#include "item_components.h"
#include "components.h"
#include "action_components.h"
#include "Fire/fire.h"
#include "sinister_strike.h"

namespace Spell_Data {

  int Sinister_Strike(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int &index) {
    Sinister_Strike::Instant_Attack(zone, entity);
    return 0;
  }


  enum Skill_Tree {
    fire,
    cold,
    warrior,
    archery,
    life,
    death,
    SIZE
  };

  class Spell {
    std::string name;
    Component::On_Mouse mouseItem;
    //spell data
    int minDamage = 0;
    int MaxDamage;
    int range;

  public:
    Component::Icon icon;
    Fire::castSpell cast = Sinister_Strike;

    //constructor
    Spell() {

    }

    Spell(SDL_Texture* texture) {
      icon.pBackground = Graphics::emptyBagIcon;
      icon.pTexture = texture;
      icon.pIconRarityBorder = Item_Component::rarityBorder[Item_Component::Rarity::common];
      icon.pIconBorder = Graphics::bagSlotBorder;
      icon.clipSprite = {0, 0, 256, 256};
      icon.clipIcon = {0, 0, 256, 256};
      icon.renderRectSize = {64.0f, 64.0f};
      icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};
      mouseItem.type = Component::Icon_Type::spell;

      name = "";
      minDamage = 0;
      MaxDamage = 0;
      range = 0;
    }

    Spell(SDL_Texture* texture, Fire::castSpell spell) {
      icon.pBackground = Graphics::emptyBagIcon;
      icon.pTexture = texture;
      icon.pIconRarityBorder = Item_Component::rarityBorder[Item_Component::Rarity::common];
      icon.pIconBorder = Graphics::bagSlotBorder;
      icon.clipSprite = {0, 0, 256, 256};
      icon.clipIcon = {0, 0, 256, 256};
      icon.renderRectSize = {64.0f, 64.0f};
      icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};
      mouseItem.type = Component::Icon_Type::spell;
      cast = spell;

      name = "";
      minDamage = 0;
      MaxDamage = 0;
      range = 0;
    }
    //function to cast the spell
    static int Cast(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index) {
      Fire::Cast_Spell(zone, entity, action, index);
      return 0;
    }
  };
}