#pragma once

#include "action_components.h"
#include "components.h"
#include "item_components.h"
#include "spell_components.h"

namespace Spell_Data {

    enum Skill_Tree {
        fire,
        cold,
        arcane,
        bard,
        earth,
        warrior,
        archery,
        lightning,
        air,
        life,
        death,
        extra,
        SIZE
    };

    class Spell {
        std::string name;
        Component::On_Mouse mouseItem;
        Component::Icon_Type type;
        //spell data
        int minDamage = 0;
        int MaxDamage;
        int range;

    public:
        Component::Icon icon;
        Spells::castSpell cast;
        //    Spells::castEffect effect;
        //    Spells::spellHit hit;
        Skill_Tree tree;
        int index;

        //constructor
        Spell() {
        }

        Spell(SDL_Texture *texture) {
            icon.pBackground = Graphics::emptyBagIcon;
            icon.pTexture = texture;
            icon.pIconRarityBorder = Item_Component::rarityBorder[Item_Component::Rarity::common];
            icon.pIconBorder = Graphics::bagSlotBorder;
            icon.clipSprite = {0, 0, 256, 256};
            icon.clipIcon = {0, 0, 256, 256};
            icon.renderRectSize = {64.0f, 64.0f};
            icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};
            type = Component::Icon_Type::spell;

            name = "";
            minDamage = 0;
            MaxDamage = 0;
            range = 0;
        }

        Spell(SDL_Texture *texture, Spells::castSpell spell) {
            icon.pBackground = Graphics::emptyBagIcon;
            icon.pTexture = texture;
            icon.pIconRarityBorder = Item_Component::rarityBorder[Item_Component::Rarity::common];
            icon.pIconBorder = Graphics::bagSlotBorder;
            icon.clipSprite = {0, 0, 256, 256};
            icon.clipIcon = {0, 0, 256, 256};
            icon.renderRectSize = {64.0f, 64.0f};
            icon.renderPositionOffset = {icon.renderRectSize.x / 2, icon.renderRectSize.y / 2};
            type = Component::Icon_Type::spell;
            cast = spell;

            name = "";
            minDamage = 0;
            MaxDamage = 0;
            range = 0;
        }

        //function to cast the spell
        static int Cast(entt::registry &zone, entt::entity &entity, Action_Component::Action &action, int index, float &x, float &y) {
            //      Spells::Cast_Spell(zone, entity, action, index, x, y);
            return 0;
        }
    };
}// namespace Spell_Data